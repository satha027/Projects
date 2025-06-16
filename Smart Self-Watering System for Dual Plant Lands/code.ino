#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "SelfWateringSystem"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

Servo myServo;
int moisture1, moisture2;
const int sensor1 = 34;
const int sensor2 = 35;
const int pumpPin = 14;
const int servoPin = 13;

bool manualMode = false;
int manualAngle = 90;
bool manualPump = false;

BLYNK_WRITE(V2) {
  manualPump = param.asInt();
}

BLYNK_WRITE(V3) {
  manualAngle = param.asInt();
}

BLYNK_WRITE(V4) {
  manualMode = param.asInt();
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  myServo.attach(servoPin);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
}

void loop() {
  Blynk.run();

  moisture1 = analogRead(sensor1);
  moisture2 = analogRead(sensor2);

  Blynk.virtualWrite(V0, moisture1);
  Blynk.virtualWrite(V1, moisture2);

  if (manualMode) {
    myServo.write(manualAngle);
    digitalWrite(pumpPin, manualPump ? HIGH : LOW);
    return;
  }

  if (moisture1 < 1800) {
    myServo.write(0);
    digitalWrite(pumpPin, HIGH);
  } else if (moisture2 < 1800) {
    myServo.write(180);
    digitalWrite(pumpPin, HIGH);
  } else {
    myServo.write(90);
    digitalWrite(pumpPin, LOW);
  }

  Blynk.virtualWrite(V2, digitalRead(pumpPin));
  delay(1000);
}
