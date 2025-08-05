import tkinter as tk
from tkinter import ttk
import serial

# Setup serial communication
SERIAL_PORT = 'COM9'  # Change to your port, e.g., 'COM4' or '/dev/ttyUSB0'
BAUD_RATE = 115200  # Adjust as needed

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except serial.SerialException:
    ser = None
    print("Could not open serial port")

    def on_key(event):
        if event.keysym == 'Up':
            lift_action(1)      # Lift up
        elif event.keysym == 'Down':
            lift_action(0)      # Lower
        elif event.keysym == 'Left':
            grip_action(1)      # Grip
        elif event.keysym == 'Right':
            grip_action(0)      # Release

    root.bind('<Up>', on_key)
    root.bind('<Down>', on_key)
    root.bind('<Left>', on_key)
    root.bind('<Right>', on_key)

def send_command(cmd):
    if ser:
        print("Sending:", cmd)
        ser.write((cmd + '\n').encode())

def grip_action(state):
    send_command(f"grip={state}")

def lift_action(state):
    send_command(f"lift={state}")

def vacuum_action(state):
    send_command(f"vacu={state}")

def fork_send():
    value = fork_entry.get()
    try:
        val = int(value)
        send_command(f"fork={val}")
    except ValueError:
        print("Invalid fork value")

# GUI Setup
root = tk.Tk()
root.title("End Effector Control")

# Gripper Controls
ttk.Label(root, text="Top Gripper").grid(row=0, column=0, pady=5)
ttk.Button(root, text="Grip", command=lambda: grip_action(1)).grid(row=1, column=0)
ttk.Button(root, text="Release", command=lambda: grip_action(0)).grid(row=1, column=1)
ttk.Button(root, text="Disable", command=lambda: grip_action(2)).grid(row=1, column=2)

# Lifter Controls
ttk.Label(root, text="Lifter").grid(row=2, column=0, pady=5)
ttk.Button(root, text="Lift", command=lambda: lift_action(1)).grid(row=3, column=0)
ttk.Button(root, text="Lower", command=lambda: lift_action(0)).grid(row=3, column=1)
ttk.Button(root, text="Disable", command=lambda: lift_action(2)).grid(row=3, column=2)

# Vacuum Controls
ttk.Label(root, text="Vacuum").grid(row=4, column=0, pady=5)
ttk.Button(root, text="Enable", command=lambda: vacuum_action(1)).grid(row=5, column=0)
ttk.Button(root, text="Disable", command=lambda: vacuum_action(0)).grid(row=5, column=1)

# Fork Control
ttk.Label(root, text="Fork (cm)").grid(row=6, column=0, pady=5)
fork_entry = ttk.Entry(root)
fork_entry.grid(row=6, column=1)
ttk.Button(root, text="Send", command=fork_send).grid(row=6, column=2)

root.mainloop()

# Close serial when done
if ser:
    ser.close()
