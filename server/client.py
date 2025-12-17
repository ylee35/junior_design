import asyncio
import websockets

SERVER_URL = "ws://localhost:8765"

async def send_messages(ws):
    """Continuously read user input and send to the server."""
    loop = asyncio.get_running_loop()
    while True:
        # run input() in a thread so it doesn’t block asyncio
        msg = await loop.run_in_executor(None, input, "")
        await ws.send(msg)

async def receive_messages(ws):
    """Continuously receive and print messages from the server."""
    try:
        while True:
            message = await ws.recv()
            print("\n" + message)
    except websockets.ConnectionClosed:
        print("Disconnected from server.")

async def main():
    async with websockets.connect(SERVER_URL) as ws:
        # First message: client ID
        client_id = input("Enter your client ID: ")
        await ws.send(client_id)

        print(f"Connected as {client_id}. Type messages to send:")

        # Run send & receive concurrently
        await asyncio.gather(
            send_messages(ws),
            receive_messages(ws),
        )

asyncio.run(main())

