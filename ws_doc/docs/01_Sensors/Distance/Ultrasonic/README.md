## HC-SR04 Ultraschallsensor

Der HC-SR04 ist ein beliebter Ultraschallsensor, der häufig zum Messen von Entfernungen verwendet wird. Er wird oft in Robotern verwendet, um Hindernisse zu erkennen und zu umgehen.

### Anschluss des Sensors

Der HC-SR04 hat vier Pins: VCC (Stromversorgung), Trig (Trigger), Echo (Echo) und GND (Masse). Schließen Sie VCC an den 5V-Pin Ihres Arduino, GND an einen GND-Pin, Trig an einen digitalen Pin (z.B. D7) und Echo an einen anderen digitalen Pin (z.B. D8) an.

### Betrieb des Sensors

Der Sensor misst Entfernungen, indem er einen Ultraschallschall auslöst und dann die Zeit misst, bis das Echo zurückkommt. Die Entfernung kann dann mit der Schallgeschwindigkeit berechnet werden.

### Arduino-Beispielcode

Hier ist ein einfaches Arduino-Skript, das die Entfernung mit dem HC-SR04 misst und auf dem seriellen Monitor ausgibt:

```c
#define TRIG_PIN 7
#define ECHO_PIN 8

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);
}

```

Dieses Skript sendet jede Sekunde einen Ultraschallimpuls und berechnet dann die Entfernung basierend auf der Zeit, die das Echo benötigt, um zurückzukommen. Die gemessene Entfernung wird dann auf dem seriellen Monitor ausgegeben.