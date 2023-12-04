#include "WebStatus.h"

#include <drogon/drogon.h>
#include <atomic>

WebStatus::WebStatus(ImageBuffer& sharedBuffer, std::atomic<bool>& running) : sharedBuffer(sharedBuffer), running(running)
{
	// Init web stuff
}

void WebStatus::setup_routes()
{
	drogon::app().registerHandler("/",
		[](const drogon::HttpRequestPtr& req,
			std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
				auto resp = drogon::HttpResponse::newHttpResponse();
				resp->setBody("<p>Web server is running</p>");
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
