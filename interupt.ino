/*
  Created By Cryus Rupa way back in 2021.
  Based on Hamad Tariq's Bluetooth Control Example.

  It's a simple sketch which waits for a character on serial
  and will then trigger one of two relays to lokc or unlock a car

  Possible string values:
  1 (to turn the Unlock Car)
  0 (to turn the Lock Car)
*/
#define LOCK_PIN 11
#define UNLOCK_PIN 12
#define CONNECTED 2
#define PASSWORD "1"

char junk;
String inputString = "";
volatile byte state = LOW;
volatile int connection = 0;
bool loggedIn = false;

void setup() {
  Serial.begin(9600);            // set the baud rate to 9600, same should be of your Serial Monitor
  digitalWrite(UNLOCK_PIN, HIGH);
  digitalWrite(LOCK_PIN, HIGH);
  pinMode(LOCK_PIN, OUTPUT); //Set high to send swith relay and send 12V to motor to LOCK the door
  pinMode(UNLOCK_PIN, OUTPUT); //Set high to send swith relay and send 12V to motor to UNLOCK the door

  pinMode(interruptPin, INPUT_PULLUP); //Hooking up Interrupt
  attachInterrupt(digitalPinToInterrupt(interruptPin), isConnected, CHANGE);
}

void loop() {
  if (connection == 1) {
    //Serial.println("Device Connected");
    if (loggedIn == true) {
      if (Serial.available()) {
        while (Serial.available()) {
          char inChar = (char)Serial.read(); //read the input
          inputString += inChar;        //make a string of the characters coming on serial
        }
        //Serial.println(inputString);
        clearSerial();
        if (inputString == "1") {       //in case of '1' Unlock
          digitalWrite(UNLOCK_PIN, LOW);
          Serial.println("Unlocking...");
          delay(500);
          digitalWrite(UNLOCK_PIN, HIGH);
        } else if (inputString == "0") { //incase of '0' Lock
          digitalWrite(LOCK_PIN, LOW);
          Serial.println("Locking...");
          delay(500);
          digitalWrite(LOCK_PIN, HIGH);
        }
        inputString = "";
      }
    } else {
      logMeIn();
    }

  } else {
    Serial.println("No Device Connected");
    loggedIn = false;
  }
}

void isConnected() {
  if (digitalRead(2) == 0) {
    connection = 0;
  } else {
    connection = 1;
  }
}

void clearSerial() {
  while (Serial.available() > 0)
  {
    junk = Serial.read() ;  // clear the serial buffer
  }
}
void logMeIn() {
  String passString = "";
  if (Serial.available()) {
    while (Serial.available())
    {
      char inChar = (char)Serial.read(); //read the input
      passString += inChar;        //make a string of the characters coming on serial
    }
    Serial.println(passString); // Output Recevied String
    clearSerial();
    if (passString == PASSWORD) { //Authenticate Device
      Serial.println("  Password Valid, Access Granted");
      passString = "";
      loggedIn = true;
    } else {
      Serial.println("  Password invalid, Access Denied");
      loggedIn = false;
      passString = "";
    }
  }
}
