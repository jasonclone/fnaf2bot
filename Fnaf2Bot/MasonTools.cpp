#include <iostream>
#include <Windows.h>


void getRGBOnRightClick()
{
	// Wait for right-click press
	while (!(GetAsyncKeyState(VK_RBUTTON) & 0x8001))
	{
		Sleep(100);
	}

	// Wait for right-click release
	while (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
	{
		Sleep(100);
	}

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	HDC hdcScreen = GetDC(NULL);
	COLORREF color = GetPixel(hdcScreen, cursorPos.x, cursorPos.y);

	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	std::cout << "Right-clicked at (" << cursorPos.x << ", " << cursorPos.y << ") - ";
	std::cout << "R: " << red << ", ";
	std::cout << "G: " << green << ", ";
	std::cout << "B: " << blue << "\n";

	ReleaseDC(NULL, hdcScreen);
}


void setCursorPositionOnRightClick(int x, int y)
{
	while (true)
	{
		// Wait for right-click press
		while (!(GetAsyncKeyState(VK_RBUTTON) & 0x8001))
			Sleep(100);

		// Wait for right-click release
		while (GetAsyncKeyState(VK_RBUTTON) & 0x8001)
			Sleep(100);

		// Set the cursor position
		SetCursorPos(x, y);

		// Get RGB values at the specified position
		HDC hdcScreen = GetDC(NULL);
		COLORREF color = GetPixel(hdcScreen, x, y);
		ReleaseDC(NULL, hdcScreen);

		int red = GetRValue(color);
		int green = GetGValue(color);
		int blue = GetBValue(color);

		// Print RGB values
		std::cout << "RGB at (" << x << ", " << y << "): R = " << red << ", G = " << green << ", B = " << blue <<
			"\n";
	}
}
