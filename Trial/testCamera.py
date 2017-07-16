import picamera


with picamera.PiCamera() as camera:
	camera.resolution=(1640, 1232)
	camera.framerate=30
	camera.start_recording('/home/pi/Desktop/1.h264')
	camera.wait_recording(10)
	
	for i in range(2, 5):
    		camera.split_recording('/home/pi/Desktop/%d.h264' % i)
    		camera.wait_recording(10)
	camera.stop_recording()
