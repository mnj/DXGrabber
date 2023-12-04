#pragma once

#include <atomic>
#include "ImageBuffer.h"

class ScreenDetection
{
private:
	ImageBuffer& sharedImageBuffer;
	std::atomic<bool>& running;
public:
	ScreenDetection(ImageBuffer& sharedImageBuffer, std::atomic<bool>& running);
	void detectionLoop();
};

