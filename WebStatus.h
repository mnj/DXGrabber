#pragma once

#include <atomic>
#include "ImageBuffer.h"

class WebStatus
{
private:
	ImageBuffer& sharedBuffer;
	std::atomic<bool>& running;
public:
	WebStatus(ImageBuffer& sharedBuffer, std::atomic<bool>& running);
	void run_server();
	void setup_routes();
};

