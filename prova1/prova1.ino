

#include <SD.h>
#include <SPI.h>
#include <Imu.h>
/*
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
*/

File GPSdata, IMUdata, Eventdata;

float roll, pitch, heading, altitude, temperature;
Imu imu;

/*
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true
boolean usingInterrupt = false;
*/

void setup() {
     Serial.begin(115200);
     Serial.println("Initializing...");
     
     SD.begin(10);
     GPSdata=SD.open("GPS_DATA.txt", FILE_WRITE);
     Eventdata=SD.open("EV_DATA.txt", FILE_WRITE);
     IMUdata=SD.open("IMU_DATA.txt", FILE_WRITE);
     pinMode(10, OUTPUT);
     Serial.println("Fitxers creats");
     
     if(!imu.initSensors()){Serial.write("h");}
     Serial.println("IMU inicializada");

     /*
     GPS.begin(9600);
     GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
     GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
     GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
     useInterrupt(true);
     Serial.println("GPS inicializado");
     */
     delay(1000);
}

/*
// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();

  if (GPSECHO)
    if (c) UDR0 = c;  
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
*/

void loop() {
  
  if(!imu.getOrientation(roll, pitch)){Serial.write("h2");}
  if(!imu.getHeading(heading)){Serial.write("h3");}
  if(!imu.getAltitudeAndTemperature(altitude, temperature)){Serial.write("h4");}
  
  if(SD.exists("IMU_DATA.txt")){
    IMUdata.println("Roll: " + (String) roll+ ", Pitch: " +  (String) pitch + ", Head: " + (String) heading + ", Alt: " + (String) altitude + ", Temp: " + (String) temperature);
    IMUdata.flush();
  }
  
  if(SD.exists("EV_DATA.txt")){
  }
  
}

