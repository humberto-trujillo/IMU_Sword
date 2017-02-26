

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>


/* Este driver realiza lectura del sensor BNO055

   Conexiones
   ===========
   SCL a analog 5, D1
   SDA a analog 4, D2
   VDD a 3.3V DC
   GROUND a common ground
*/

/* delay entre muestras */
#define BNO055_SAMPLERATE_DELAY_MS (50)

Adafruit_BNO055 bno055_sensor = Adafruit_BNO055(1,BNO055_ADDRESS_A);
/*
const char* ssid = "STR";
const char* password = "LABRTS2011-1";
*/
const char* ssid = "dlink-A4F9";
const char* password = "xywwo52000";


unsigned int port = 12345;
const char* host = "192.168.100.103";
const char* host_name = "OTA-HS-FW";

char trama[150];
String sensor_ID = "3";  

WiFiUDP Udp;

void setup(void)
{
  Serial.begin(9600);
  /* Inicializar el sensor */
  if(!bno055_sensor.begin())
  {
    Serial.print("Sensor BNO055 no detectado... revisar cableado I2C ADDR!");
    while(1);
  }
  
  delay(1000);
  
  /* Obtener temperatura actual
  int8_t temp1 = bno055_sensor.getTemp();*/
  
  bno055_sensor.setExtCrystalUse(true);
  
  Serial.println("Estados de calibracion: 0=no calibrado, 3=completamente calibrado");
                                           
  WiFi.begin(ssid, password);
  
  while(WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");  
  }
  
  /* Config servidor OTA */
  ArduinoOTA.setHostname(host_name);
  ArduinoOTA.onError([](ota_error_t error) { ESP.restart();});
  ArduinoOTA.begin();
  Serial.println("Listo");
}

void loop()
{
  ArduinoOTA.handle();
  // Possibles vectores:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  
  imu::Quaternion quat = bno055_sensor.getQuat();  
  imu::Vector<3> acce = bno055_sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu::Vector<3> magne = bno055_sensor.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
  imu::Vector<3> euler = bno055_sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
  
  uint8_t system, gyro, accel, mag = 0; //0: no calibrado, 3:completamente calibrado
  int calib_byte = 0;
  bno055_sensor.getCalibration(&system, &gyro, &accel, &mag);
  calib_byte = (system<<6) |(gyro<<4) | (accel<<2) | mag;
 
  String temp;
  temp = sensor_ID;
  temp+=",";
  temp+=quat.w();
  temp+=",";
  temp+=quat.x();
  temp+=",";
  temp+=quat.y();
  temp+=",";
  temp+=quat.z();
  temp+=",";
  temp+=euler.x();
  temp+=",";
  temp+=euler.y();
  temp+=",";
  temp+=euler.z();
  temp+=",";
  temp+=calib_byte;
  temp+=",";
  temp+= "1"; //checksum: w^2+x^2+y^2+z^2 = 1 
  temp+=",";
  temp+=millis();
  
  Udp.beginPacket(host,port);
  temp.toCharArray(trama,temp.length());
  Udp.write(trama,temp.length());
  Udp.endPacket();

  delay(BNO055_SAMPLERATE_DELAY_MS);
}
