#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

class Imu {
	private:
		//Esto se puede optimizar
		Adafruit_10DOF                dof;
		Adafruit_LSM303_Accel_Unified accel;
		Adafruit_LSM303_Mag_Unified   mag;
		Adafruit_BMP085_Unified       bmp;

	public:

		//Post: inicializa y comprueba el cableado
		bool initSensors();

		//Pre: roll y pitch tienen data irrelevante
		//Post: si getOrientation devuelve cierto, roll y pitch 
		//		tienen los valores del acceletometro
		bool getOrientation(float& roll, float& pitch);

		//Pre: heading tiene data irrelevante
		//Post: si getHeading devuelve cierto heading tiene el valor del Magnetometro
		bool getHeading(float& heading);

		//Pre: altitude y temperature tienen data irrelevante
		//Post: si getAltitudeAndTemperature devuelve cierto,
		// 		altitude y temperatura tendran los valores correctos
		bool getAltitudeAndTemperature(float& altitude, float& temperature);
};