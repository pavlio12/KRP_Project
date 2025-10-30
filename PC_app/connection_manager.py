import serial
import time
import threading


# Connection manager
# - tries to open the port and starts a receiver for active connection.
class ConnectionManager(threading.Thread):
    def __init__(self, port, baud, on_status_change=None, on_receive=None):
        super().__init__(daemon=True)
        self.port = port
        self.baud = baud
        self._lock = threading.Lock()
        self._ser = None
        self._stop_event = threading.Event()
        # Callbacks for UI updates
        self._on_status_change = on_status_change or (lambda x: None)
        self._on_receive = on_receive or (lambda x: None)


    def get_ser(self):
        with self._lock:
            return self._ser

    def set_ser(self, ser):
        with self._lock:
            self._ser = ser

    def close_ser(self):
        with self._lock:
            if self._ser is not None:
                try:
                    self._ser.close()
                except Exception as e:
                    self._on_status_change(f"Error closing serial port: {e}")
                finally:
                    self._ser = None

    def stop(self):
        self._stop_event.set()
        self.close_ser()
        self.join()  # Wait for the thread to terminate

    def run(self):
        while not self._stop_event.is_set():
            if self.get_ser() is None:
                try:
                    ser = serial.Serial(self.port, self.baud, timeout=1)
                    self.set_ser(ser)
                    self._on_status_change(f'Connected to {self.port}')
                    # start a receiver thread tied to this serial instance
                    threading.Thread(target=self._receiver_for, args=(ser,), daemon=True).start()
                except Exception as e:
                    self._on_status_change(f'Disconnected - retrying ({e})')
                    time.sleep(2)
            time.sleep(0.5)

    def _receiver_for(self, ser_instance):
        # receive until the serial is closed or replaced
        try:
            while True:
                # if the active serial changed, stop this receiver
                if self.get_ser() is not ser_instance:
                    break
                line = ser_instance.readline().decode(errors='ignore').strip()
                if line:
                    self._on_receive(line)
        except Exception as e:
            self._on_receive(f"[RX error] {e}")
        finally:
            # cleanup if this was the active serial
            if self.get_ser() is ser_instance:
                self.close_ser()
                self._on_status_change('Disconnected - attempting reconnect')

