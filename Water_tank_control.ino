#define ULTRASONIC_ECHO 2
#define ULTRASONIC_TRIG 4

void setup() {
  pinMode(ULTRASONIC_ECHO, INPUT);
  pinMode(ULTRASONIC_TRIG, OUTPUT);
  Serial.begin(115200);
  delay(100);
}

void loop() {
  getDistance();
  Serial.println("Distance: " + String(getDistance()) + "cm");
  Serial.println("Height: " + String(getHeight()) + "cm");
  delay(1000);
}

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

float getHeight() {
  float TANK_HEIGHT = 22.0;
  float height;

  height = TANK_HEIGHT - getDistance();
  return height;
}
