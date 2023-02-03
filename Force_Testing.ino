// Code was heavily altered, but the base was taking from the below example:

/******************************************************************************
Force_Sensitive_Resistor_Example.ino
Example sketch for SparkFun's force sensitive resistors
  (https://www.sparkfun.com/products/9375)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining an FSR with a 3.3k resistor.
- The resistor should connect from A0 to GND.
- The FSR should connect from A0 to 3.3V
As the resistance of the FSR decreases (meaning an increase in pressure), the
voltage at A0 should increase.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/ 



const int FSR_PIN = A2; // Pin connected to FSR/resistor divider

// Measure the voltage at 5V and resistance of your 3.3k resistor, and enter
// their value's below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

// Variables:
// - start time: time when the readings begin
// - rising: to track if the sensor data is rising
// - tracking: to see if we are reading data
// - force_arr: used for data smoothing
float start_time = 0.0;
bool rising = true;
bool tracking = false;
float force_arr[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() 
{
  // Start Serial
  Serial.begin(9600);
  pinMode(FSR_PIN, INPUT);  
}

void loop() 
{
  int fsrADC = analogRead(FSR_PIN);
  // If the FSR has no pressure, the resistance will be
  // near infinite. So the voltage should be near 0.
  if (fsrADC != 0) // If the analog reading is non-zero
  {
    // Use ADC reading to calculate voltage:
    float fsrV = fsrADC * VCC / 1023.0;
    // Use voltage and static resistor value to 
    // calculate FSR resistance:
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
    // Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    float force;
    float fsrG = 1.0 / fsrR; // Calculate conductance
    // Break parabolic curve down into two linear slopes:
    if (fsrR <= 600) 
      force = (fsrG - 0.00075) / 0.00000032639;
    else
      force =  fsrG / 0.000000642857;


    // DATA FILTERING: moving average
    // put the new force at the end of the array
    for(int i; i > 9; i++){
      force_arr[i] = force_arr[i + 1];
    }
    force_arr[9] = force;
    // Find the new average
    float arr_sum = 0;
    for(auto i: force_arr){
      arr_sum = arr_sum + i;
    }
    arr_sum = arr_sum / 10;

    // start tracking the gripping time: if the gripping force is less than 5 N don't track it
    if (arr_sum > 5 && rising){
      start_time = millis();
      rising = false;
      tracking = true;
    }
      
    if(tracking){
      // print out all relevant data to the serial 
      Serial.print((millis() - start_time)/1000);
      Serial.print(",");
      Serial.print(arr_sum);
      Serial.print(",");
      Serial.println(tracking); // boolean: if we are still tracking numbers or not
    }  

    // If we are done tracking the force is low, reset values to exit loop 
    // also print correct tracking value.
    if(arr_sum < 5 && tracking){
      tracking = false;
      rising = true;
      Serial.print("0,0,");
      Serial.println(tracking);
      Serial.flush();
    }

    delay(50);
  }
  ;
}