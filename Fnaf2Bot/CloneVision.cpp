#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <Winuser.h>
#include <iostream>

//image recognition (slow)
void captureWindow(const char* savePath)
{
	// Get the screen dimensions
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Create a compatible device context
	HDC hdcScreen = GetDC(nullptr);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
	SelectObject(hdcMem, hbmp);

	// Copy the content of the entire screen to the bitmap
	BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

	// Create a Mat object from the captured screen
	cv::Mat screenshot(screenHeight, screenWidth, CV_8UC4);
	GetBitmapBits(hbmp, screenshot.total() * screenshot.elemSize(), screenshot.data);

	// Save the screenshot to the specified directory
	cv::imwrite(savePath, screenshot);

	// Cleanup
	DeleteObject(hbmp);
	DeleteDC(hdcMem);
	ReleaseDC(nullptr, hdcScreen);

	std::cout << "Screenshot captured and saved to: " << savePath << std::endl;
}


/*
	const char* screenshotName = "path/to/screenshot.png";
	const char* imageName = "path/to/bonnieOutside.png";
 */

bool locateOnScreen(const char* imagePath)
{
	// (Generating unique screenshot name)
	// Generate a timestamp for uniqueness
	auto timestamp = std::chrono::system_clock::now();

	std::time_t now_c = std::chrono::system_clock::to_time_t(timestamp);
	tm timeinfo;
	localtime_s(&timeinfo, &now_c);
	std::ostringstream ss;
	ss << std::put_time(&timeinfo, "%Y%m%d%H%M%S");

	std::string timestampStr = ss.str();

	// Construct the path for saving the screenshot
	std::string savePath = "FNAF_Screenshots/screenshot.png";

	// Capture the current frame
	captureWindow(savePath.c_str());

	// Read the captured screenshot and the template image
	cv::Mat screenshot = cv::imread(savePath);
	cv::Mat imageToSearch = cv::imread(imagePath);

	if (screenshot.empty())
	{
		std::cerr << "Failed to capture the screenshot.\n";
		return false;
	}

	if (imageToSearch.empty())
	{
		std::cerr << "Failed to load the template image: " << imagePath << "\n";
		return false;
	}

	// Print debug information
	std::cout << "Screenshot dimensions: " << screenshot.cols << " x " << screenshot.rows << "\n";
	std::cout << "Template dimensions: " << imageToSearch.cols << " x " << imageToSearch.rows << "\n";

	// Convert images to grayscale
	cv::cvtColor(screenshot, screenshot, cv::COLOR_BGR2GRAY);
	cv::cvtColor(imageToSearch, imageToSearch, cv::COLOR_BGR2GRAY);

	// Perform template matching
	cv::Mat result;
	cv::matchTemplate(screenshot, imageToSearch, result, cv::TM_CCOEFF_NORMED);

	// Define a threshold for matching
	double threshold = 0.6;

	// Find the location of the best match
	cv::Point minLoc, maxLoc;
	double minVal, maxVal;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

	// Print additional debug information
	std::cout << "Max matching value: " << maxVal << std::endl;

	// Check if the match is above the threshold
	return maxVal >= threshold;
}

bool locateOnScreenWithParam(const char* imagePath, double threshold)
{
	// (Generating unique screenshot name)
	// Generate a timestamp for uniqueness
	auto timestamp = std::chrono::system_clock::now();

	std::time_t now_c = std::chrono::system_clock::to_time_t(timestamp);
	tm timeinfo;
	localtime_s(&timeinfo, &now_c);
	std::ostringstream ss;
	ss << std::put_time(&timeinfo, "%Y%m%d%H%M%S");

	std::string timestampStr = ss.str();

	// Construct the path for saving the screenshot
	std::string savePath = "FNAF_Screenshots/screenshot.png";

	// Capture the current frame
	captureWindow(savePath.c_str());

	// Read the captured screenshot and the template image
	cv::Mat screenshot = cv::imread(savePath);
	cv::Mat imageToSearch = cv::imread(imagePath);

	if (screenshot.empty())
	{
		std::cerr << "Failed to capture the screenshot." << "\n";
		return false;
	}

	if (imageToSearch.empty())
	{
		std::cerr << "Failed to load the template image: " << imagePath << "\n";
		return false;
	}

	// Print debug information
	std::cout << "Screenshot dimensions: " << screenshot.cols << " x " << screenshot.rows << "\n";
	std::cout << "Template dimensions: " << imageToSearch.cols << " x " << imageToSearch.rows << "\n";

	// Convert images to grayscale
	cv::cvtColor(screenshot, screenshot, cv::COLOR_BGR2GRAY);
	cv::cvtColor(imageToSearch, imageToSearch, cv::COLOR_BGR2GRAY);

	// Perform template matching
	cv::Mat result;
	cv::matchTemplate(screenshot, imageToSearch, result, cv::TM_CCOEFF_NORMED);

	// Define a threshold for matching
	//(threshold already entered in param)

	// Find the location of the best match
	cv::Point minLoc, maxLoc;
	double minVal, maxVal;
	cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

	// Print additional debug information
	std::cout << "Max matching value: " << maxVal << "\n";

	// Check if the match is above the threshold
	return maxVal >= threshold;
}
