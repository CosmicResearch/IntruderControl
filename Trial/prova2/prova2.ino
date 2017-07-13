
#include <Adafruit_GPS.h>

#ifdef __AVR__
  #include <SoftwareSerial.h>
  #include <avr/sleep.h>
#endif

#include <SPI.h>
#include <SD.h>
//#include <Imu.h>



#ifdef __AVR__
SoftwareSerial mySerial(8, 7);
#else
#define mySerial Serial1
#endif

Adafruit_GPS GPS(&mySerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  true
/* set to true to only log to SD when GPS has a fix, for debugging, keep it false */
#define LOG_FIXONLY false  

// Set the pins used
#define chipSelect 10
#define ledPin 13

File logfile; //, imufile;

//float roll, pitch, heading, altitude, temperature;
//Imu imu;

// read a Hex value and return the decimal equivalent
uint8_t parseHex(char c) {
  if (c < '0')
    return 0;
  if (c <= '9')
    return c - '0';
  if (c < 'A')
    return 0;
  if (c <= 'F')
    return (c - 'A')+10;
}

// blink out an error code
void error(uint8_t errno) {
/*
  if (SD.errorCode()) {
    putstring("SD error: ");
    Serial.print(card.errorCode(), HEX);
    Serial.print(',');
    Serial.println(card.errorData(), HEX);
  }
  */
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

void setup() {
  // for Leonardos, if you want to debug SD issues, uncomment this line
  // to see serial output
  //while (!Serial);
  
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("\r\nUltimate GPSlogger Shield");
  pinMode(ledPin, OUTPUT);

  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  //if (!SD.begin(chipSelect, 11, 12, 13)) {
  if (!SD.begin(chipSelect)) {      // if you're using an UNO, you can use this line instead
    Serial.println("Card init. failed!");
    error(2);
  }
  
  char filename[13];
  strcpy(filename, "GPS000.TXT");
  for (uint8_t i = 0; i < 1000; i++) {
    filename[3] = '0' + i/100;
    filename[4] = '0' + (i-(i/100)*100)/10; //Revisar
    filename[5] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); Serial.println(filename);
  
  // connect to the GPS at the desired rate
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
  GPS.sendCommand(PGCMD_NOANTENNA);
  
/* IMU part
  memset(filename, '\0', sizeof(filename));
  strcpy(filename, "IMU000.TXT");
  for (uint8_t i = 0; i < 1000; i++) {
    filename[3] = '0' + i/100;
    filename[4] = '0' + (i-(i/100)*100)/10; //Revisar
    filename[5] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  imufile = SD.open(filename, FILE_WRITE);
  if( ! imufile ) {
    Serial.print("Couldnt create "); Serial.println(filename);
    error(3);
  }

  if(!imu.initSensors()){
    Serial.write("h");
  }
  */
  
  Serial.println("Ready!");
}

void loop() {
  char c = GPS.read();
  if (GPSECHO)
     if (c)   Serial.print(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
        
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
    
    // Sentence parsed! 
    Serial.println("OK");
    if (LOG_FIXONLY && !GPS.fix) {
        Serial.print("No Fix");
        return;
    }

    // Rad. lets log it!
    Serial.println("Log");
    
    char *stringptr = GPS.lastNMEA();
    uint8_t stringsize = strlen(stringptr);
    if (stringsize != logfile.write((uint8_t *)stringptr, stringsize))    //write the string to the SD file
      error(4);
    if (strstr(stringptr, "RMC"))   logfile.flush();
    Serial.println();
  }

  /* IMU part
  if(!imu.getOrientation(roll, pitch)){Serial.write("Err. orient");}
  if(!imu.getHeading(heading)){Serial.write("Err. head.");}
  if(!imu.getAltitudeAndTemperature(altitude, temperature)){Serial.write("Err. alt.");}
  
  if(SD.exists("IMU_DATA.txt")){
    imufile.println("Roll: " + (String) roll+ ", Pitch: " +  (String) pitch);// + ", Head: " + (String) heading + ", Alt: " + (String) altitude + ", Temp: " + (String) temperature);
    imufile.flush();
  }
  */
}


/* End code */
