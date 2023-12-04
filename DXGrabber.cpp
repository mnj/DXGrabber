#include <thread>
#include <iostream>
#include <ostream>
#include <atomic>

#include "ImageBuffer.h"
#include "ScreenCapture.h"
#include "ScreenDetection.h"

int main()
{
	ImageBuffer sharedBuffer;
	std::atomic<bool> running(true);

	ScreenCapture capture(sharedBuffer, running);
	ScreenDetection detection(sharedBuffer, running);

	std::thread captureThread(&ScreenCapture::captureLoop, &capture);
	std::thread detectionThread(&ScreenDetection::detectionLoop, &detection);

	std::cout << "Press enter to stop" << std::endl;
	std::cin.get();

	// Signal the capture/detection loop to stop
	running = false;

	// Wait for the capture/detection loop to finish
	if (captureThread.joinable())
	{
		captureThread.join();
	}

	if (detectionThread.joinable())
	{
		detectionThread.join();
	}

	std::cout << "Application exiting..." << std::endl;
	return 0;
}
