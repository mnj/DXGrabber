#include "WebStatus.h"

#include <drogon/drogon.h>
#include <atomic>
#include <fstream>

#include "EventLogger.h"

std::string read_html_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	if (!file)
	{
		throw std::runtime_error("Failed to open file: " + filename);
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	return content;
}

WebStatus::WebStatus(ImageBuffer& sharedBuffer, std::atomic<bool>& running) : sharedBuffer(sharedBuffer), running(running)
{
	// Init web stuff

	// Load the index page
	html_index = read_html_file("Index.html");
}

void WebStatus::setup_routes()
{
	drogon::app().registerHandler("/",
		[this](const drogon::HttpRequestPtr& req,
		       std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

				const auto resp = drogon::HttpResponse::newHttpResponse();
				resp->setBody(html_index);
				callback(resp);

			/*auto events = EventLogger::getInstance().getAllEvents();

				std::string responseBody = "<html><body><h1>Web Status</h1><p>Latest Events:</p><ul>";
				for (auto& event : events)
				{
					responseBody += "<li>" + event + "</li>";
				}
				responseBody += "</ul></body></html>";

				auto resp = drogon::HttpResponse::newHttpResponse();
				resp->setBody(responseBody);
				// reload every 1 second
				resp->addHeader("Refresh", "1");
				callback(resp);*/
		});

	drogon::app().registerHandler("/api/events",
		[this](const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

				auto events = EventLogger::getInstance().getAllEvents();
				Json::Value jsonArray;
				for(const auto& event : events)
				{
					Json::Value jsonEvent;
					jsonEvent["timestamp"] = event.timestamp;
					jsonEvent["description"] = event.description;
					jsonArray.append(jsonEvent);
				}

				const auto resp = drogon::HttpResponse::newHttpResponse();
				resp->setContentTypeCode(drogon::ContentType::CT_APPLICATION_JSON);
				resp->setBody(jsonArray.toStyledString());
				callback(resp);
		});
}

void WebStatus::run_server()
{
	setup_routes();

	drogon::app().setThreadNum(1);

	drogon::app().addListener("0.0.0.0", 8888);
	drogon::app().addListener("::", 8888);
	std::cout << "Web server is listening on http://localhost:8888" << std::endl;	


	auto timerIdPtr = std::make_shared<trantor::TimerId>();

	auto loop = drogon::app().getLoop();
	*timerIdPtr = loop->runEvery(std::chrono::milliseconds(500), [this, loop, timerIdPtr]() {
		if (!running.load())
		{
			loop->invalidateTimer(*timerIdPtr);
			drogon::app().quit();
		}
	});

	drogon::app().run();
}
