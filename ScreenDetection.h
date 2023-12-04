#pragma once

#include <atomic>
#include "ImageBuffer.h"

class ScreenDetection
{
private:
	ImageBuffer& sharedBuffer;
	std::atomic<bool>& running;
public:
	ScreenDetection(ImageBuffer& sharedBuffer, std::atomic<bool>& running);
	void detectionLoop();
};

