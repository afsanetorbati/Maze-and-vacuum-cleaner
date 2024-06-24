/* an example of an Arduino-controlled robot that uses ultrasonic sensors and GPS to navigate through a house */
/* Logic: The robot saves its current location if it hasn’t been visited. If an obstacle is detected, it reverses and turns. Once all points are visited, it can navigate to any unvisited points. */
/* afsane torbati */

#include <AFMotor.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define trigPin 12
#define echoPin 13

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

TinyGPSPlus gps;
SoftwareSerial ss(4, 3); // RX, TX

struct Point {
  double latitude;
  double longitude;
  bool visited;
};

Point points[10]; // Array to store points
int currentPoint = 0;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  motor1.setSpeed(100);
  motor2.setSpeed(100);
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  if (gps.location.isUpdated()) {
    double lat = gps.location.lat();
    double lng = gps.location.lng();
    if (!points[currentPoint].visited) {
      points[currentPoint].latitude = lat;
      points[currentPoint].longitude = lng;
      points[currentPoint].visited = true;
      currentPoint++;
    }
  }

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  if (distance < 15) {
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    delay(500);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(500);
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(500);
  } else {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
  }

  if (currentPoint >= 10) {
    for (int i = 0; i < 10; i++) {
      if (!points[i].visited) {
        // Navigate to the unvisited point
        // Add navigation logic here
      }
    }
  }
}
