#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <Windows.h>
#include "MasonTools.h"
#include "StartingPhase.h"
#include "CloneVision.h"
#include "CloneActions.h"


std::mutex mutex_;

bool timeReachedNinePointFive = false; //time to drop mask for toy bonnie
bool timeReachedOnePointFive = false;
bool timeReachedFivePointFive = false;


//timer
void checkElapsedTime(const std::chrono::time_point<std::chrono::high_resolution_clock>& start_time)
{
	while (true)
	{
		// Get the current time
		auto current_time = std::chrono::high_resolution_clock::now();

		// Calculate the elapsed time since the start in seconds (as a double)
		auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time).
			count();

		// Round the elapsed time to one decimal place
		elapsed_seconds = std::round(elapsed_seconds * 10.0) / 10.0;

		// Check if elapsed time ends in 1.5 seconds or 5.5 or 1.4 seconds
		if (std::fmod(elapsed_seconds, 10.0) >= 1.5 && std::fmod(elapsed_seconds, 10.0) <= 5.8)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			timeReachedOnePointFive = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			// delay to prevent duplicate time detection
		}
		else if (std::fmod(elapsed_seconds, 10.0) >= 5.5 && std::fmod(elapsed_seconds, 10.0) <= 6.0)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			timeReachedFivePointFive = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			// delay to prevent duplicate time detection
		}
		else if (std::fmod(elapsed_seconds, 10.0) >= 9.3 && std::fmod(elapsed_seconds, 10.0) <= 9.9)
		{
			std::unique_lock<std::mutex> lock(mutex_);
			timeReachedNinePointFive = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			// delay to prevent duplicate time detection
		}

		// Print the elapsed time in seconds (rounded to one decimal place)
		std::cout << "Elapsed time: " << elapsed_seconds << " seconds" << "\n";

		// Sleep for a short interval (adjust as needed)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


bool isFivePointFive()
{
	// Check if the time ending in 5.5 seconds is reached
	std::unique_lock<std::mutex> lock(mutex_);

	if (timeReachedFivePointFive)
	{
		std::cout << "IsFivePointFive(): Time ending in 5.5 reached!" << "\n";
		timeReachedFivePointFive = false; //reset for future calls
		return true;
	}

	return false;
}

bool isOnePointFive()
{
	// Check if the time ending in 1.8 seconds is reached
	std::unique_lock<std::mutex> lock(mutex_);
	if (timeReachedOnePointFive)
	{
		std::cout << "isOnePointFive(): Time ending in 1.8 reached!" << "\n";
		timeReachedOnePointFive = false; //reset for future calls
		return true;
	}
	return false;
}

bool isNinePointFive() //for toy bonnie
{
	// Check if the time ending in 3.5 seconds is reached
	std::unique_lock<std::mutex> lock(mutex_);
	if (timeReachedNinePointFive)
	{
		std::cout << "isNinePointFive(): Time ending in 3.5 reached!(DROP MASK FOR TOY BONNIE)" << "\n";
		timeReachedNinePointFive = false;
		return true;
	}

	return false;
}


void cloneVentCheck(bool blackoutJustOccurred, int flashInterval, int shortStrobeCount, int longStrobeCount)
//call fn when u are looking RIGHT and no mask on!
{
	if (blackoutJustOccurred)
	{
		for (int i = 0; i < shortStrobeCount; ++i) //(short strobe) ***set to 2 for 10/20!!!!!
		{
			foxyFlash();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			// delay between each flash *AND before checking right vent!!!!
		}
		const int whoThereRV = checkRightVent();
		if (whoThereRV == 1) // (***We dont need to watch out for mangle in this case***)
		{
			//wait until 5.5 to drop mask for bonnie blackout

			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************


			toggleMask();
			blackoutFromToyBonnie();

			int attempts = 0;
			while (true)
			{
				//attempt to get mask up over and over until it works

				toggleMask(); //attempt to pull up mask

				std::cout << "Mask attempt #" << attempts << " to be pulled up\n";

				std::this_thread::sleep_for(std::chrono::milliseconds(350));
				//delay to detect possible mask up pixel


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
					std::cout << "TOY BONNIE BLACKOUT STILL HAPPENING!\n";
					//mask is STILL down! blackout detected!
				}
				else
				{
					if (attempts > 0)
					{
						std::cout << "MASK FINALLY LIFTED FROM TOY BONNIE's RUDE BLACKOUT!!!\n";
					}
					break;
				}
				attempts++;
			}
		}
		else
		{
			for (int i = 0; i < shortStrobeCount; ++i) //(short strobe) ***set to 2 for 10/20!!!!!
			{
				foxyFlash();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				// delay between each flash *AND before checking right vent!!!!
			}
			return;
		}
		for (int i = 0; i < shortStrobeCount; ++i) //(short strobe) ***set to 2 for 10/20!!!!!
		{
			foxyFlash();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			// delay between each flash *AND before checking right vent!!!!
		}
		return; //regardless leave fn at this point, whether toy bonnie has been detected or not
	}
	//*************************************************************************************************************************
	//otherwise if blackout did not occur:


	//check right vent

	const int whoThereRV = checkRightVent();


	if (whoThereRV == 1)
	{
		//SAME CODE AS TOY BONNIE CHECK FOR THE "BLACKOUTJUSTOCCURED" CASE!

		//wait until 5.5 to drop mask for bonnie blackout

		//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
		// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
		//**************************************************************************
		timeReachedNinePointFive = false;
		timeReachedOnePointFive = false;
		timeReachedFivePointFive = false;
		//**************************************************************************
		for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
		{
			foxyFlash();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			// delay between each flash *AND before checking right vent!!!!
		}

		toggleMask();
		blackoutFromToyBonnie();

		int attempts = 0;
		while (true)
		{
			//attempt to get mask up over and over until it works

			toggleMask(); //attempt to pull up mask

			std::cout << "Mask attempt #" << attempts << " to be pulled up\n";

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
				std::cout << "TOY BONNIE BLACKOUT STILL HAPPENING!\n";
				//mask is STILL down! blackout detected!
			}
			else
			{
				if (attempts > 0)
				{
					std::cout << "MASK FINALLY LIFTED FROM TOY BONNIE's RUDE BLACKOUT!!!\n";
				}
				break;
			}
			attempts++;
		}
	}
	else if (whoThereRV == 2)
	{
		for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
		{
			foxyFlash();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			// delay between each flash *AND before checking right vent!!!!
		}
		//will toggle mask if left or right vent occupied
		toggleMask(); //just put on mask, wait for 5100 ms to pass, then pull it up!

		std::this_thread::sleep_for(std::chrono::milliseconds(4800));


		// WE NEED TO SEE IF THE TOGGLE MASK FAILED BECAUSE OF TOY BONNIE ENTERING DURING MASK DOWN!!!
		int attempts = 0;
		while (true)
		{
			//attempt to get mask up over and over until it works

			toggleMask(); //attempt to pull up mask

			std::cout << "Mask attempt #" << attempts << " to be pulled up\n";

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
				std::cout << "BLACKOUT DETECTED!! KEEPING MASK DOWN!!\n";
				//mask is STILL down! blackout detected!
			}
			else
			{
				if (attempts > 0)
				{
					std::cout << "MASK FINALLY LIFTED FROM TOY BONNIE's RUDE BLACKOUT!!!\n";
				}
				break;
			}
			attempts++;
		}
	}
	else //nobody in right vent
	{
		lookLeft(flashInterval); //look left and check left vent
		bool leftVentOccupied = checkLeftVent();

		if (leftVentOccupied)
		{
			for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
			{
				foxyFlash();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				// delay between each flash *AND before checking right vent!!!!
			}
			toggleMask();
			lookRight(flashInterval);
			std::this_thread::sleep_for(std::chrono::milliseconds(4000));
			//wait a shorter duration then check if a toy bonnie blackout came in

			// WE NEED TO SEE IF THE TOGGLE MASK FAILED BECAUSE OF TOY BONNIE ENTERING DURING MASK DOWN!!!
			int attempts = 0;
			while (true)
			{
				//attempt to get mask up over and over until it works

				toggleMask(); //attempt to pull up mask

				std::cout << "Mask attempt #" << attempts << " to be pulled up\n";

				std::this_thread::sleep_for(std::chrono::milliseconds(350));
				//delay to detect possible mask up pixel


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
					std::cout << "BLACKOUT DETECTED!! KEEPING MASK DOWN!!\n";
					//mask is STILL down! blackout detected!
				}
				else
				{
					if (attempts > 0)
					{
						std::cout << "MASK FINALLY LIFTED FROM TOY BONNIE's RUDE BLACKOUT!!!\n";
					}
					break;
				}
				attempts++;
			}
			for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
			{
				foxyFlash();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				// delay between each flash *AND before checking right vent!!!!
			}

			const int whoThereRV = checkRightVent();
			//check right vent AGAIN!! Bonnie could have slipped into the vent!

			if (whoThereRV == 1)
			{
				//SAME CODE AS TOY BONNIE CHECK FOR THE "BLACKOUTJUSTOCCURED" CASE!

				//wait until 5.5 to drop mask for bonnie blackout

				//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
				// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
				//**************************************************************************
				timeReachedNinePointFive = false;
				timeReachedOnePointFive = false;
				timeReachedFivePointFive = false;
				//**************************************************************************

				toggleMask();
				blackoutFromToyBonnie();

				int attempts = 0;
				while (true)
				{
					//attempt to get mask up over and over until it works

					toggleMask(); //attempt to pull up mask

					std::cout << "Mask attempt #" << attempts << " to be pulled up\n";

					std::this_thread::sleep_for(std::chrono::milliseconds(350));
					//delay to detect possible mask up pixel


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
						std::cout << "TOY BONNIE BLACKOUT STILL HAPPENING!\n";
						//mask is STILL down! blackout detected!
					}
					else
					{
						if (attempts > 0)
						{
							std::cout << "MASK FINALLY LIFTED FROM TOY BONNIE's RUDE BLACKOUT!!!\n";
						}
						break;
					}
					attempts++;
				}
				for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
				{
					foxyFlash();
					std::this_thread::sleep_for(std::chrono::milliseconds(5));
					// delay between each flash *AND before checking right vent!!!!
				}
			}

			return;
		}
		//otherwise left vent not occupied but mangle is in right vent
		bool mangleInRightVent = (whoThereRV == 2);

		lookRight(flashInterval); //look right no matter if mangle is going to be in right vent or not

		if (mangleInRightVent)
		{
			for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
			{
				foxyFlash();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				// delay between each flash *AND before checking right vent!!!!
			}
			//will toggle mask if left or right vent occupied
			toggleMask(); //just put on mask, wait for 5100 ms to pass, then pull it up!
			std::this_thread::sleep_for(std::chrono::milliseconds(4800));


			// WE NEED TO SEE IF THE TOGGLE MASK FAILED BECAUSE OF TOY BONNIE ENTERING DURING MASK DOWN!!!
			int attempts = 0;
			while (true)
			{
				//attempt to get mask up over and over until it works

				toggleMask(); //attempt to pull up mask

				std::cout << "Mask attempt #" << attempts << " to be pulled up\n";

				std::this_thread::sleep_for(std::chrono::milliseconds(350));
				//delay to detect possible mask up pixel


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
					std::cout << "BLACKOUT DETECTED!! KEEPING MASK DOWN!!\n";
					//mask is STILL down! blackout detected!
				}
				else
				{
					if (attempts > 0)
					{
						std::cout << "MASK FINALLY LIFTED FROM TOY BONNIE's RUDE BLACKOUT!!!\n";
					}
					break;
				}
				attempts++;
			}
		}
	}
	for (int i = 0; i < longStrobeCount; ++i) //(long strobe) ***set to 7 for 10/20!!!!!!
	{
		foxyFlash();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		// delay between each flash *AND before checking right vent!!!!
	}
}


class night1To5
{
public:
	night1To5()
	{
		cloneLauncher();

		//start timer
		auto start_time = std::chrono::high_resolution_clock::now();

		// Start the background timer thread
		std::thread background_thread(checkElapsedTime, start_time);

		//activate timer (move to top left of screen) (make sure to close timer after program ends before starting another one)
		SetCursorPos(50, 25); //move cursor to timer
		simulateLeftClick(); //click on timer
		simulateUpArrowKeyPress(); //start timer
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		SetCursorPos(1000, 50); //put cursor off timer
		simulateLeftClick(); //click off timer to have clicks be in game again
		std::this_thread::sleep_for(std::chrono::milliseconds(50));


		//beginning of bot

		//wind music box controls
		// Input flags
		INPUT input_down = {0};
		input_down.type = INPUT_MOUSE;
		input_down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		INPUT input_up = {0};
		input_up.type = INPUT_MOUSE;
		input_up.mi.dwFlags = MOUSEEVENTF_LEFTUP;


		//main code


		lookRight(30);
		std::cout << "OPENING CAMERA....\n";
		openCamAndSetToPrizeCorner(); //open cam immediately because early game

		// Simulate hold mouse button down (begin winding)
		SendInput(1, &input_down, sizeof(INPUT));
		while (!timeReachedFivePointFive) //changed
		{
			//wait
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		// Simulate mouse button up
		SendInput(1, &input_up, sizeof(INPUT)); //stop winding

		std::cout << "DROPPING CAM\n";
		toggleCam();
		toggleMask(); //mask down from cam
		std::this_thread::sleep_for(std::chrono::milliseconds(75));
		toggleMask(); //mask up (because it is first time)


		int x = 0;
		while (true)
		{
			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************

			int checks = 0;
			//sleep to prevent too many vent checks due to less flashing
			while (!timeReachedOnePointFive)
			//changed!!!!!! //(check both vents once and use slower strobe to kill time until 1.8 to wind)
			{
				if (timeReachedNinePointFive)
				{
					cloneVentCheck(false, 30, 1, 1); //less flash for night 6
					timeReachedNinePointFive = false; //reset value for thread to decide again
					checks++;
					continue;
				}
				timeReachedOnePointFive = false; //reset value for thread to decide again
				if (checks % 5 == 0) //adjust to reduce battery usage
				{
					//change back to no modulus if NEEDED
					foxyFlash();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				checks++;
			}

			int waitUntilIntervalCount = 0;
			timeReachedOnePointFive = false; //reset value for thread to decide again
			while (!timeReachedOnePointFive)
			//force wait until next interval to pull up cams (in case of cloneventcheck
			// detecting someone during the flash loop
			{
				if (waitUntilIntervalCount % 60 == 0) //adjust to reduce battery usage
				{
					foxyFlash();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				waitUntilIntervalCount++;
			}
			foxyFlash();
			std::cout << "OPENING CAMERA....\n";
			toggleCam();

			SetCursorPos(541, 609); //set cursor to music box for winding


			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************


			int y = 0;
			// Simulate hold mouse button down (begin winding)
			SendInput(1, &input_down, sizeof(INPUT));
			while (!timeReachedFivePointFive) //changed!!!!!!!!
			{
				timeReachedFivePointFive = false; //reset value for thread to decide again
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (y < 3)
				{
					foxyFlash();
				}
				y++;
			}
			// Simulate mouse button up
			SendInput(1, &input_up, sizeof(INPUT)); //stop winding

			toggleCam();
			toggleMask();
			bool blackoutOccurred = checkBlackoutAfterCam();

			//flash foxy and check right vent

			cloneVentCheck(blackoutOccurred, 30, 1, 1); //less flash for night 6

			x++;
		}

		//end of test
		SetCursorPos(100, 300);

		// Join the background thread (wait for it to finish)
		background_thread.join();
	}
};

//DIVIDER----------------------------------------

class night6
{
public:
	night6()
	{
		cloneLauncher();

		//start timer
		auto start_time = std::chrono::high_resolution_clock::now();

		// Start the background timer thread
		std::thread background_thread(checkElapsedTime, start_time);

		//activate timer (move to top left of screen) (make sure to close timer after program ends before starting another one)
		SetCursorPos(50, 25); //move cursor to timer
		simulateLeftClick(); //click on timer
		simulateUpArrowKeyPress(); //start timer
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		SetCursorPos(1000, 50); //put cursor off timer
		simulateLeftClick(); //click off timer to have clicks be in game again
		std::this_thread::sleep_for(std::chrono::milliseconds(50));


		//beginning of bot

		//wind music box controls
		// Input flags
		INPUT input_down = {0};
		input_down.type = INPUT_MOUSE;
		input_down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		INPUT input_up = {0};
		input_up.type = INPUT_MOUSE;
		input_up.mi.dwFlags = MOUSEEVENTF_LEFTUP;


		//main code


		lookRight(30);
		std::cout << "OPENING CAMERA....\n";
		openCamAndSetToPrizeCorner(); //open cam immediately because early game

		// Simulate hold mouse button down (begin winding)
		SendInput(1, &input_down, sizeof(INPUT));
		while (!timeReachedFivePointFive) //changed
		{
			//wait
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		// Simulate mouse button up
		SendInput(1, &input_up, sizeof(INPUT)); //stop winding

		std::cout << "DROPPING CAM\n";
		toggleCam();
		toggleMask(); //mask down from cam
		std::this_thread::sleep_for(std::chrono::milliseconds(75));
		toggleMask(); //mask up (because it is first time)


		int x = 0;
		while (true)
		{
			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************

			int checks = 0;
			//sleep to prevent too many vent checks due to less flashing
			while (!timeReachedOnePointFive)
			//changed!!!!!! //(check both vents once and use slower strobe to kill time until 1.8 to wind)
			{
				if (timeReachedNinePointFive)
				{
					cloneVentCheck(false, 30, 1, 1); //less flash for night 6
					timeReachedNinePointFive = false; //reset value for thread to decide again
					checks++;
					continue;
				}
				timeReachedOnePointFive = false; //reset value for thread to decide again
				if (checks % 3 == 0)
				{
					//change back to no modulus if NEEDED
					foxyFlash();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				checks++;
			}

			int waitUntilIntervalCount = 0;
			timeReachedOnePointFive = false; //reset value for thread to decide again
			while (!timeReachedOnePointFive)
			//force wait until next interval to pull up cams (in case of cloneventcheck
			// detecting someone during the flash loop
			{
				if (waitUntilIntervalCount % 60 == 0)
				{
					foxyFlash();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				waitUntilIntervalCount++;
			}
			foxyFlash();
			std::cout << "OPENING CAMERA....\n";
			toggleCam();

			SetCursorPos(541, 609); //set cursor to music box for winding


			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************


			int y = 0;
			// Simulate hold mouse button down (begin winding)
			SendInput(1, &input_down, sizeof(INPUT));
			while (!timeReachedFivePointFive) //changed!!!!!!!!
			{
				timeReachedFivePointFive = false; //reset value for thread to decide again
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (y < 3)
				{
					foxyFlash();
				}
				y++;
			}
			// Simulate mouse button up
			SendInput(1, &input_up, sizeof(INPUT)); //stop winding

			toggleCam();
			toggleMask();
			bool blackoutOccurred = checkBlackoutAfterCam();

			//flash foxy and check right vent

			cloneVentCheck(blackoutOccurred, 30, 1, 1); //less flash for night 6

			x++;
		}

		//end of test
		SetCursorPos(100, 300);

		// Join the background thread (wait for it to finish)
		background_thread.join();
	}
};


//***************************************BEST BOT******************************************
class ten20Mode
{
public:
	ten20Mode()
	{
		cloneLauncher();

		//start timer
		auto start_time = std::chrono::high_resolution_clock::now();

		// Start the background timer thread
		std::thread background_thread(checkElapsedTime, start_time);

		//activate timer (move to top left of screen) (make sure to close timer after program ends before starting another one)
		SetCursorPos(50, 25); //move cursor to timer
		simulateLeftClick(); //click on timer
		simulateUpArrowKeyPress(); //start timer
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		SetCursorPos(1000, 50); //put cursor off timer
		simulateLeftClick(); //click off timer to have clicks be in game again
		std::this_thread::sleep_for(std::chrono::milliseconds(50));


		//beginning of bot


		//wind music box controls
		// Input flags
		INPUT input_down = {0};
		input_down.type = INPUT_MOUSE;
		input_down.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

		INPUT input_up = {0};
		input_up.type = INPUT_MOUSE;
		input_up.mi.dwFlags = MOUSEEVENTF_LEFTUP;


		//main code


		lookRight(6);
		std::cout << "OPENING CAMERA....\n";
		openCamAndSetToPrizeCorner(); //open cam immediately because early game

		// Simulate hold mouse button down (begin winding)
		SendInput(1, &input_down, sizeof(INPUT));
		while (!timeReachedFivePointFive) //changed
		{
			//wait
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		// Simulate mouse button up
		SendInput(1, &input_up, sizeof(INPUT)); //stop winding

		std::cout << "DROPPING CAM\n";
		toggleCam();
		toggleMask(); //mask down from cam
		std::this_thread::sleep_for(std::chrono::milliseconds(75));
		toggleMask(); //mask up (because it is first time)


		int x = 0;
		while (true)
		{
			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************

			int checks = 0;
			while (!timeReachedOnePointFive)
			//changed!!!!!! //(check both vents once and use slower strobe to kill time until 1.8 to wind)
			{
				if (timeReachedNinePointFive)
				{
					cloneVentCheck(false, 6, 2, 7); //was 6,2,7
					timeReachedNinePointFive = false; //reset value for thread to decide again
					checks++;
					continue;
				}
				timeReachedOnePointFive = false; //reset value for thread to decide again
				foxyFlash();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				checks++;
			}

			int waitUntilIntervalCount = 0;
			timeReachedOnePointFive = false; //reset value for thread to decide again
			while (!timeReachedOnePointFive)
			//force wait until next interval to pull up cams (in case of cloneventcheck
			// detecting someone during the flash loop
			{
				if (waitUntilIntervalCount % 10 == 0) //changed from 10
				{
					foxyFlash();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				waitUntilIntervalCount++;
			}
			std::cout << "OPENING CAMERA....\n";
			toggleCam();

			SetCursorPos(541, 609); //set cursor to music box for winding


			//*******************RESET MOTHER NUTTERS!!!!!!!!!!!!!**********************************
			// *************************SO reset value for thread to decide again!!!!!!!!!!!!!!!********************************
			//**************************************************************************
			timeReachedNinePointFive = false;
			timeReachedOnePointFive = false;
			timeReachedFivePointFive = false;
			//**************************************************************************


			int y = 0;
			// Simulate hold mouse button down (begin winding)
			SendInput(1, &input_down, sizeof(INPUT));
			while (!timeReachedFivePointFive) //changed!!!!!!!!
			{
				timeReachedFivePointFive = false; //reset value for thread to decide again
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (y < 5)
				{
					foxyFlash();
				}
				y++;
			}
			// Simulate mouse button up
			SendInput(1, &input_up, sizeof(INPUT)); //stop winding

			toggleCam();
			toggleMask();
			bool blackoutOccurred = checkBlackoutAfterCam();

			//flash foxy and check right vent

			cloneVentCheck(blackoutOccurred, 6, 2, 7); //was 6,2,7

			x++;
		}

		//end of test
		SetCursorPos(100, 300);

		// Join the background thread (wait for it to finish)
		background_thread.join();
	}
};

int main()
{
	//ten20Mode jasonClone;
	//night6 jasonClone;
	//night1To5 jasonClone;

	while (true)
	{
		std::cout << R"(
----------------------------------------
|          FNAF 2 BOT STARTUP          |
----------------------------------------
CHOOSE YOUR NIGHT FOR JASON CLONE TO PLAY FOR YOU:

Input | Mode                                               | Star
-------------------------------------------------------------------
0     | Info/Instructions                                  |
1     | Nights 1-5 (High success rate)                     | *
2     | Night 6   (High success rate)                      | **
3     | Night 7 (10/20 Mode) (MEDIUM - LOW Success rate)   | ***
-------------------------------------------------------------------
Enter here: )";

		int nightChoice = -1;

		std::cin >> nightChoice;

		switch (nightChoice)
		{
		case 0:
			{
				std::cout << R"(
-----------------------------------------------------------------------------------------------
|                               JASON CLONE IMPORTANT INFO                                    |
-----------------------------------------------------------------------------------------------

I)  How does the bot play?
    Plays for you through simulated button/key-clicks.
    DO NOT INTERRUPT Jason Clone when he is playing!!!
    This can result in UNEXPECTED results which can cause you to DIE!

II) How does it know when the game is over?
    This bot does NOT ever stop clicking, even after it dies or wins.
    To force end the bot (if you ever want to use your pc again),
    open task manager using SHIFT + CTRL + ESC then terminate Visual
    Studio from there.

III) Can it beat the custom night challenges other than 10/20?
    I have not tested the other challenges, but if you must use the bot
    for them I would recommend using the 10/20 configuration for ones that
    have extreme foxy AI and many blackouts, use the night 6 for ones with
    medium - hard foxy AI or moderate blackouts, and the night 1-5 for ones
    with medium - low foxy AI or little to no blackouts. This is because the
    flashlight battery usage for the bot is ordered like:
    10/20 > Night 6 > Nights 1-5

IV) Is this compatible for OS other than Windows?
    No. This bot uses a windows exclusive header file, so I doubt
    it will work on Mac/Linux.

V)  The bot got killed!!??
    Be patient and look at the success rate of each option before you choose it.
    This game has something called RNG, and this bot's algorithm reduces the affect
    of it as much as possible.

VI) Can this get me banned?
    Don't worry GoGuardian is screen recording your computer and BattleEye is about to
    hardware ban your device!!! In all seriousness I doubt Scott Cawthon cares enough for
    anti cheat mechanisms on an offline game that was created in 2014. (Use at your own
    risk though)

-----------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------

 )";
				break;
			}
		case 1:
			{
				std::cout << "*Night 1-5 config Selected!!\n";
				night1To5 jasonClone;
				break;
			}
		case 2:
			{
				std::cout << "**Night 6 config Selected!\n";
				night6 jasonClone;
				break;
			}
		case 3:
			{
				std::cout << "***10/20 config Selected!\n";
				ten20Mode jasonClone;
				break;
			}
		default:
			{
				std::cout << "INVALID INPUT! ABORTING!!!!\n";
				exit(69);
			}
		}
	}
}
