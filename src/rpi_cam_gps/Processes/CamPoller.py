import picamera
import RPi.GPIO as GPIO
import threading
import os.path

camera = None

class CamPoller(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(18,GPIO.OUT)
    global camera
    camera=picamera.PiCamera(resolution=(1640, 1232),framerate=30)
    self.current_value = None
    self.running = True #setting the thread running to true
    
  def run(self):
	global camera
	for i in range (1,100):
	  if not os.path.exists('/home/pi/Videos/%d.h264' % i):
		break
	  i = i + 1
	camera.start_recording('/home/pi/Videos/%d.h264' % i)
	try:
	  camera.wait_recording(5)
	except PiCameraError:
	  GPIO.output(18,GPIO.HIGH)
	while self.running:
	  i=i+1
	  camera.split_recording('/home/pi/Videos/%d.h264' % i)
	  try:
		camera.wait_recording(5)
	  except PiCameraError:
		GPIO.output(18,GPIO.HIGH)
	camera.stop_recording()
