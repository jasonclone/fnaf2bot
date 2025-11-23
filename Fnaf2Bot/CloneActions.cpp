#include <chrono>
#include <iostream>
#include <thread>
#include <Windows.h>

//private fns


void flash() //  (simulate CTRL key press)
{
	// Simulate a CTRL key press
	keybd_event(VK_CONTROL, 0, 0, 0);

	// Sleep for a short duration (adjust as needed)
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	// Simulate a CTRL key release
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}

void foxyFlash() //slightly longer flash
{
	// Simulate a CTRL key press
	keybd_event(VK_CONTROL, 0, 0, 0);

	//Golden Freddy Head: (308, 377) RGB: 88, 76, 56

	// Sleep for a short duration (adjust as needed)
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	HDC hdcScreen = GetDC(NULL);
	COLORREF color = GetPixel(hdcScreen, 308, 377);

	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	std::cout <<
		"Golden Freddy Head detection. If RGB vals different from RGB: 88, 76, 56 then \n"
		"STOP FLASHING TEMPORARILY!!\n";
	std::cout << "R: " << red << ", ";
	std::cout << "G: " << green << ", ";
	std::cout << "B: " << blue << "\n";

	ReleaseDC(NULL, hdcScreen);

	if (red == 88 && green == 76 && blue == 56)
	{
		//golden freddy spotted!!! sleep temporarily!!!
		// Simulate a CTRL key release
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(1400)); //sleep for one second
		return;
	}

	// Simulate a CTRL key release
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}

void holdLeftMouseButton(int duration)
{
	// Input flags
	INPUT input_down = {0};
	input_down.type = INPUT_MOUSE;
	input_down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	INPUT input_up = {0};
	input_up.type = INPUT_MOUSE;
	input_up.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	// Simulate mouse button down
	SendInput(1, &input_down, sizeof(INPUT));

	// Sleep for the specified duration (milliseconds)
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));

	// Simulate mouse button up
	SendInput(1, &input_up, sizeof(INPUT));
}

//public

void simulateLeftClick()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	// Simulate a right mouse button down event
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);

	// Simulate a right mouse button up event
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void simulateUpArrowKeyPress()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	// Simulate an up arrow key down event
	keybd_event(VK_UP, 0, 0, 0);

	// Simulate an up arrow key up event
	keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void lookLeft(int flashInterval)
{
	std::cout << "Looking left!\n";
	int x = 0;
	while (x < 45)
	{
		SetCursorPos(1, 300);
		if (x % flashInterval == 0) //use 6 for interval for 10/20
		{
			flash();
		}
		x++;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void lookRight(int flashInterval)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	std::cout << "Looking right!\n";
	int x = 0;
	while (x < 45)
	{
		SetCursorPos(1279, 300);
		if (x % flashInterval == 0) //use 6 for interval for 10/20
		{
			flash();
		}
		x++;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void toggleCam()
{
	//921,751
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	SetCursorPos(921, 551);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	SetCursorPos(921, 751);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	SetCursorPos(921, 551);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	SetCursorPos(1240, 400); //set cursor to right of screen
}

void openCamAndSetToPrizeCorner()
{
	toggleCam();
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); //delay for camera to go up before click
	SetCursorPos(1190, 487);

	simulateLeftClick();
	std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Time for click to happen!!!!
	SetCursorPos(541, 609); //set cursor to music box for winding
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); // WAIT FOR CAMERA TO GO UP!!!!
}


void wind()
{
	holdLeftMouseButton(1); //each call of wind() will hold down on music box for 10ms
}

void toggleMask() //SURROUND EACH TOGGLE MASK FN CALL WITH 100 MS DELAY!!!
{
	//314,751
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	SetCursorPos(564, 651);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	SetCursorPos(564, 751);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	SetCursorPos(564, 651);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	SetCursorPos(1240, 400); //set cursor to right of screen
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void blackoutFromToyBonnie() //call this fn if mask is DOWN and looking RIGHT!!!
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //***wait for mask to go all the way down
	//Pixel in mask eye hole used to detect blackout :  752, 369

	//wait for blackout to occur

	int checks = 0;

	while (checks < 1000)
	{
		if (checks % 40 == 0 && checks != 0)
		//few seconds passed and no blackout, quickly flash hall and pull mask back down!
		{
			toggleMask(); //pull mask up
			for (int i = 0; i < 7; ++i) //(faster strobe) quick flash to stun foxy after vent check (was 7)
			{
				foxyFlash();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				// delay between each flash *AND before checking right vent!!!!
			}
			toggleMask(); //put mask back down
			checks++;
			std::this_thread::sleep_for(std::chrono::milliseconds(50)); //delay between each check
			continue;
		}


		HDC hdcScreen = GetDC(NULL);
		COLORREF color = GetPixel(hdcScreen, 752, 369);

		int red = GetRValue(color);
		int green = GetGValue(color);
		int blue = GetBValue(color);

		std::cout << "Toy Bonnie detection: If RG 0 and B <= 8, then blackout has started!!\n";
		std::cout << "R: " << red << ", ";
		std::cout << "G: " << green << ", ";
		std::cout << "B: " << blue << "\n";

		if (red == 0 && green == 0 && (blue > 0 && blue <= 8))
		{
			SetCursorPos(1240, 100); //cursor indication that bot detected blackout ************
			std::cout << "Blackout has started! Toy bonnie has appeared!\n";
			return;
		}

		ReleaseDC(NULL, hdcScreen);
		checks++;
		std::this_thread::sleep_for(std::chrono::milliseconds(50)); //delay between each check
		std::cout << "Still waiting for toy bonnie.... (Attempts: " << checks << " )\n";
	}
}

bool checkBlackoutAfterCam() //call this fn if mask is DOWN and looking RIGHT!!!
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //delay to make sure mask down is captured

	toggleMask(); //try to pull up mask. if detection of mask pixel still there, there must be a blackout!
	std::cout << "Mask attempted to be pulled up\n";

	std::this_thread::sleep_for(std::chrono::milliseconds(350)); //delay to detect possible mask up pixel


	//Get the pixel rgb values.
	//Pixel on mask to detect if mask still down(for blackoutAfterCam() fn)
	//1154, 149


	HDC hdcScreen = GetDC(NULL);
	COLORREF color = GetPixel(hdcScreen, 1154, 149);

	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	std::cout << "Mask detection. If RGB All 0, blackout is detected!!\n";
	std::cout << "R: " << red << ", ";
	std::cout << "G: " << green << ", ";
	std::cout << "B: " << blue << "\n";

	ReleaseDC(NULL, hdcScreen);

	if (red == 0 && green == 0 && blue == 0)
	{
		SetCursorPos(1240, 100); //cursor indication that bot detected blackout
		std::cout << "Attempt to lift mask failed!:\n";
		std::cout << "BLACKOUT DETECTED!! KEEPING MASK DOWN!!\n";
		//mask is STILL down! blackout detected!
		//sleep for the duration of the blackout, pull mask up 0.75 seconds after it ends
		std::this_thread::sleep_for(std::chrono::milliseconds(4575));

		toggleMask(); //pull mask up again
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //make sure mask is up before next action
		return true; //return true because a blackout happened
	}
	std::cout << "No blackout detected! Mask lifted up successfully!\n";
	// no blackout, do nothing!
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); //make sure mask is up before next action
	return false; //return false, meaning no blackout happened
}

int checkRightVent() //call this fn if looking RIGHT
{
	// return values: 0 = no animatronics, 1 = toy bonnie, 2 = Mangle

	// Right Light Physical: X=1072,Y=440
	// ToyBonnie's Eye (Greenish): RGB at (1158, 624): R = 19, G = 118, B = 44
	// Mangle's Snout (Pinkish): RGB at (1174, 673): R = 95, G = 43, B = 85

	SetCursorPos(1072, 440); //set cursor on right vent light button
	holdLeftMouseButton(150); //hold it down for short duration (turns on light fast)

	//immediately get rgb values before the light turns off!
	HDC hdcScreen = GetDC(NULL);
	COLORREF color1 = GetPixel(hdcScreen, 1158, 624); //get pixel at toybonnie eyeball location
	COLORREF color2 = GetPixel(hdcScreen, 1174, 673); //get pixel at mangle snout location

	int red1 = GetRValue(color1);
	int green1 = GetGValue(color1);
	int blue1 = GetBValue(color1);

	int red2 = GetRValue(color2);
	int green2 = GetGValue(color2);
	int blue2 = GetBValue(color2);

	std::cout << "Toy Bonnie detection: If R = 0, G = 0, and B = 0, then Toy Bonnie is detected!!\n";
	std::cout << "R: " << red1 << ", ";
	std::cout << "G: " << green1 << ", ";
	std::cout << "B: " << blue1 << "\n";

	std::cout << "Mangle detection: If R = 0, G = 0, and B = 0, then Mangle is detected!!\n";
	std::cout << "R: " << red2 << ", ";
	std::cout << "G: " << green2 << ", ";
	std::cout << "B: " << blue2 << "\n";

	int animatronicInVent = 0; //return value to tell who is in vent or if there is nobody there

	if ((green1 > blue1) && red1 > 0)
	{
		std::cout << "TOY BONNIE DETECTED!!\n";
		animatronicInVent = 1; //toy bonnie detected
	}
	else if ((red2 > blue2 && red2 > green2) && (blue2 > green2))
	{
		std::cout << "MANGLE DETECTED!!\n";
		animatronicInVent = 2; //mangle detected
	}
	else
	{
		std::cout << "No animatronics detected!\n";
		//no animatronics detected
	}


	ReleaseDC(NULL, hdcScreen);

	return animatronicInVent;
}


int checkLeftVent() //call this fn if looking LEFT
{
	// return values: 0 = no animatronics, 1 = Toy Chica, 2 = Balloon Boy

	// Left Light Physical: X=194,Y=448
	//Shadow of Toy Chica/Balloon Boy: 203, 698


	SetCursorPos(194, 448); //set cursor on left vent light button
	holdLeftMouseButton(150); //hold it down for short duration (turns on light fast)

	//immediately get rgb values before the light turns off!
	HDC hdcScreen = GetDC(NULL);
	COLORREF color = GetPixel(hdcScreen, 203, 698); //get pixel at balloon boy or toy chica shadow

	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	std::cout <<
		"Toy Chica/Balloon Boy detection: If R = 79, G = 93, B = 0, then Toy Chica/Balloon Bitch is detected!!\n";
	std::cout << "R: " << red << ", ";
	std::cout << "G: " << green << ", ";
	std::cout << "B: " << blue << "\n";

	int animatronicInVent = 0; //return value to tell who is in vent or if there is nobody there

	if (red == 0 && green == 0 && blue == 0)
	{
		std::cout << "TOY CHICA OR BALLOON BOY DETECTED!!\n";
		animatronicInVent = 1; //toy bonnie detected
	}
	else
	{
		std::cout << "No animatronics detected!\n";
		//no animatronics detected
	}


	ReleaseDC(NULL, hdcScreen);

	return animatronicInVent;
}
