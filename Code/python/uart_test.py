#Sending the data thorugh UART
#UART PINOUT
#Rx -> GPIO15
#Tx -> GPIO14

# get the GPIO Library
import RPi.GPIO as GPIO
import serial
import time

#Open named port
ser = serial.Serial ("/dev/ttyAMA0", timeout = 4)

# the input buttons
up = 7
down = 8

#Setting up the GPIO Pins
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.cleanup()

#Setting up the Button Inputs
GPIO.setup(up, GPIO.IN)
GPIO.setup(down, GPIO.IN)

#Set baud rate to 9600
ser.baudrate = 9600
'''
message = bytes("hello")
x = 0
while x<10:
    #Handling the Button Debounce
    ser.write(message)
    x = x + 1
    time.sleep(1)
'''
ser.flush()
datas = ser.read(10)
print(bytes.decode(datas))
#Close the Serial Connection
ser.close()

#End of the Script
