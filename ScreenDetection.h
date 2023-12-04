#pragma once

#include <atomic>
#include "ImageBuffer.h"

class ScreenDetection
{
private:
	ImageBuffer& sharedImageBuffer;
	std::atomic<bool>& running;
public:
	ScreenDetection(ImageBuffer& shared_image_buffer, std::atomic<bool>& running);
	void detection_loop();
};

