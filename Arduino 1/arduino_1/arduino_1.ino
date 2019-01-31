// Sensor List
// GSR
// Max 30100
// Red EMG

#include <MAX30100.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_PulseOximeter.h>
#include <MAX30100_Registers.h>
#include <MAX30100_SpO2Calculator.h>
#include <Wire.h>

#define REPORTING_PERIOD_MS    500

PulseOximeter pox;

const int numReadings = 10;
float filterweight = 0.5;
uint32_t tsLastReport = 0;
uint32_t last_beat = 0;
int readIndex = 0;
int average_beat = 0;
int average_SpO2 = 0;
bool calculation_complete = false;
bool calculating = false;
bool initialized = false;
byte beat = 0;
boolean Is_Max_30100_INIT = false;

// GSR
const int GSR = A0;
int sensorValue = 0;
int gsr_average = 0;

#define pin_1 2
#define pin_2 3
#define pin_3 4


void setup()
{
  Serial.begin(115200);
  
  pinMode(pin_1, INPUT);
  pinMode(pin_2, INPUT);
  pinMode(pin_3, INPUT);
  Serial.println("System Start System 1");
}


void loop()
{
  if (digitalRead(pin_1)) {
    Serial.println("Pin 1 HIGH");
    EMG_func();
  } else {
    // Serial.println("Pin 1 LOW");
  }
  if (digitalRead(pin_2)) {
     Serial.println("Pin 2 HIGH");
    MAX_30100_func();
  } else {
    // Serial.println("Pin 2 LOW");
  }
  if (digitalRead(pin_3)) {
     Serial.println("Pin 3 HIGH");
    GSR_func();
  } else {
    // Serial.println("Pin 3 LOW");
  }

}


// EMG Sensor
void EMG_func() {
  int sensorValue = analogRead(A1);
   Serial.print("EMG_func");
  Serial.println(sensorValue);
  delay(1);
}

// GSR //////////////////////////////////
void GSR_func() {
  long sum = 0;
  for (int i = 0; i < 10; i++)    //Average the 10 measurements to remove the glitch
  {
    sensorValue = analogRead(GSR);
    sum += sensorValue;
    delay(5);
  }
  gsr_average = sum / 10;
  Serial.print("gsr_average");
  Serial.println(gsr_average);
}
/// GSR END ///////////////////////


// MAX 301000 ///////////////////////////////////////

void MAX_30100_INIT(){
  pox.begin();
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void MAX_30100_func() {
  if(!Is_Max_30100_INIT){
    MAX_30100_INIT();
    Is_Max_30100_INIT = true;
  }
  
  pox.update();
  if ((millis() - tsLastReport > REPORTING_PERIOD_MS) and (not calculation_complete)) {
    calculate_average(pox.getHeartRate(), pox.getSpO2());
    tsLastReport = millis();
  }
  if ((millis() - last_beat > 10000)) {
    calculation_complete = false;
    average_beat = 0;
    average_SpO2 = 0;
    initial_display();
  }
}

void onBeatDetected() //Calls back when pulse is detected
{
  viewBeat();
  last_beat = millis();
}

void viewBeat()
{

  if (beat == 0) {
    Serial.print("_");
    beat = 1;
  }
  else
  {
    Serial.print("^");
    beat = 0;
  }
}

void initial_display()
{
  if (not initialized)
  {
    viewBeat();
    Serial.print(" MAX30100 Pulse Oximeter Test");
    Serial.println("******************************************");
    Serial.println("Place place your finger on the sensor");
    Serial.println("********************************************");
    initialized = true;
  }
}

void display_calculating(int j) {

  viewBeat();
  Serial.println("Measuring");
  for (int i = 0; i <= j; i++) {
    Serial.print(". ");
  }
}

void display_values()
{
  Serial.print(average_beat);
  Serial.print("| Bpm ");
  Serial.print("| SpO2 ");
  Serial.print(average_SpO2);
  Serial.print("%");
}

void calculate_average(int beat, int SpO2)
{
  if (readIndex == numReadings) {
    calculation_complete = true;
    calculating = false;
    initialized = false;
    readIndex = 0;
    display_values();
  }

  if (not calculation_complete and beat > 30 and beat<220 and SpO2>50) {
    average_beat = filterweight * (beat) + (1 - filterweight ) * average_beat;
    average_SpO2 = filterweight * (SpO2) + (1 - filterweight ) * average_SpO2;
    readIndex++;
    display_calculating(readIndex);
  }
}

//////////////////////////////////////////////////////