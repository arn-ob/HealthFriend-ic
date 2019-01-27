
#include <SoftwareSerial.h>

// software serial #1: RX = digital pin 10, TX = digital pin 11
SoftwareSerial portOne(10, 11);

// software serial #2: RX = digital pin 8, TX = digital pin 9
// on the Mega, use other pins instead, since 8 and 9 don't work on the Mega


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // Start each software serial port
 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void loop() {
  // By default, the last intialized port is listening.
  // when you want to listen on a port, explicitly select it:
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  delay(5000);
  // One ------------ 2 HIGH
  digitalWrite(2, HIGH);
  delay(5000);
  Serial.println("Data from Part one:");
  // pin Select
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(1000);
  
  SerialRead();
  delay(5000);
  digitalWrite(2, LOW);

  // blank line to separate data from the two ports:
  // Serial.println();
  delay(3000);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(3000);

  // Two ----- 3 HIGH
  digitalWrite(3, HIGH);
  delay(5000);
  Serial.println("Data from Part Two:");

  // pin Select
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(1000);
  
  SerialRead();
  delay(5000);
  digitalWrite(3, LOW);
  
  // pin Select
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(2000);
}


void SerialRead(){
  portOne.begin(115200);
  portOne.listen();
  String a = "";
  while (portOne.available() > 0) {
    char inByte1 = portOne.read();
    a += inByte1;
    // Serial.write(inByte1);
  }
  Serial.println(a);
  a = "";
  delay(5000);
  Serial.println();
}
