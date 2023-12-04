#pragma once

#include <atomic>
#include "ImageBuffer.h"

class ScreenCapture
{
private:
	ImageBuffer& sharedBuffer;
	std::atomic<bool>& running;

	// DX11/12 stuff

public:
	ScreenCapture(ImageBuffer& sharedBuffer, std::atomic<bool>& running);
	void captureLoop();
};
