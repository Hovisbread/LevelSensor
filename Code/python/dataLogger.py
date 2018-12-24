from __future__ import print_function
from random import randint
import time
import serial
import datetime
import sys

#adjust params for current setup
com   	   = "COM4"
fName 	   = "KegData"
pourVolume = "500"
temp	   = "20"
reset	   = "20"

#open serial connection
ser = serial.Serial(com, 9600, timeout=.1) #########

#Print pretty things
print ("")
print ("*************************************")
print ("*    Welcome to Data Logger V1.0    *")
print ("*      Keg All The Things!          *")
print ("*************************************")
print ("")

#get user input for params or allow default values
print ("Enter new values or press enter for [default value]")
print ("")
com 	    = input("Arduino COM Port [" + str(com) + "]: ") or com
fName 	    = input("Ouput Filename [" + str(fName) + "]: ") or fName
pourVolume  = input("Pour Mass (g) [" + str(pourVolume) + "]: ") or pourVolume
temp 		= input("Water Temp (C) [" + str(temp) + "]: ") or temp
reset		= input("Pressure Reset Time (s) [" + str(reset) + "]: ") or reset
testComment = input("Test Description/Comment: ")

#open file and catch errors
ts = time.time()
st = datetime.datetime.fromtimestamp(ts).strftime('%Y%m%d_%H%M%S')
fName = fName + "_" + str(st) + ".csv"

fileOpen = False
while fileOpen != True:
	try:
		f = open(fName, "w")
		fileOpen = True
	except IOError:
		print ("Error: Unable to open file", fName)
		print ("File may already be open.  Please close and try again.")
		retry = raw_input("Try Again? Y/N: ")
		print ("")
		if retry == "N" or retry == "n":
			sys.exit()
	
#add time stamp and titles
st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
print ("Keg Data Collection,", st, file = f)
print ("\nPour Mass(g):,", pourVolume, file=f)
print ("Temp(C):,", temp, file=f)
if testComment:
	print ("Comment:,", testComment, file=f)
print ("", file=f)
print ("Test #, Pinitial (kPa), Pfinal (kPa), Stabilization Time (s)", file=f)
f.close()

#initialize variables
pInit     = 0
pFinal    = 0
timeCount = 0
stabTime  = 0
testCount = 0

#print Title
print ("")
print("Pressure")
tempPressure = 0
while True:
	try:
		#read in one line from Serial
		data = ser.readline().decode().strip()
		#data = randint(10, 30)
		
		#if data is populated output to console
		if len(data) > 1:
			tempStr = "  " + str(data) #space added for fomatting
			#If between tests output stabTimer
			if pInit == 0 and testCount != 0:
				tempStr += "           " + str(int(reset) - timeCount) 
				#timer has expired
				if timeCount > int(reset):
					tempStr += " Begin Test!"
			print (tempStr) 
			timeCount += 1	
			tempPressure = data
		#wait one second and increment
		#time.sleep(1)
		#timeCount += 1
		
	#handle ctrl+C keypress for data logging
	except KeyboardInterrupt:
		#first interrupt collect Pressure Initial
		if pInit == 0:
			print("")
			print ("*************************************")
			print ("Begin Test ", testCount, "  Pinit:", tempPressure)
			print ("Pressure")
			pInit = tempPressure #some subset of data
			stabTime = timeCount
		else:
			#Second Interrupt collect Pressure Final and output both
			print("PFinal: ", tempPressure)
			print ("*************************************")
			print("")
			f = open(fName, "a")
			pFinal = tempPressure #some subset of data
			print (testCount, ", ", pInit, ",", pFinal, ", ", stabTime, file=f)
			f.close()
			
			#reset/increment variables
			testCount += 1
			pInit     = 0
			pFinal    = 0
			timeCount = 0
			stabTime  = 0
			print("Pressure    Stab Time")
#close Serial		
ser.close()