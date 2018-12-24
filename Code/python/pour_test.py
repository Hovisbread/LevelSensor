import time
import smbus
import RPi.GPIO as GPIO


# Pin definitions
sol1 = 12

# Suppress warnings
#GPIO.setwarnings(False)

# Use "GPIO" pin numbering
GPIO.setmode(GPIO.BCM)

#Only bus 1 exists for Raspberry Pi B+
i2c_ch = 1

# sensor address on the I2C bus
i2c_address = 0x38

# Set LED pin as output
GPIO.setup(sol1, GPIO.OUT)
#x=1
# Blink forever

bus = smbus.SMBus(1)

def measure():
	data = bus.read_i2c_block_data(i2c_address,0x00,4)
	output = int(data[0]<<8 | data[1])
	pressure = ((float(output)-1638)*60)/13107
	temp_bin = int((data[2]<<8 | data[3])>>5)
	temp = ((((float(temp_bin)/2047)*200)-50)*9/5)+32
	print("Pressure: ", pressure)
	print("Temperature: ", temp)



'''
while x<10:
    GPIO.output(sol1, GPIO.HIGH) # Turn LED on
    time.sleep(0.2)                   # Delay for 1 second
    GPIO.output(sol1, GPIO.LOW)  # Turn LED off
    time.sleep(1)
    x = x+1
'''
measure()

GPIO.output(sol1, GPIO.HIGH) # Turn LED on
time.sleep(30)                   # Delay for 1 second
GPIO.output(sol1, GPIO.LOW)  # Turn LED off
measure()
GPIO.cleanup()                # Delay for 1 second
