#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>

SoftwareSerial mySerial(10, 11); // TX,RX for DFPlayer
DFPlayerMini_Fast myMP3;

const unsigned long detectionDuration = 2500; // 3 seconds (3000 ms)
unsigned long obstacleStartTime = 0;
bool obstaclePresent = false; // Tracks if obstacle is in range
const int trigPin = 7;
const int echoPin = 4;
const int led = 12;

void setup() {
  // Set up pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);

  // Start serial communications
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(1000);  // Give DFPlayer Mini some time to initialize

  // Initialize DFPlayer Mini
  if (!myMP3.begin(mySerial)) {
    Serial.println("DFPlayer Mini not detected!");
    while (true); // Stop execution if DFPlayer is not detected
  }

  myMP3.volume(30);  // Set volume level (0 to 30)
  delay(5000);
  Serial.println("DFPlayer Mini ready!");

  // Test track (optional)
  Serial.println("Test track");
  myMP3.play(2);
  delay(10000);
  Serial.println("Test Success");
  digitalWrite(led,HIGH);
  delay(2500);
  digitalWrite(led,LOW);

}

void loop() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10 microsecond pulse to the trigPin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the time it takes for the echo to come back
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm
  int distance = duration * 0.034 / 2;

  // Print the distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(600);

  // Check if the distance is between 30 and 100 cm (obstacle detected)
  if (distance > 30 && distance < 100) {
    if (!obstaclePresent) {
      // If obstacle wasn't detected before, mark the start time
      obstaclePresent = true;
      obstacleStartTime = millis();
    }

    // Check if the obstacle has been there for more than 3 seconds
    if (millis() - obstacleStartTime >= detectionDuration) {
      Serial.println("Obstacle Detected for 3 Seconds");
      Serial.println("Playing Track");

      // Play the track
      myMP3.play(1);
      digitalWrite(led,HIGH);
      // Reset the timer and obstacle state
      obstaclePresent = false;  // Reset to avoid replaying
      delay(6000); 
      digitalWrite(led,LOW);
       // Wait for 6 seconds to allow track to finish
    }
  } else {
    // Reset if no obstacle is detected or obstacle moved out of range
    obstaclePresent = false;
    obstacleStartTime = 0;
    Serial.println("No Obstacle");
  }
}
