const int trigPin = 9;

const int echoPin = 10;

const int ledPin = 11;

const int btnPin = 12;

int btnState = 0;

float baseline = 0;

bool runnState = false;

float duration, distance;

void setup() {
  // put your setup code here, to run once:

  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);

  pinMode(ledPin, OUTPUT);

  pinMode(btnPin, INPUT_PULLUP);


  Serial.begin(9600);
}

void loop() {

  // Check button
  btnState = digitalRead(btnPin);

  if (!runnState) {
    if (btnState == LOW) {
      Serial.println("System online");
      runnState = true;

      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);  // Brief delay to ensure clean LOW
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);  // 10 microsecond pulse width
      digitalWrite(trigPin, LOW);

      // Measure the pulse duration on echo pin
      duration = pulseIn(echoPin, HIGH, 100000);  // 100ms timeout, adjust if needed

      distance = (duration / .0343) / 2;

      baseline = distance;
    }
  } else {

    if (btnState == LOW) {
      // Hold for two second to turn device off
      delay(2000);
      btnState = digitalRead(btnPin);

      if (btnState == LOW) {
        Serial.println("Turning off");
        runnState = false;
        // Blink to indicate turning off
        for (int i = 0; i <= 5; i++) {
          digitalWrite(ledPin, LOW);
          delay(100);
          digitalWrite(ledPin, HIGH);
          delay(100);
        }
        digitalWrite(ledPin, LOW);


      } else {
        Serial.println("Resetting baseline");
        // Reset baseline

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);  // Brief delay to ensure clean LOW
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);  // 10 microsecond pulse width
        digitalWrite(trigPin, LOW);

        // Measure the pulse duration on echo pin
        duration = pulseIn(echoPin, HIGH, 100000);  // 100ms timeout, adjust if needed
        distance = (duration / .0343) / 2;


        baseline = distance;
      }
    }

    if (runnState) {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);  // Brief delay to ensure clean LOW
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);  // 10 microsecond pulse width
      digitalWrite(trigPin, LOW);

      // Measure the pulse duration on echo pin
      duration = pulseIn(echoPin, HIGH, 100000);  // 100ms timeout, adjust if needed

      distance = (duration / .0343) / 2;

      Serial.print(distance);
      Serial.print(" / ");
      Serial.print(baseline);
      Serial.println();
      if (distance < baseline - 10000 || distance > baseline + 10000) {
        digitalWrite(ledPin, HIGH);  // LED ON when out of range
      } else {
        digitalWrite(ledPin, LOW);  // LED OFF when within range
      }


      delay(250);
    }
  }
}
