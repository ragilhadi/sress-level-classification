#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "DecisionTree.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


Eloquent::ML::Port::DecisionTree clf;

MAX30105 particleSensor;

const byte RATE_SIZE = 4; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0; 
float features[2];

float start;
float finish;

float beatsPerMinute;
int beatAvg;


void features_processing(float HR){
    features[0] = HR;
    float HRV = 60.0 / HR;
    features[1] = HRV; 
}

// Decision Tree
void decisiton_tree(){
    int label = clf.predict(features);
    Serial.print("Label Detection : ");
    Serial.println(label);
    lcd.setCursor(0,1);
    lcd.print("Stress Level : ");
    lcd.setCursor(15,1);
    lcd.print(label);
}

// Compute Time
void calculate_time(float start, float end){
  float time = end - start;
  Serial.print("Computational Time : ");
  Serial.println(time);
}


void setup() {
    Serial.begin(115200);
    lcd.begin();
    lcd.clear();
    Serial.println("Initializing...");
    lcd.setCursor(0,0);
    lcd.print("Initializing...");

    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) 
    {
        lcd.setCursor(0,1);
        lcd.print("Sensor not found");
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }
    Serial.println("Place your index finger on the sensor with steady pressure.");
    lcd.setCursor(0,1);
    lcd.print("Sensor found");

    particleSensor.setup(); 
    particleSensor.setPulseAmplitudeRed(0x0A); 
    particleSensor.setPulseAmplitudeGreen(0); 
    delay(1000);
}

void loop() {
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
        rateSpot %= RATE_SIZE; //Wrap variable

        //Take average of readings
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++)
            beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
        }
    }

    if (irValue < 50000){
        lcd.clear();
        Serial.print(" No finger?");
        lcd.setCursor(0,0);
        lcd.print("Place finger");
        Serial.println();
    }

    else {
        lcd.clear();
        Serial.print("IR=");
        Serial.print(irValue);
        Serial.print(", BPM=");
        Serial.print(beatsPerMinute);
        Serial.print(", Avg BPM=");
        Serial.print(beatAvg);
        Serial.println();

        lcd.setCursor(0,0);
        lcd.print("HR :");
        lcd.setCursor(4,0);
        lcd.print(beatAvg);
        start = millis();
        features_processing(beatAvg);
        decisiton_tree();
        finish = millis();
        calculate_time(start, finish);
        Serial.println();
    }
}
