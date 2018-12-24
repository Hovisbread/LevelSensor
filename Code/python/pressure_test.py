#pressure_test.py
import time
import smbus

#Only bus 1 exists for Raspberry Pi B+
i2c_ch = 1

# sensor address on the I2C bus
i2c_address = 0x38

bus = smbus.SMBus(1)
while True:
	data = bus.read_i2c_block_data(i2c_address,0x00,4)
	output = int(data[0]<<8 | data[1])
	pressure = ((float(output)-1638)*60)/13107
	temp_bin = int((data[2]<<8 | data[3])>>5)
	temp = ((((float(temp_bin)/2047)*200)-50)*9/5)+32
	print("Pressure: ", pressure)
	print("Temperature: ", temp)	
	time.sleep(1)
