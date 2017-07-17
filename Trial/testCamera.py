import picamera


with picamera.PiCamera() as camera:
	camera.resolution=(1640, 1232)
	camera.framerate=30
	camera.start_recording('/home/pi/Desktop/1.h264')
	camera.wait_recording(5)
	print 'Video 1 of 5 saved to Desktop'
	
	for i in range(2, 6):
    		camera.split_recording('/home/pi/Desktop/%d.h264' % i)
    		camera.wait_recording(5)
		print 'Video '+str(i)+' of 5 saved to Desktop'
	print
	print 'All 5 videos saved to Desktop, test complete correctly :)'
	camera.stop_recording()
