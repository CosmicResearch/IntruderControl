"""
    Guidance Computer Software
    Copyright (C) 2016 Associacio Cosmic Research

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

from Processes import CamPoller
from time import *
import time
import os
from gps import *
import threading
from datetime import datetime

gpsd = None

class GpsPoller(threading.Thread):
  def __init__(self):
    threading.Thread.__init__(self)
    global gpsd
    gpsd = gps(mode=WATCH_ENABLE)
    self.current_value = None
    self.running = True
 
  def run(self):
    global gpsd
    while gpsp.running:
      gpsd.next()

if __name__ == '__main__':
  global cam
  gpsfile = open("/home/pi/GPS_log.csv", "a")
  if os.stat ("/home/pi/GPS_log.csv").st_size == 0:
    gpsfile.write("Rpi Time,Time,Latitude,Longitude,Altitude,Speed,Climb,Track,Mode\n")
  gpsfile.write("-,-,-,-,-,-,-,-,-\n")
  cam = CamPoller()
  gpsp = GpsPoller()
  try:
    cam.start()
    gpsp.start()
    while True:
      os.system('clear')
      now = datetime.now()
      gpsfile.write(str(now)+","+str(gpsd.fix.time)+","+str(gpsd.fix.latitude)+","+str(gpsd.fix.longitude)+","+str(gpsd.fix.altitude)+","+str(gpsd.fix.speed)+","+str(gpsd.fix.climb)+","+str(gpsd.fix.track)+","+str(gpsd.fix.mode)+"\n")
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
 
      time.sleep(0.05)
 
  except (KeyboardInterrupt, SystemExit): #when you press ctrl+c
    print "\nKilling Thread..."
    cam.running = False
    gpsp.running = False
    cam.join()
    gpsp.join()
  gpsfile.close()
  print "Done.\nExiting."
