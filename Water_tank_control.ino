#include <ESP32Servo.h>

#define ULTRASONIC_ECHO 2
#define ULTRASONIC_TRIG 4

Servo myServo;

int level;
double ap = 0, ai = 0, ad = 0, output, error = 0, errora = 0;
double kp = 2, ki = 0.05, kd = 0.09, tm = 0.01, salida = 0;
int setPoint = 1250;

void setup() {
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  Serial.begin(115200);
  myServo.attach(15);
  delay(100);
}

void loop() {
  //delay(10);
  
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
    return 15.06 - 0.7;
  }
  
  return level;
}

// Sets a relation between the setPoint and the distance,
// the distance is between the sensor and the water.
float getReference() {
  if (setPoint == 2000) {
    return 3.84 - 0.5;
  } else if (setPoint == 1750) {
    return 5.95 - 0.2;
  } else if (setPoint == 1500) {
    return 8.04 - 0.3;
  } else if (setPoint == 1250) {
    return 10.20;
  } else if (setPoint == 1000) {
    return 12.30 - 0.5;
  } else if (setPoint == 750) {
    return 15.06 - 0.7;
  }
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
  ap = kp * error;
  ai = ki * ((tm * errora) + (tm * (error - errora)) / 2);
  ad = kd * ((error - errora) / tm);
  output += ap + ai + ad;
  //Serial.println(output);
  if (output > 100) {
    output = 100;
  }
  if (output < 0) {
    output = 0;
  }
  errora = error;

  int valveAngle = map(output, 0, 100, 70, 120);
  //Serial.println("Angle: " + String(valveAngle) + " output: " + String(output));

  myServo.write(valveAngle);
}
