"""
serial_receiver.py

STEP 2 - PC-side Python serial receiver for the STM32 health monitoring
test packet.

Reads lines of the form:
    HR=78;STEPS=1250;DIST=0.8;CAL=42;BAT=82\r\n
from the STM32 over UART (via USB-to-TTL adapter / ST-Link VCP), parses
them into a Python dictionary, and prints both the raw and parsed data.

This is a simple, synchronous, terminal-only test tool.
No Flask, no WebSocket, no frontend yet.

Dependency: pyserial only.
"""

import serial
import time

# ============================================================
# CONFIGURATION - change these to match your setup
# ============================================================
SERIAL_PORT = "COM5"      # <-- change to your actual COM port (Device Manager)
BAUD_RATE = 4800        # <-- must match MUSART2_vInit() baud rate on STM32
SERIAL_TIMEOUT_SEC = 2    # readline() will wait at most this long for a line

# Expected packet keys -> (dictionary field name, type-cast function)
PACKET_FIELDS = {
    "HR":    ("heart_rate", int),
    "STEPS": ("steps",      int),
    "DIST":  ("distance",   float),
    "CAL":   ("calories",   int),
    "BAT":   ("battery",    int),
}


def parse_packet(line: str):
    """
    Parse a semicolon-separated key=value packet string into a dictionary.

    Example input:
        "HR=78;STEPS=1250;DIST=0.8;CAL=42;BAT=82"

    Returns:
        dict on success, e.g.
            {"heart_rate": 78, "steps": 1250, "distance": 0.8,
             "calories": 42, "battery": 82}
        None if the packet is malformed (missing/unknown keys, bad values,
        wrong number of fields, etc.) so the caller can safely skip it.
    """
    if not line:
        return None

    parts = line.split(";")
    if len(parts) != len(PACKET_FIELDS):
        # Wrong number of fields -> malformed packet (e.g. torn by UART noise)
        return None

    result = {}
    for part in parts:
        if "=" not in part:
            return None

        key, _, value_str = part.partition("=")
        key = key.strip()
        value_str = value_str.strip()

        if key not in PACKET_FIELDS:
            return None

        field_name, cast_func = PACKET_FIELDS[key]

        try:
            result[field_name] = cast_func(value_str)
        except ValueError:
            # e.g. "abc" where an int/float was expected
            return None

    # Make sure every expected field was actually present
    expected_field_names = {field_name for field_name, _ in PACKET_FIELDS.values()}
    if set(result.keys()) != expected_field_names:
        return None

    return result


def print_result(raw_line: str, parsed: dict):
    print("Received:")
    print(raw_line)
    print()
    print("Parsed:")
    print("{")
    for i, (key, value) in enumerate(parsed.items()):
        comma = "," if i < len(parsed) - 1 else ""
        print(f"    '{key}': {value!r}{comma}")
    print("}")
    print("-" * 50)


def main():
    print(f"Opening serial port {SERIAL_PORT} at {BAUD_RATE} baud...")

    try:
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            timeout=SERIAL_TIMEOUT_SEC,
        )
    except serial.SerialException as e:
        print(f"ERROR: Could not open serial port {SERIAL_PORT}.")
        print(f"Details: {e}")
        print("Check that:")
        print("  - The STM32 / USB-TTL adapter is plugged in")
        print("  - SERIAL_PORT matches the port shown in Device Manager")
        print("  - No other program (PuTTY, Arduino IDE, etc.) has the port open")
        return

    print(f"Connected. Listening on {SERIAL_PORT} (Ctrl+C to stop)...\n")

    try:
        while True:
            try:
                raw_bytes = ser.readline()
            except serial.SerialException as e:
                print(f"ERROR: Serial read failed (device disconnected?): {e}")
                break

            if not raw_bytes:
                # Timeout with no data received - just keep waiting
                continue

            try:
                line = raw_bytes.decode("utf-8", errors="replace").strip()
            except UnicodeDecodeError:
                print("WARNING: Received undecodable bytes, skipping packet.")
                continue

            if not line:
                continue

            parsed = parse_packet(line)

            if parsed is None:
                print(f"WARNING: Malformed packet ignored: {line!r}")
                print("-" * 50)
                continue

            print_result(line, parsed)

    except KeyboardInterrupt:
        print("\nStopping (Ctrl+C received)...")

    finally:
        if ser.is_open:
            ser.close()
        print("Serial port closed.")


if __name__ == "__main__":
    main()