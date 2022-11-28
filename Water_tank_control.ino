#include <ESP32Servo.h>

#define ULTRASONIC_ECHO 2
#define ULTRASONIC_TRIG 4

Servo myServo;

int level;
double kp = 12.0, ap;
int setPoint = 1250;
double error = 0.0;

void setup() {
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  Serial.begin(115200);
  myServo.attach(15);
  delay(100);
}

void loop() {
  delay(500);

  control();

  Serial.println("Distance: " + String(getDistance()) + " Reference: " + String(getReference()) + " Error: " + String(error));
}

// Returns distance between the sensor and the object
float getDistance() {
  long elapsedTime;
  float distance;
  
  digitalWrite(ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG, LOW);

  elapsedTime = pulseIn(ULTRASONIC_ECHO, HIGH);
  distance = float(elapsedTime * 0.0343) / 2;
  
  delay(10);
  return distance;
}

// Sets a relation between the setPoint and the distance,
// the distance is between the sensor and the water.
float getReference() {
  if (setPoint == 2000) {
    return 3.84;
  } else if (setPoint == 1750) {
    return 5.95;
  } else if (setPoint == 1500) {
    return 8.04;
  } else if (setPoint == 1250) {
    return 10.20;
  } else if (setPoint == 1000) {
    return 12.30;
  } else if (setPoint == 750) {
    return 15.06;
  }
}

int getLevel() {
  static int level;
  float errorMarginTop = 0.2; // cm
  float errorMarginBottom = 0.2; // cm
  
  if(getDistance() >= 3.84 - errorMarginBottom && getDistance() <= 3.84 + errorMarginTop) {
    level = 2000;
  } else if (getDistance() >= 5.95 - errorMarginBottom && getDistance() <= 5.95 + errorMarginTop) {
    level = 1750;
  } else if(getDistance() >= 8.04 - errorMarginBottom && getDistance() <= 8.04 + errorMarginTop) {
    level = 1500;
  } else if(getDistance() >= 10.20 - errorMarginBottom && getDistance() <= 10.20 + errorMarginTop) {
    level = 1250;
  } else if(getDistance() >= 12.30 - errorMarginBottom && getDistance() <= 12.30 + errorMarginTop) {
    level = 1000;
  } else if(getDistance() >= 15.06 - errorMarginBottom && getDistance() <= 15.06 + errorMarginTop) {
    level = 750;
  }
  
  return level;
}

void closeValve() {
  myServo.write(50);
  Serial.println("Closed");
}

void openValve() {
  myServo.write(120);
  Serial.println("Opened");
}

void control() {
  error = getReference() - getDistance();

  if (getReference() < getDistance()) {
    closeValve();
  } else if (getReference() > getDistance()) {
    openValve();
  }

  /*
  if (getDistance() < 2.0) {
    closeValve();
  } else if (error > 4.0) {
    openValve();
  } else if (error > 0.0 && error < 1.5) {
    openValve();
  } else if(error > -3.0 && error < 0.0) {
    closeValve();
  } else if (error < -3.0) {
    closeValve();
  } */
}
