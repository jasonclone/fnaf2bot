#pragma once
void simulateLeftClick();

void simulateUpArrowKeyPress();

void toggleCam();

void lookLeft(int flashInterval); //lower the flash interval, more rapid the flash

void lookRight(int flashInterval); //lower the flash interval, more rapid the flash

void toggleCam();

void openCamAndSetToPrizeCorner();


void toggleMask();

void blackoutFromToyBonnie();
//waits then toggles mask if toy bonnie detected, then lifts it after blackout 

bool checkBlackoutAfterCam(); //returns true if blackout occured, false if it didnt happen!! (Toggles mask)

void flash();

void foxyFlash();

int checkRightVent(); //presses light, but does NOT toggle mask!

int checkLeftVent(); //presses light, but does NOT toggle mask!
