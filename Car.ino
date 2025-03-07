#include <ArduinoBLE.h>  
const int mtr1pin1 = 2;  
const int mtr1pin2 = 3;
const int mtr2pin1 = 4;  
const int mtr2pin2 = 5;
const int ENA = 6;  
const int ENB = 9;
BLEService motorService("180C");  
BLEStringCharacteristic motorCharacteristic("2A56", BLERead | BLEWrite, 1);  
void setup() {
  pinMode(mtr1pin1, OUTPUT);
  pinMode(mtr1pin2, OUTPUT);
  pinMode(mtr2pin1, OUTPUT);
  pinMode(mtr2pin2, OUTPUT);
  Serial.begin(9600);
  if (!BLE.begin()) {
    Serial.println("BLE failed!");
    while (1);
  }
  BLE.setLocalName("Car");
  BLE.setAdvertisedService(motorService);
  motorService.addCharacteristic(motorCharacteristic);
  BLE.addService(motorService);
  BLE.advertise();
  Serial.println("BLE device is on.");
   Serial.print("Device Address: ");
  Serial.println(BLE.address());
}
void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());
    while (central.connected()) {
      if (motorCharacteristic.written()) {  
        char command = motorCharacteristic.value()[0];
        Serial.print("Received: ");
        Serial.println(command);
        if (command == 'f') forward(200);  
        else if (command == 'b') backward(200);
        else if (command == 'l') left(150);
        else if (command == 'r') right(150);
        else stop();
        delay(200);
      }
    }
    Serial.println("Disconnected from BLE device.");
    stop();  
  }
  delay(100);  
}
void forward() {
  digitalWrite(mtr1pin1, HIGH);  
  digitalWrite(mtr1pin2, LOW);
  digitalWrite(mtr2pin1, HIGH);  
  digitalWrite(mtr2pin2, LOW);
  delay(1000);
  stop();
}
void backward() {
  digitalWrite(mtr1pin1, LOW);   
  digitalWrite(mtr1pin2, HIGH);
  digitalWrite(mtr2pin1, LOW);   
  digitalWrite(mtr2pin2, HIGH);
  delay(1000);
  stop();
}
void right() {
  digitalWrite(mtr1pin1, HIGH);  
  digitalWrite(mtr1pin2, LOW);
  digitalWrite(mtr2pin1, LOW);   
  digitalWrite(mtr2pin2, HIGH);
  delay(425);
  stop();
}
void left() {
  digitalWrite(mtr1pin1, LOW);   
  digitalWrite(mtr1pin2, HIGH);
  digitalWrite(mtr2pin1, HIGH);  
  digitalWrite(mtr2pin2, LOW);
  delay(425);
  stop();
}
void stop() {
  digitalWrite(mtr1pin1, LOW);
  digitalWrite(mtr1pin2, LOW);
  digitalWrite(mtr2pin1, LOW);
  digitalWrite(mtr2pin2, LOW);
}
