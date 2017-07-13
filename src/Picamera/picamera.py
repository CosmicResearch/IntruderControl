import picamera
import RPi.GPIO as GPIO
import sys

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(18,GPIO.OUT) # No tiene por que ser pin 18

camera = picamera.PiCamera(resolution=(1640, 1232), framerate=30)
camera.start_recording('1.h264')
try:
    camera.wait_recording(30)
except PiCameraError:
    GPIO.output(18,GPIO.HIGH)
    sys.exit()

for i in range(2, 100):
    camera.split_recording('%d.h264' % i)
    try:
    	camera.wait_recording(30)
    except PiCameraError:
        GPIO.output(18,GPIO.HIGH)
        sys.exit()
    camera.stop_recording()
