#include <dht.h>
#include <AccelStepper.h>

#define DHT11_PIN 3
#define sensorPower 2
#define sensorPin A0

#define FULLSTEP 4
#define HALFSTEP 8

#define photoResistancePin 2

AccelStepper stepper1(HALFSTEP, 8, 10, 9, 11);
AccelStepper stepper2(FULLSTEP, 4, 6, 5, 7);
dht DHT;
int waterLevel = 0;
int photoResistanceValue = 0;

void setup() {
  setupWheels();
  setupWaterLevelSensor();
  Serial.begin(9600);
}

void loop() {

  int starttime = millis();
  int endtime = starttime;
  while ((endtime - starttime) <=5000) 
  {
  activateWheel();
  endtime = millis();
  }
  Serial.println("Sensors");
  readPhotoResistanceSensorData();
  readHumidityAndTemperatureSensorData();
  readWaterLevelSensorData(); 
}

void setupWaterLevelSensor() {
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
}

void readWaterLevelSensorData() {
  digitalWrite(sensorPower, HIGH);
  delay(10);
  waterLevel = analogRead(sensorPin);
  digitalWrite(sensorPower, LOW);
  Serial.println("Water level: ");
  Serial.println(waterLevel);
}

void readPhotoResistanceSensorData() {
  photoResistanceValue = analogRead(photoResistancePin);
  Serial.print("Photoresistor Value: ");
  Serial.println(photoResistanceValue, DEC);
  delay(100);
}

void readHumidityAndTemperatureSensorData() {
  int chk = DHT.read11(DHT11_PIN);

  Serial.print("Temperature (°C): ");
  Serial.println(DHT.temperature);

  Serial.print("Humidity (%): ");
  Serial.println(DHT.humidity);

  delay(1000);
}


void setupWheels() {
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(500.0);
  stepper1.setSpeed(4000);
  stepper1.moveTo(-2038);

  stepper2.setMaxSpeed(1000.0);
  stepper2.setAcceleration(500.0);
  stepper2.setSpeed(4000);
  stepper2.moveTo(2038);
}

void deactivateWheels() {
  stepper1.setSpeed(0);
  stepper2.setSpeed(0);
}

void activateWheel() {
  if (stepper1.distanceToGo() == 0)
    stepper1.moveTo(-stepper1.currentPosition());
  if (stepper2.distanceToGo() == 0)
    stepper2.moveTo(-stepper2.currentPosition());

  stepper1.run();
  stepper2.run();
}