#pragma once

#include <chrono>
#include <vector>
#include <mutex>
#include <string>

struct LogEvent
{
	long long timestamp;
	std::string description;
	LogEvent() : timestamp(0), description("") {}
	LogEvent(long long ts, std::string desc) : timestamp(ts), description(std::move(desc)) {}
};

class EventLogger
{
public:
	EventLogger(size_t size) : buffer(size) {}

	void logEvent(const std::string& description) {

		std::lock_guard<std::mutex> lock(mtx);

		auto now = std::chrono::system_clock::now();
		auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

		LogEvent log_event(timestamp, description);

		buffer[head] = std::move(log_event);

		head = (head + 1) % buffer.size();
		if (head == tail) {
			tail = (tail + 1) % buffer.size(); // Overwrite oldest event
		}
	}

	std::vector<LogEvent> getAllEvents() {
		std::lock_guard<std::mutex> lock(mtx);
		std::vector<LogEvent> events;
		size_t current = tail;
		while (current != head) {
			events.push_back(buffer[current]);
			current = (current + 1) % buffer.size();
		}
		return events;
	}

	static EventLogger& getInstance() {
		static EventLogger instance(10);
		return instance;
	}

private:
	std::vector<LogEvent> buffer;
	size_t head = 0;
	size_t tail = 0;
	mutable std::mutex mtx;
};