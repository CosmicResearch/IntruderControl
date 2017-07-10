#include "Imu.h"


bool Imu::initSensors()
{
  dof   = Adafruit_10DOF();
  accel = Adafruit_LSM303_Accel_Unified(30301);
  mag   = Adafruit_LSM303_Mag_Unified(30302);
  bmp   = Adafruit_BMP085_Unified(18001);

  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    return false;
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    return false;
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    //Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    return false;
  }
  return true;
}

bool Imu::getOrientation(float& roll, float& pitch){
  sensors_event_t accel_event;
  sensors_vec_t   orientation;

  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation)){
    roll = orientation.roll;
    pitch = orientation.pitch;
    return true;
  }
  //Serial.println("Error in communication with Acceletometer (getOrientation(float& roll, float& pitch))");
  return false;
}

bool Imu::getHeading(float& heading){
  sensors_event_t mag_event;
  sensors_vec_t   orientation;

  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    heading = orientation.heading;
    return true;
  }
  //Serial.println("Error in communication with Magnetometer (getHeading(float& heading))");
  return false;
}

bool Imu::getAltitudeAndTemperature(float& altitude, float& temperature){
  sensors_event_t bmp_event;
  float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    /* Get ambient temperature in C */
    float t;
    bmp.getTemperature(&t);
    /* Convert atmospheric pressure, SLP and temp to altitude    */
    altitude = bmp.pressureToAltitude(seaLevelPressure, bmp_event.pressure,
                                        t); 
    temperature = t;
    return true;
  }
  //Serial.println("Error in communication with Barometer (getAltitudeAndTemperature)");
  return false;
}
