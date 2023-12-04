#pragma once

#include <mutex>
#include <condition_variable>

class ImageBuffer
{
private:
	std::mutex bufferMutex;
	std::condition_variable bufferCondition;
	bool newFrameAvailable = false;
public:
	ImageBuffer();
	void updateFrame();
	bool getLatestFrame();
};
