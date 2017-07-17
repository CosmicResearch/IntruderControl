import picamera
import RPi.GPIO as GPIO
import threading
import os.path
import ConfigParser

camera = None

class CamPoller(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)
    config = ConfigParser.ConfigParser()
    config.read("/home/pi/IntruderControl/src/rpi_cam_gps/config.ini")
    led = int(config.get("LED", "ledCam"))
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(led,GPIO.OUT)
    GPIO.output(led,GPIO.HIGH)
    global camera
    camera=picamera.PiCamera(resolution=(1640, 1232),framerate=30)
    self.current_value = None
    self.running = True
    
  def run(self):
	global camera
	for i in range (1,100):
	  if not os.path.exists('/home/pi/Videos/%d.h264' % i):
		break
	  i = i + 1
	GPIO.output(18,GPIO.LOW)
	camera.start_recording('/home/pi/Videos/%d.h264' % i)
	try:
	  camera.wait_recording(30)
	except PiCameraError:
	  GPIO.output(18,GPIO.HIGH)
	while self.running:
	  i=i+1
	  camera.split_recording('/home/pi/Videos/%d.h264' % i)
	  try:
		camera.wait_recording(30)
	  except PiCameraError:
		GPIO.output(18,GPIO.HIGH)
	camera.stop_recording()
