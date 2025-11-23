#include <Windows.h>
#include <iostream>

void cloneLauncher()
{
	std::cout << R"(
If you want the bot to auto start a timer (optional), do the following:
    1) Download LiveSplit game timer
    2) Open the timer, right click on it, click on settings
    3) Set Start/Split to Up arrow on keyboard (Up), then press OK
    4) Move LiveSplit timer to top left of screen
    5) Ready to run bot

)";

	std::cout <<
		"Right-click to start Jason Clone. (DO IT BETWEEN WHEN YOU CLICKED THE NIGHT & WHEN THE CLOCK APPEARS!)\n";

	// Wait for right-click press
	while (!(GetAsyncKeyState(VK_RBUTTON) & 0x8001))
		Sleep(100);

	// Wait for right-click release
	while (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
		Sleep(100);

	SetCursorPos(229, 364); //

	// Test the locateOnScreen function

	HDC hdcScreen = GetDC(NULL);
	while (true)
	{
		COLORREF color = GetPixel(hdcScreen, 1223, 752); //clock coords

		int red = GetRValue(color);
		int green = GetGValue(color);
		int blue = GetBValue(color);

		bool found = (red == 255 && green == 255 && blue == 255);

		if (found)
		{
			break;
		}
	}

	SetCursorPos(229, 64); //
	std::cout << "--------------------------------\n";
	std::cout << "Game loading...\n";
	std::cout << "--------------------------------\n";

	while (true)
	{
		COLORREF color = GetPixel(hdcScreen, 1223, 752); //clock coords

		int red = GetRValue(color);
		int green = GetGValue(color);
		int blue = GetBValue(color);

		bool found = (red == 255 && green == 255 && blue == 255);

		if (!found)
		{
			break;
		}
	}

	SetCursorPos(229, 264); //

	std::cout << "--------------------------------\n";
	std::cout << "Game started!!\n";
	std::cout << "--------------------------------\n";
}
