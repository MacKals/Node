
import serial

ser = serial.Serial('/dev/cu.usbserial-1410')

# Check for Teensy output:
try:
    while True:
        bytesToRead = ser.inWaiting() # get the amount of bytes available at the input queue
        if bytesToRead:
            line = ser.readline(bytesToRead) # read the byte
            print(line.strip())

except:
    ser.close()
