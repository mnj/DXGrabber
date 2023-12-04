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

	WebStatus web_status(sharedBuffer, running);
	ScreenCapture capture(sharedBuffer, running);
	ScreenDetection detection(sharedBuffer, running);	

	std::thread web_status_thread(&WebStatus::run_server, &web_status);
	std::thread capture_thread(&ScreenCapture::capture_loop, &capture);
	std::thread detection_thread(&ScreenDetection::detection_loop, &detection);	

	std::cout << "Press enter to stop" << std::endl;
	std::cin.get();

	// Signal the capture/detection/web status loop to stop
	running = false;

	// Wait for the capture/detection/web status loop to finish
	if (capture_thread.joinable())
	{
		capture_thread.join();
	}

	if (detection_thread.joinable())
	{
		detection_thread.join();
	}

	if (web_status_thread.joinable())
	{
		web_status_thread.join();
	}

	std::cout << "Application exiting..." << std::endl;
	return 0;
}
