

#include <SD.h>
#include <SPI.h>
#include <Imu.h>

File GPSdata, IMUdata, Eventdata;
float roll, pitch, heading, altitude, temperature;
Imu imu;
void setup() {
     Serial.begin(9600);
     SD.begin(10);
     GPSdata=SD.open("GPS_DATA.txt", FILE_WRITE);
     IMUdata=SD.open("IMU_DATA.txt", FILE_WRITE);
     Eventdata=SD.open("EVENT_DATA.txt", FILE_WRITE);
     pinMode(10, OUTPUT);

     if(!imu.initSensors()){Serial.write("h");}
}

void loop() {
  
  if (!imu.getOrientation(roll, pitch)){Serial.write("h2");}
  if(!imu.getHeading(heading)){Serial.write("h3");}
  if(!imu.getAltitudeAndTemperature(altitude, temperature)){Serial.write("h4");}
  
  if(SD.exists("IMU_DATA.txt")){
    IMUdata.println("Roll: " + (String) roll+ ", Pitch: " +  (String) pitch + ", Head: " + (String) heading + ", Alt: " + (String) altitude + ", Temp: " + (String) temperature);
    IMUdata.flush();
  }
  else{Serial.write("hello");}


}
