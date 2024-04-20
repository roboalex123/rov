import serial
import time

arduino_port = "COM5"
baud_rate = 9600

ser = serial.Serial(arduino_port, baud_rate)
print("started serial")
time.sleep(10)
print("timer done")

print("motor running")
ser.write(b'1100')
time.sleep(1.15)
ser.write(b'1200')
time.sleep(1.15)
ser.write(b'1300')
time.sleep(2)
ser.write(b'1500')

print("motor stop")
time.sleep(3)
ser.write(b'TEST')
ser.close()