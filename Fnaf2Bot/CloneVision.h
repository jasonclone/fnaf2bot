#pragma once


//only use for non rapid checking
void captureWindow(const char* savePath);

bool locateOnScreen(const char* imagePath);

bool locateOnScreenWithParam(const char* imagePath, double threshold);

//rapid checking:
