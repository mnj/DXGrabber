#include "ScreenCapture.h"

#include <iostream>

#include "EventLogger.h"

ScreenCapture::ScreenCapture(ImageBuffer& sharedBuffer, std::atomic<bool>& running) : sharedBuffer(sharedBuffer), running(running)
{
	// Init DX stuff
}

void ScreenCapture::captureLoop()
{
	while (running)
	{
		// Capture screen
		// Update DX stuff
		sharedBuffer.updateFrame();

		// Sleep 25ms
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EventLogger::getInstance().logEvent("Capture loop");
		//std::cout << "Capture loop" << std::endl;
	}

	// Release DX stuff
}


/*
  	// #include <dxgi.h>
	// #include <wrl.h>
	// #include <d3d11.h>
	// #include <iostream>
	// #include <ostream>
	//
	// #pragma comment(lib, "dxgi.lib")
	// #pragma comment(lib, "d3d11.lib")
	//
	// using Microsoft::WRL::ComPtr;

    ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;

	D3D_DRIVER_TYPE DriverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT NumDriverTypes = ARRAYSIZE(DriverTypes);

	D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1,
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

	D3D_FEATURE_LEVEL FeatureLevel;
	HRESULT hr = S_OK;

	for (UINT DriverTypeIndex = 0; DriverTypeIndex < NumDriverTypes; ++DriverTypeIndex)
	{
		hr = D3D11CreateDevice(nullptr, DriverTypes[DriverTypeIndex], nullptr, 0, FeatureLevels, NumFeatureLevels,
			D3D11_SDK_VERSION, &Device, &FeatureLevel, &DeviceContext);

		if (SUCCEEDED(hr))
		{
			std::cout << "Device created successfully" << std::endl;
			break;
		}
	}

	if (FAILED(hr))
	{
		std::cout << "Failed to create device" << std::endl;
	}
*/
