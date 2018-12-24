#Sending the data thorugh UART
#UART PINOUT
#Rx -> GPIO15
#Tx -> GPIO14

#import RPi.GPIO as GPIO
import serial
import struct

def parseTime(datas):
    time, = struct.unpack_from('<I', datas, 0)
    #print("Time: ", time)
    return time

def parseStatus(datas):
    status, = struct.unpack_from('>B', datas, 4)
    status = (status & 0xC0) >> 6
    #print("Status: ", status)
    return status

def parsePresure(datas):
    counts, = struct.unpack_from('>H', datas, 4)
    counts = counts & 0x3FFF
    max_counts = 14745
    min_counts = 1638
    max_pressure = 5
    min_pressure = -5
    pressure = ((counts-min_counts) * (max_pressure-min_pressure) / (max_counts-min_counts)) + min_pressure
    #print("Pressure: ", pressure)
    return pressure

#Open named port
ser = serial.Serial ("/dev/ttyAMA0")
#Set baud rate to 115200
ser.baudrate = 115200
ser.reset_input_buffer()
while True:
    if ser.in_waiting > 0:
        datas = ser.read(9)
        time = parseTime(datas)
        pressure = parsePresure(datas)
        status = parseStatus(datas)
        print('Time: {}, Pressure: {}, Status {}'.format(time, pressure, status))

        #print("original data: ", end="")
        #print(datas)
