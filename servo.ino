//WORKING WITH LITTLE SERVO
#include <Servo.h> 
String readString; //String captured from serial port
Servo myservo;  // create servo object to control a servo 
int n; //value to write to servo

void setup() {
  Serial.begin(9600);
  myservo.writeMicroseconds(1500); //set initial servo position if desired
  // myservo.attach(7, 500, 2500);  //the pin for the servo control, and range if desired
  // myservo.attach(7, 0, 500000);
  myservo.attach(7, 0, 180); // project
  // myservo.attach(7, 0, 202); // lab
  Serial.println("servo all-in-one test code 12-25-13"); // so I can keep track of what is loaded
  Serial.println();
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  if (readString.length() >0) {
    Serial.println(readString);  //so you can see the captured string 

      // attach or detach servo if desired
    if (readString == "d") { 
      myservo.detach(); //detach servo
      Serial.println("servo detached");
      goto bailout; //jump over writing to servo
    }
    if (readString == "a") {
      myservo.attach(7); //reattach servo to pin 7
      Serial.println("servo attached");
      goto bailout;
    }    

    n = readString.toInt();  //convert readString into a number

    // auto select appropriate value
    if(n >= 500)
    {
      Serial.print("writing Microseconds: ");
      Serial.println(n);
      myservo.writeMicroseconds(n);
    }
    else
    {   
      Serial.print("writing Angle: ");
      Serial.println(n);
      myservo.write(n); 
    }

bailout: //reenter code loop
    Serial.print("Last servo command position: ");    
    Serial.println(myservo.read());
    Serial.println();
    readString=""; //empty for next input
  }
}




// #include <Servo.h>
// int servoPin=7;
// Servo myServo;

// void setup ()
// {
//   Serial.begin(9600);
//   myServo.attach(servoPin);
// }
// void loop ()
// {
//   myServo.write(180);
// }



