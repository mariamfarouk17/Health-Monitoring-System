import asyncio
import websockets

async def test():
    uri = "ws://localhost:8765"
    async with websockets.connect(uri) as ws:
        print("[Test Client] Connected to WebSocket server!")
        while True:
            message = await ws.recv()
            print("[Received from backend]:", message)

if __name__ == "__main__":
    try:
        asyncio.run(test())
    except KeyboardInterrupt:
        print("\n[Test Client] Stopped.")