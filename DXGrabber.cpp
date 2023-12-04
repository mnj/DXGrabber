#include <thread>
#include <iostream>
#include <ostream>
#include <atomic>

#include "ImageBuffer.h"
#include "ScreenCapture.h"
#include "ScreenDetection.h"
#include "WebStatus.h"

int main()
{
	ImageBuffer sharedBuffer;
	std::atomic<bool> running(true);

	WebStatus webStatus(sharedBuffer, running);
	ScreenCapture capture(sharedBuffer, running);
	ScreenDetection detection(sharedBuffer, running);	

	std::thread webStatusThread(&WebStatus::run_server, &webStatus);
	std::thread captureThread(&ScreenCapture::captureLoop, &capture);
	std::thread detectionThread(&ScreenDetection::detectionLoop, &detection);	

	std::cout << "Press enter to stop" << std::endl;
	std::cin.get();

	// Signal the capture/detection/web status loop to stop
	running = false;

	// Wait for the capture/detection/web status loop to finish
	if (captureThread.joinable())
	{
		captureThread.join();
	}

	if (detectionThread.joinable())
	{
		detectionThread.join();
	}

	if (webStatusThread.joinable())
	{
		webStatusThread.join();
	}

	std::cout << "Application exiting..." << std::endl;
	return 0;
}
