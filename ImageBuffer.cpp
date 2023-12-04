#include "ImageBuffer.h"

ImageBuffer::ImageBuffer()
{
}

void ImageBuffer::updateFrame()
{
	std::unique_lock<std::mutex> lock(bufferMutex);
	newFrameAvailable = true;
	bufferCondition.notify_one();
}

bool ImageBuffer::getLatestFrame()
{
	std::unique_lock<std::mutex> lock(bufferMutex);
	bufferCondition.wait(lock, [this]() { return newFrameAvailable; });
	if (newFrameAvailable)
	{
		newFrameAvailable = false;
		return true;
	}
	return false;
}