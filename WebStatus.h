#pragma once

#include <atomic>
#include "ImageBuffer.h"

class WebStatus
{
private:
	ImageBuffer& sharedBuffer;
	std::atomic<bool>& running;
	std::string html_index;
public:
	WebStatus(ImageBuffer& sharedBuffer, std::atomic<bool>& running);
	void run_server() const;
	void setup_routes() const;
};

