import os, sys
import serial

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(BASE_DIR)
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'btl.settings')

import django
django.setup()

from app.models import SensorReading, RawUART  
from django.utils import timezone


SERIAL_PORT = 'COM13'
BAUDRATE = 9600

def parse_kit_line(line):
   
    try:
        parts = line.strip().split()
        data = {}

        for p in parts:
            if '=' in p:
                key, val = p.split('=', 1)

                # loại bỏ đơn vị: ppm, mg/m3, Pa, C, m
                val = val.replace("ppm", "").replace("mg/m3", "")
                val = val.replace("Pa", "").replace("C", "").replace("m", "")
                val = val.strip()

                # convert float
                try:
                    val = float(val)
                except:
                    pass

                data[key.lower()] = val

        # map real keys
        sensor1 = (data.get('temp'),data.get('press'),data.get('alt'))

        sensor2 = (data.get('iaq'),data.get('tvoc'),data.get('eco2'),data.get('etoh'))

        return sensor1, sensor2
    
    except Exception as e:
        print("Parse error:", e, "| Line:", line)
        return (None, None), (None, None)

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
        print(f"Serial port opened: {SERIAL_PORT} at {BAUDRATE}")
    except Exception as e:
        print("Cannot open serial port:", SERIAL_PORT, e)
        return

    print("Đang đọc UART...")

    latest_sensor1 = None   # (temp, press, alt)
    latest_sensor2 = None   # (IAQ, TVOC, eCO2, EtOH)

    while True:
        try:
            line = ser.readline().decode(errors='ignore').strip()
            if not line:
                continue

            print("Raw line:", line)

            # Lưu raw log
            try:
                RawUART.objects.create(raw=line, timestamp=timezone.now())
            except Exception as e:
                print("Failed to save RawUART:", e)

            sensor1, sensor2 = parse_kit_line(line)

            updated = False

            # SENSOR1
            if sensor1[0] is not None and sensor1[1] is not None and sensor1[2] is not None:
                latest_sensor1 = sensor1
                updated = True
                print("Sensor1 data: ", latest_sensor1)

            # SENSOR2
            if sensor2[0] is not None and sensor2[1] is not None and sensor2[2] is not None and sensor2[3] is not None:
                latest_sensor2 = sensor2
                updated = True
                print("Sensor2 data: ", latest_sensor2)

            # When both sensors have data , save to databasse
            if updated and latest_sensor1 and latest_sensor2:
                SensorReading.objects.create(
                    temp = latest_sensor1[0],
                    press = latest_sensor1[1],
                    alt = latest_sensor1[2],
                    IAQ = latest_sensor2[0],
                    TVOC = latest_sensor2[1],
                    eCO2 = latest_sensor2[2],
                    EtOH = latest_sensor2[3]
                )

                print("Data saved: ", latest_sensor1, latest_sensor2)

        except Exception as e:
            print("Error reading/parsing:", e)


if __name__ == "__main__":
    main()
