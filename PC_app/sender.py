import time
from queue import Queue, Empty

def sender_loop(conn_mgr, message_queue, on_tx=None):
    on_tx = on_tx or (lambda msg: None)
    while True:
        try:
            message = message_queue.get(timeout=0.5)
        except Empty:
            if getattr(conn_mgr, "_stop_event").is_set():
                break
            continue

        ser = conn_mgr.get_ser()
        if ser is None:
            message_queue.put(message)
            on_tx(f"[TX] Device disconnected, message queued: {message}")
            time.sleep(1)
            continue

        try:
            ser.write(f"{message}\n".encode())
            on_tx(f"[TX] {message}")

        except Exception as e:
            conn_mgr.close_ser()
            message_queue.put(message)
            on_tx(f"[TX error] {e} -- message requeued")
            time.sleep(1)