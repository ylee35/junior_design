import asyncio
import websockets

# Store client info as: { websocket: client_id }
clients = {}

async def handler(websocket):
    try:
        # --- First message from client is the client ID ---
        client_id = await websocket.recv()
        clients[websocket] = client_id
        print(f"Client connected: {client_id}")

        # --- Listen for messages from this client ---
        async for message in websocket:
            print(f"Received from {client_id}: {message}")

            # Broadcast "<client_id>: <message>" to all clients
            broadcast_message = f"{client_id}.{message}"

            await asyncio.gather(
                *[client.send(broadcast_message) for client in clients.keys() if client != websocket]
            )

    except websockets.ConnectionClosed:
        if websocket in clients:
            print(f"Client disconnected: {clients[websocket]}")
            del clients[websocket]


async def main():
    async with websockets.serve(handler, "0.0.0.0", 8765):
        print("Server running on ws://0.0.0.0:8765")
        await asyncio.Future()  # run forever

asyncio.run(main())

