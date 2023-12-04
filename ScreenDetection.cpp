#include "ScreenDetection.h"

#include <iostream>
#include "EventLogger.h"

ScreenDetection::ScreenDetection(ImageBuffer& sharedBuffer, std::atomic<bool>& running) : sharedBuffer(sharedBuffer), running(running)
{
	// Init detection stuff
}

void ScreenDetection::detectionLoop()
{
	while (running)
	{
		if (sharedBuffer.getLatestFrame())
		{
			// Do detection stuff
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EventLogger::getInstance().logEvent("Detecting loop");
		//std::cout << "Detection loop" << std::endl;
	}

	// Release detection stuff
}