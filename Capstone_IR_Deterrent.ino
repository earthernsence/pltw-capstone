// Code to run our animal deterrent on a garden plot.
// Created by Raahim Mian + Jace Royer
// for PLTW Capstone, 24-25.

// Moisture Sensor Settings
const int sensorPin = A0;       // Capacitive moisture sensor
const int ledPin = 13;          // LED for dry soil alert
const int dry = 595;            // Calibrate this value for your soil
const int wet = 239;            // Calibrate this value for your soil

// PIR + Buzzer Settings
int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 5000;

boolean lockLow = true;
boolean takeLowTime;

const int pirPin = 3;    // PIR sensor pin
const int buzzerPin = 8; // Buzzer pin

void setup() {
  Serial.begin(9600);

  // Moisture sensor setup
  pinMode(ledPin, OUTPUT);

  // PIR sensor + buzzer setup
  pinMode(pirPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  // PIR calibration
  Serial.print("Calibrating motion sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void loop() {
  // --- Moisture Sensor Check ---
  int sensorVal = analogRead(sensorPin);
  int percentageHumidity = map(sensorVal, wet, dry, 100, 0);

  Serial.print("Soil Moisture: ");
  Serial.print(percentageHumidity);
  Serial.println("%");

  if (percentageHumidity < 30) {
    digitalWrite(ledPin, HIGH); // Soil is too dry
  } else {
    digitalWrite(ledPin, LOW);  // Soil moisture is okay
  }

  // --- Motion Detection Logic ---
  if (digitalRead(pirPin) == HIGH) {
    if (lockLow) {
      lockLow = false;
      Serial.println("---");
      Serial.print("Motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");

      // Buzzer alert sequence (~5 seconds)
      for (int i = 0; i < 10; i++) {
        tone(buzzerPin, 1000);
        delay(200);
        tone(buzzerPin, 800);
        delay(200);
      }
      noTone(buzzerPin);
    }
    takeLowTime = true;
  }

  if (digitalRead(pirPin) == LOW) {
    if (takeLowTime) {
      lowIn = millis();
      takeLowTime = false;
    }

    if (!lockLow && millis() - lowIn > pause) {
      lockLow = true;
      Serial.print("Motion ended at ");
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      delay(50);
    }
  }

  delay(500); // slight delay to reduce noise
}
