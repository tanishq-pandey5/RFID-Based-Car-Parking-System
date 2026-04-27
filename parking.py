import serial
import csv
import os
from datetime import datetime

ser = serial.Serial('/dev/cu.usbserial-110', 9600)

file_exists = os.path.isfile("parking_log.csv")

file = open("parking_log.csv", "a", newline="")
writer = csv.writer(file)

if not file_exists:
    writer.writerow(["Slot", "UID", "Entry Time", "Exit Time", "Duration"])

active_entries = {}

print("Logging Started...")

while True:
    try:
        line = ser.readline().decode().strip()
        print("Received:", line)

        data = line.split(",")

        # ---------- ENTRY ----------
        if data[0] == "ENTRY":
            slot = data[1]
            uid = data[2]

            entry_time = datetime.now()

            active_entries[slot] = (uid, entry_time)

            
            writer.writerow([
                slot,
                uid,
                entry_time.strftime("%Y-%m-%d %H:%M:%S"),
                "", ""
            ])
            file.flush()

        # ---------- EXIT ----------
        elif data[0] == "EXIT":
            slot = data[1]

            if slot in active_entries:
                uid, entry_time = active_entries[slot]
                exit_time = datetime.now()

                duration_sec = (exit_time - entry_time).seconds
                minutes = duration_sec // 60
                seconds = duration_sec % 60

                duration = f"{minutes} min {seconds} sec"

               
                writer.writerow([
                    slot,
                    uid,
                    entry_time.strftime("%Y-%m-%d %H:%M:%S"),
                    exit_time.strftime("%Y-%m-%d %H:%M:%S"),
                    duration
                ])
                file.flush()

                del active_entries[slot]

    except Exception as e:
        print("Error:", e)
