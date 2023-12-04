#include "ScreenDetection.h"

#include "EventLogger.h"

ScreenDetection::ScreenDetection(ImageBuffer& sharedImageBuffer, std::atomic<bool>& running) : sharedImageBuffer(sharedImageBuffer), running(running)
{
	// Init detection stuff
}

void ScreenDetection::detection_loop()
{
	while (running)
	{
		if (sharedImageBuffer.getLatestFrame())
		{
			// Do detection stuff
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		EventLogger::getInstance().logEvent("Detecting loop");		
	}

	// Release detection stuff
}