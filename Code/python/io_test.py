import time
import RPi.GPIO as GPIO

# Pin definitions
sol1 = 12
sol2 = 16

# Suppress warnings
GPIO.setwarnings(False)

# Use "GPIO" pin numbering
GPIO.setmode(GPIO.BCM)

# Set LED pin as output
GPIO.setup(sol1, GPIO.OUT)
x=1
# Blink forever
while x<10:
    GPIO.output(sol1, GPIO.HIGH) # Turn LED on
    time.sleep(0.2)                   # Delay for 1 second
    GPIO.output(sol1, GPIO.LOW)  # Turn LED off
    time.sleep(1)
    x = x+1

GPIO.cleanup()                # Delay for 1 second
