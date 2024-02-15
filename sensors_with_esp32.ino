//Code to inetrface Temperature and vibration sensor with ESP32


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <max6675.h>

Adafruit_MPU6050 mpu;

float frequency_Hz = 200.0; 			// Desired frequency of vibration

int thermoDO = 19;             		//Define the pins that are interfacing with the MAX6675 thermocouple amplifier.
int thermoCS = 23;
int thermoCLK = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);               //Create a MAX6675 object called thermocouple on the pins

void setup(void) 
{
  Serial.begin(115200);
  while (!Serial)
    delay(2000);                  					   // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Reading values from Sensors..!");

  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");

    while (1)
    {
      delay(10);
    }
  }
    Serial.println("MPU6050 Found!");
  
  delay(2000);
  

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);           // set accelerometer range to +-16G
    Serial.print("Accelerometer range set to: ");
    Serial.println(mpu.getAccelerometerRange());

    
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);               // set gyro range to +- 500 deg/s
    Serial.print("Gyro range set to: ");
    Serial.println(mpu.getGyroRange());
    
  
    mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);           // set filter bandwidth to 184 Hz
    Serial.print("Filter bandwidth set to: ");
    Serial.println(mpu.getFilterBandwidth());

    delay(2000);
}

void loop() 
{
 
 Serial.print("Temperature = "); 

  Serial.println(thermocouple.readCelsius());                     // returns temperature in Celsius degrees.  

  if(thermocouple.readCelsius() > 30 && thermocouple.readCelsius() <=40 )
  {
     Serial.println("Ambient temperature.Motor is Normal.!!! \n");
  }
  else if(thermocouple.readCelsius() > 40)
  {
    Serial.println("Temperature exceeds ambient temprature ..detecting some issues in motor \n");
  }
  
  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  
  delay(2000);
 
                                            
  sensors_event_t a,g,temp;			// Get new sensor events with the readings
  mpu.getEvent(&a,&g,&temp);

  Serial.println("\n");                                           
  Serial.print("Acceleration X: ");		// Print out the values of acceleration
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  delay(2000);
   
  Serial.print("Rotation X: ");               // Print out the values of GyroRange
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  delay(2000);

  // Serial.print("Temperature: ");             // Print out the values of temperature during acceleration and gyro
  // Serial.print(temp.temperature);
  // Serial.println(" degC");

  // delay(2000);

  double accelerationX = a.acceleration.x / 16384.0;
  double accelerationY = a.acceleration.y / 16384.0;
  double accelerationZ = a.acceleration.z / 16384.0;

  double accelerationMagnitude = sqrt(pow(accelerationX, 2) + pow(accelerationY, 2) + pow(accelerationZ, 2));            //calculate acceleration magnitude do the RMS of g values.

  double vibrationVelocity_mm_s = accelerationMagnitude * frequency_Hz * 9.81;           //vibration velocity can be calculated using acceleration magnitude, frequency, and acceleration of gravity.

  Serial.print("Vibration Velocity :");
  Serial.println(vibrationVelocity_mm_s);

  if(vibrationVelocity_mm_s >=2.8 && vibrationVelocity_mm_s <=4.5)
  {
    Serial.println("Problem may cause due to excessive vibration, such as unbalance or misalignment");
  }
  
  delay(2000);
}
