# Clothes-Organizer-for-the-Colorblind-Pioneer-Research-
Engineering Research Paper on the design of a device which uses color sensing and object detection technologies to support colorblind people when choosing the colors for their outfits.

  FILE NAME: README.md
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
