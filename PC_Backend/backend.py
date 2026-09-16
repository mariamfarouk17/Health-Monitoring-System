"""
backend.py

STEP 3 - Python backend: Serial Receiver + Packet Parser + WebSocket Server.

Data flow:
    STM32 --UART--> COM5 --> pyserial --> parse_packet() (from serial_receiver.py)
        --> JSON --> WebSocket broadcast --> connected browsers

Run with:
    python backend.py

No frontend, no Flask, no HTTP. Pure asyncio + websockets + pyserial.
"""

import asyncio
import json

import serial
import websockets

from serial_receiver import parse_packet

# ============================================================
# CONFIGURATION - change these to match your setup
# ============================================================
SERIAL_PORT = "COM5"
BAUD_RATE = 4800
SERIAL_TIMEOUT_SEC = 2

WEBSOCKET_HOST = "localhost"
WEBSOCKET_PORT = 8765

# Set of currently connected WebSocket clients
CONNECTED_CLIENTS = set()


async def handle_client(websocket):
    """
    Called once per browser connection. Keeps the client registered
    in CONNECTED_CLIENTS until it disconnects, so the serial loop
    knows who to broadcast to.
    """
    CONNECTED_CLIENTS.add(websocket)
    print(f"[WebSocket] Client connected. Total clients: {len(CONNECTED_CLIENTS)}")

    try:
        # We don't expect messages FROM the browser in this step,
        # but we keep the connection open and listen so we detect
        # disconnects promptly.
        async for _ in websocket:
            pass
    except websockets.exceptions.ConnectionClosed:
        pass
    finally:
        CONNECTED_CLIENTS.discard(websocket)
        print(f"[WebSocket] Client disconnected. Total clients: {len(CONNECTED_CLIENTS)}")


async def broadcast(json_message: str):
    """
    Send json_message to every currently connected client.
    If a client has silently died, drop it instead of crashing.
    """
    if not CONNECTED_CLIENTS:
        return  # No one listening - just skip, keep serial loop alive

    dead_clients = set()

    for client in CONNECTED_CLIENTS:
        try:
            await client.send(json_message)
        except websockets.exceptions.ConnectionClosed:
            dead_clients.add(client)

    for client in dead_clients:
        CONNECTED_CLIENTS.discard(client)


async def serial_loop():
    """
    Opens the serial port and continuously reads/parses/broadcasts
    STM32 health packets. Runs forever inside the asyncio event loop
    without blocking the WebSocket server.
    """
    print(f"[Serial] Opening {SERIAL_PORT} at {BAUD_RATE} baud...")

    try:
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            timeout=SERIAL_TIMEOUT_SEC,
        )
    except serial.SerialException as e:
        print(f"[Serial] ERROR: Could not open {SERIAL_PORT}: {e}")
        print("[Serial] Check the port name and that no other program has it open.")
        return

    print(f"[Serial] Connected. Reading {SERIAL_PORT}...")

    while True:
        try:
            raw_bytes = ser.readline()
        except serial.SerialException as e:
            print(f"[Serial] ERROR: Read failed (device disconnected?): {e}")
            break

        if not raw_bytes:
            # Timeout, no data yet - yield control and keep going
            await asyncio.sleep(0)
            continue

        try:
            line = raw_bytes.decode("utf-8", errors="replace").strip()
        except UnicodeDecodeError:
            print("[Serial] WARNING: Undecodable bytes received, skipping.")
            continue

        if not line:
            continue

        parsed = parse_packet(line)

        if parsed is None:
            print(f"[Serial] WARNING: Malformed packet ignored: {line!r}")
            continue

        print(f"[Serial] Received: {line}")
        print(f"[Serial] Parsed:   {parsed}")

        json_message = json.dumps(parsed)
        await broadcast(json_message)

        # Let the event loop breathe so WebSocket handling stays responsive
        await asyncio.sleep(0)

    if ser.is_open:
        ser.close()
    print("[Serial] Port closed. Serial loop stopped.")


async def main():
    print(f"[WebSocket] Starting server at ws://{WEBSOCKET_HOST}:{WEBSOCKET_PORT}")

    async with websockets.serve(handle_client, WEBSOCKET_HOST, WEBSOCKET_PORT):
        await serial_loop()  # Runs forever alongside the WebSocket server


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\n[Backend] Stopped by user (Ctrl+C).")