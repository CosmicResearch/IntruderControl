import os
from gps import *
from time import *
import time
import threading
from datetime import datetime
 
gpsd = None #seting the global variable
 
class GpsPoller(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)
    global gpsd #bring it in scope
    gpsd = gps(mode=WATCH_ENABLE) #starting the stream of info
    self.current_value = None
    self.running = True #setting the thread running to true
 
  def run(self):
    global gpsd
    while gpsp.running:
      gpsd.next() #this will continue to loop and grab EACH set of gpsd info to clear the buffer
 
if __name__ == '__main__':
  gpsfile = open("/home/pi/GPS_log.csv", "a")
  if os.stat ("/home/pi/GPS_log.csv").st_size == 0:
    gpsfile.write("Rpi Time,Time,Latitude,Longitude,Altitude,Speed,Climb,Track,Mode,Satellites\n")
  gpsfile.write("-,-,-,-,-,-,-,-,-,-\n")
  gpsp = GpsPoller() # create the thread
  try:
    gpsp.start() # start it up
    while True:
      os.system('clear')
      now = datetime.now()
      gpsfile.write(str(now)+","+str(gpsd.fix.time)+","+str(gpsd.fix.latitude)+","+str(gpsd.fix.longitude)+","+str(gpsd.fix.altitude)+","+str(gpsd.fix.speed)+","+str(gpsd.fix.climb)+","+str(gpsd.fix.track)+","+str(gpsd.fix.mode)+","+str(gpsd.satellites)+"\n")
      gpsfile.flush()

      print
      print ' GPS reading'
      print '----------------------------------------'
      print 'latitude    ' , gpsd.fix.latitude
      print 'longitude   ' , gpsd.fix.longitude
      print 'time utc    ' , gpsd.utc,' + ', gpsd.fix.time
      print 'altitude (m)' , gpsd.fix.altitude
      print 'eps         ' , gpsd.fix.eps
      print 'epx         ' , gpsd.fix.epx
      print 'epv         ' , gpsd.fix.epv
      print 'ept         ' , gpsd.fix.ept
      print 'speed (m/s) ' , gpsd.fix.speed
      print 'climb       ' , gpsd.fix.climb
      print 'track       ' , gpsd.fix.track
      print 'mode        ' , gpsd.fix.mode
      print
      print 'sats        ' , gpsd.satellites
      print now
 
      time.sleep(1)
 
  except (KeyboardInterrupt, SystemExit): #when you press ctrl+c
    print "\nKilling Thread..."
    gpsp.running = False
    gpsp.join() # wait for the thread to finish what it's doing
  gpsfile.close()
  print "Done.\nExiting."
