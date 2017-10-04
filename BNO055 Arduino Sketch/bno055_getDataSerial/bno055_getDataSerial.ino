
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Este driver realiza lectura del sensor BNO055

   Conexiones
   ===========
   SCL a analog 5, D1
   SDA a analog 4, D2
   VDD a 3.3V DC
   GROUND a common ground
*/

/* delay entre muestras */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno055_sensor = Adafruit_BNO055(1,BNO055_ADDRESS_A);

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
  bno055_sensor.setExtCrystalUse(true);
}

void loop()
{
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
  
  uint8_t system, gyro, accel, mag = 0;
  int calib_byte = 0;
  bno055_sensor.getCalibration(&system, &gyro, &accel, &mag);
  calib_byte = (system<<6) |(gyro<<4) | (accel<<2) | mag;
  
  String temp;
  temp = "0";
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
  temp += calib_byte;
  temp+=",";
  temp += "1"; //checksum: w^2+x^2+y^2+z^2 = 1
  temp+=",";
  temp+=millis();
  
  Serial.println(temp);
  
  delay(BNO055_SAMPLERATE_DELAY_MS);
}
