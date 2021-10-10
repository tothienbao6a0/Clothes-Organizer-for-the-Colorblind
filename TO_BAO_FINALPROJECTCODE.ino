/*
  FILE NAME: TO_BAO_FINALPROJECTCODE
  AUTHOR: Bao To
  Date created September 12th 2021
  Description: This is the code for the automatic clothes finder project. The purpose of this code
  is to detect the colors of clothing items and save them in an array, allowing for communication with the arduino controller to
  enable the mechanical system to display the clothing item of the right color to the user.

  Upon uploading this program to the circuit, the pan servo will begin to move the camera around and scan the objects, saving its color
  signature to an array. The code written in the loop allows it to select unique values only (sadly, the array data structure is the
  only one available for arduino, so I could not use other types of data structures that adds only unique value, and instead had to
  write the code to make this function possible.) and add them into the array. This array stores the data of all uniquely detected colors,
  in other words all unique "clothing items" or objects. The array will then be printed out to the monitor.

  Afterwards, when the user presses a button in the user input subsystem, the microcontroller will receive the signal to send its own signal to the correct servo, which will move the clothing item to show it to the user. 

  Functions:
  - pixy.init(): start the camera
  - pixy.changeProg(string): call another program to use for PixyMon app
  - pixy.setServos(int,int): set pan and tilt servo values for the camera to move
  - pixy.ccc.getBlocks(): detect colors
  - checkUserInput(): check which button is pressed
  - moveColorServo(int colorSig): go through all the array and check if any of the items in the array has the right color signature as chosen by the user. if yes, move the servo holding that item to show to the user.

  Variables:
  - pixy.cc.blocks[0].m_signature: the block value is 0 by default of pixy library is set to the signature of the biggest object detected,
  which is useful because it allows more accurate and directed color detection of the object that is the closest to the screen,
  instead of random objects everywhere in the camera space.


*/
//--------------------------------------------
//include all necessary libraries
#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>

Pixy2 pixy; //create Pixy2 object
int detectedObjects[10]; //create a detected objects array
int starter = 0; //int to be used in object detection algorithm
int servoMove = 0; //move variable for pan and tilt servos
int mechServoMovement = 90; // move variable for SG90 servos
Servo mechServos[7]; //array used to control mechanical subsystem's (SG90) servos
boolean detected = false; //used to represent whether an object is detected or not
boolean deviceOn = false; //used to represent whether the device is activated or not
//creates all servos objects

int purpleKeyPin = 36; //set all pins for buttons for colors and ON/OFF
int blueKeyPin = 38;
int cyanKeyPin = 40;
int greenKeyPin = 42;
int yellowKeyPin = 44;
int orangeKeyPin = 46;
int redKeyPin = 48;
int onButtonPin = 23; 
int offButtonPin = 25;

void setup() {
  //runs once
  Serial.begin(115200); // begin serial connection with the serial monitor
  Serial.print("Starting...\n");

  pixy.init();//start the camera
  pixy.changeProg("color_connected_components"); //call the color detection program



  mechServos[0].attach(4); //attach SG90 servos to the correct signal pins on the microcontroller
  mechServos[1].attach(5);
  mechServos[2].attach(6);
  mechServos[3].attach(7);
  mechServos[4].attach(8);
  mechServos[5].attach(9);
  mechServos[6].attach(10);

  for (int i=0;i<=6;i++){

    mechServos[i].write(0); //set all SG90 servos' arm location to 0 during set up
  }
  pinMode(purpleKeyPin, INPUT_PULLUP); //set all button pins to input (pullup)
  pinMode(blueKeyPin, INPUT_PULLUP);
  pinMode(cyanKeyPin, INPUT_PULLUP);
  pinMode(greenKeyPin, INPUT_PULLUP);
  pinMode(yellowKeyPin, INPUT_PULLUP);
  pinMode(orangeKeyPin, INPUT_PULLUP);
  pinMode(redKeyPin, INPUT_PULLUP);
  pinMode(onButtonPin, INPUT_PULLUP);
  pinMode(offButtonPin, INPUT_PULLUP);


}

void loop() {


  if (digitalRead(onButtonPin) == LOW)deviceOn = true; //if the on button is pressed, the device will turn on
  if (digitalRead(offButtonPin) == LOW)deviceOn = false;//if the off button is pressed, the device will stop working


    if (deviceOn == true) { // only runs if the device is on
    
    checkUserInput(); //check which button is pressed
    detected = false; //set detected boolean to false
    if (servoMove < 1000)servoMove+=2; //change the horizontal location of the pan servo
    else servoMove = 0;
    Serial.print("servo moved ");
    Serial.println(servoMove);
    pixy.setServos(servoMove,600); //set the horizontal value of the servo to make it move horizontally

    pixy.ccc.getBlocks();//getBlocks must be called after setServos because setServos reset the getBlocks method, making the camera unable to collect the signature of the biggest object
    for (int i = 0; i <= 9; i++) {
      if (pixy.ccc.blocks[0].m_signature == detectedObjects[i]) //if the object is already detected, it is no longer added to the array as a separate value --> no duplicates
      {
        detected = true;
      }
    }
    Serial.print("block detected is = ");//print out the currently detected object.
    Serial.println(pixy.ccc.blocks[0].m_signature);
    Serial.println(detected);
    Serial.println("starter is = " + starter);
    if (pixy.ccc.blocks[0].m_signature >= 1 && pixy.ccc.blocks[0].m_signature <= 7 && detected == false && starter <= 9)
    //if the color signature is valid (between 1 and 7), it has not been detected before, and the value is within range of the array, set the color signature to be a part of the array
    {
      detectedObjects[starter] = pixy.ccc.blocks[0].m_signature;
      starter++;
      Serial.print("detected block is " );

      Serial.println( detectedObjects[starter]);
      //block 0 is by default the largest block in the picture.
      //Therefore as the camera tilt, that block will be added to the detectedObjects array.

      // delay(100);
    }

    for (int j = 0; j <= 9; j++) { // prints out the array of uniquely detected objects

      Serial.println(detectedObjects[j]);
      //delay(500);
    }
    Serial.print("\n");
                                                                                                                                                                                                                               
     }
  }                                 

  
    void checkUserInput() {
    if (digitalRead(purpleKeyPin) == LOW)moveColorServo(7);//if the button is pressed, move the servos that contains the right color signature
    if (digitalRead(blueKeyPin) == LOW)moveColorServo(6);
    if (digitalRead(cyanKeyPin) == LOW)moveColorServo(5);
    if (digitalRead(greenKeyPin) == LOW)moveColorServo(4);
    if (digitalRead(yellowKeyPin) == LOW)moveColorServo(3);
    if (digitalRead(orangeKeyPin) == LOW)moveColorServo(2);
    if (digitalRead(redKeyPin) == LOW)moveColorServo(1);
    }

    void moveColorServo(int colorSig) {
    for (int k = 0; k <= 6; k++) {

      if (detectedObjects[k] == colorSig) mechServos[k].write(mechServoMovement); //go through all the array and check if any of the items in the array has the right color signature as chosen by the user. if yes, move the servo holding that item to show to the user.

    }
    }
  
