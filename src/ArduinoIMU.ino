

#include <SD.h>
#include <SPI.h>
#include <Imu.h>

#define CHIP_SELECT 10
#define LED_PIN_ERROR 3
#define LED_PIN_OK 4
File IMUdata, Eventdata;

float roll, pitch, heading, altitude, temperature;
Imu imu;


void setup() {

     /* Descomentar para debuggar
     while(!Serial);
     Serial.begin(115200);
     Serial.println("Initializing...");
     */
     pinMode(LED_PIN, OUTPUT);
     SD.begin(CHIP_SELECT);
   
     Eventdata=SD.open("EV_DATA.txt", FILE_WRITE);
     IMUdata=SD.open("IMU_DATA.txt", FILE_WRITE);
     pinMode(CHIP_SELECT, OUTPUT);
     
     //Serial.println("Fitxers creats");
     
     Eventdata.print(millis());
     Eventdata.println("- Fitxers creats");
     Eventdata.flush();
     
     if(!imu.initSensors()){
      Eventdata.print(millis());
      Eventdata.println("- Hi ha hagut un problema inicialitzant els sensors");
      while(1){
          digitalWrite(LED_PIN_ERROR, HIGH);
          delay(500);
          digitalWrite(LED_PIN_ERROR, LOW);
          delay(500);
        }
     }
     //Serial.println("IMU inicializada");
     Eventdata.print(millis());
     Eventdata.println("- IMU inicializada");
     Eventdata.flush();
     delay(1000);
}


void loop() {
  
  if(!imu.getOrientation(roll, pitch)){
      //Serial.println("- Ha habido un problema obteniendo la orientacion")
      Eventdata.print(millis());
      Eventdata.println("- Ha habido un problema obteniendo la orientacion");
      while(1){
          digitalWrite(LED_PIN_ERROR, HIGH);
          delay(500);
          digitalWrite(LED_PIN_ERROR, LOW);
          delay(500);
        }
      }
  if(!imu.getHeading(heading)){
      //Serial.println("- Ha habido un problema obteniendo la direccion")
      Eventdata.print(millis());
      Eventdata.println("- Ha habido un problema obteniendo la direccion");
      while(1){
          digitalWrite(LED_PIN_ERROR, HIGH);
          delay(500);
          digitalWrite(LED_PIN_ERROR, LOW);
          delay(500);
        }
      }
  if(!imu.getAltitudeAndTemperature(altitude, temperature)){
      //Serial.println("- Ha habido un problema obteniendo la altura y la temperatura")
      Eventdata.print(millis());
      Eventdata.println("- Ha habido un problema obteniendo la altura y la temperatura");
      while(1){
          digitalWrite(LED_PIN_ERROR, HIGH);
          delay(500);
          digitalWrite(LED_PIN_ERROR, LOW);
          delay(500);
        }
      }

  IMUdata.print(millis());
  IMUdata.println("- Roll: " + (String) roll+ ", Pitch: " +  (String) pitch + ", Head: " + (String) heading + ", Alt: " + (String) altitude + ", Temp: " + (String) temperature);
  IMUdata.flush();
  
}

