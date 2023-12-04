#pragma once

#include <atomic>
#include "ImageBuffer.h"

class ScreenCapture
{
private:
	ImageBuffer& sharedImageBuffer;
	std::atomic<bool>& running;

	// DX11/12 stuff

public:
	ScreenCapture(ImageBuffer& sharedImageBuffer, std::atomic<bool>& running);
	void capture_loop();
};
