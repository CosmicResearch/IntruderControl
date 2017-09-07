#include <SD.h>
#include <SPI.h>
#include <Imu.h>

#define CHIP_SELECT 10
#define LED_PIN_ERROR 3
File IMUdata, Eventdata;

float roll, pitch, heading, altitude, temperature, offsetAltitude, oldAltitude;
Imu imu;
int i;

void setup() {

     /* Descomentar para debuggar
     while(!Serial);
     Serial.begin(115200);
     Serial.println("Initializing...");
     */
     pinMode(CHIP_SELECT, OUTPUT);
     pinMode(LED_PIN_ERROR, OUTPUT);
     if(!SD.begin(CHIP_SELECT)){
      while(1){
          digitalWrite(LED_PIN_ERROR, HIGH);
          delay(500);
          digitalWrite(LED_PIN_ERROR, LOW);
          delay(500);
        }
     }
   
     Eventdata=SD.open("EV_DATA.txt", FILE_WRITE);
     if(SD.exists("IMU_DATA.csv")){
          IMUdata=SD.open("IMU_DATA.csv", FILE_WRITE);
          IMU_data.println("- - - - - - -");
     }
     else{
          IMUdata=SD.open("IMU_DATA.csv", FILE_WRITE);
     }
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
     IMUdata.println("Time Roll Pitch Heading Altitude Temperature Apogee");

     i=0;
     while(i<10){
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
        offsetAltitude+=altitude;
        i++; 
     }
     offsetAltitude=(offsetAltitude/i);
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
   if((altitude-offsetAltitude)>oldAltitude){
      apogee=altitude-offsetAltitude;
   }
  IMUdata.print(millis());
  IMUdata.println(" "+ (String) roll+ " " +  (String) pitch + " " + (String) heading + " " + (String) (altitude-offsetAltitude) + " " + (String) temperature + " "+ (String));
  IMUdata.flush();
    
}

