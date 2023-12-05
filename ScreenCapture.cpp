#include "ScreenCapture.h"

#include <fstream>

#include "EventLogger.h"

ScreenCapture::ScreenCapture(ImageBuffer& sharedImageBuffer, std::atomic<bool>& running) : sharedImageBuffer(sharedImageBuffer), running(running)
{
		
}

void ScreenCapture::capture_loop()
{
	if (!InitializeDirectX())
	{
		EventLogger::getInstance().logEvent("Failed to initialize DirectX");
		return;
	}

	while (running)
	{
		// Capture the primary display
		ComPtr<IDXGIResource> dxgi_resource;
		DXGI_OUTDUPL_FRAME_INFO frame_info = { 0 };

		HRESULT hrAquireNextFrame = DesktopDuplication->AcquireNextFrame(500, &frame_info, &dxgi_resource);
		if (FAILED(hrAquireNextFrame))
		{
			EventLogger::getInstance().logEvent("Failed to acquire next frame");
			continue;
		}

		// We need to get the texture, and store it as a tensor for the detection loop

		HRESULT hrReleaseFrame = DesktopDuplication->ReleaseFrame();
		if (FAILED(hrReleaseFrame))
		{
			EventLogger::getInstance().logEvent("Failed to release frame");
			continue;
		}

		/*
		// Hack Start
		ComPtr<ID3D11Texture2D> DesktopImage;
		DesktopResource.As(&DesktopImage);

		D3D11_TEXTURE2D_DESC desc;
		DesktopImage->GetDesc(&desc);
		desc.BindFlags = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;

		ComPtr<ID3D11Texture2D> stagingTexture;
		Device->CreateTexture2D(&desc, nullptr, &stagingTexture);
		DeviceContext->CopyResource(stagingTexture.Get(), DesktopImage.Get());

		D3D11_MAPPED_SUBRESOURCE mapped_subresource;
		DeviceContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mapped_subresource);

		std::ofstream file("capture.bmp", std::ios::out | std::ios::binary);

		int width = desc.Width;
		int height = desc.Height;
		int rowPitch = mapped_subresource.RowPitch;
		uint8_t* data = reinterpret_cast<uint8_t*>(mapped_subresource.pData);

		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;

		memset(&fileHeader, 0, sizeof(fileHeader));
		memset(&infoHeader, 0, sizeof(infoHeader));

		// Fill fileHeader fields
		fileHeader.bfType = 0x4D42;  // "BM" in hex
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		fileHeader.bfSize = fileHeader.bfOffBits + width * height * 4; // Assuming 4 bytes per pixel

		// Fill infoHeader fields
		infoHeader.biSize = sizeof(BITMAPINFOHEADER);
		infoHeader.biWidth = width;
		infoHeader.biHeight = -height;  // Negative for top-down bitmap
		infoHeader.biPlanes = 1;
		infoHeader.biBitCount = 32;  // Assuming 32-bit bitmap (4 bytes per pixel)
		infoHeader.biCompression = BI_RGB;

		file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
		file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

		// Write pixel data
		for (int y = 0; y < height; ++y) {
			file.write(reinterpret_cast<char*>(data + y * rowPitch), width * 4);
		}

		DeviceContext->Unmap(stagingTexture.Get(), 0);
		file.close();

		// Hack end

		DesktopDuplication->ReleaseFrame();

		// Update DX stuff
		sharedImageBuffer.updateFrame();
		*/


		std::this_thread::sleep_for(std::chrono::milliseconds(25000));		
	}	
}

bool ScreenCapture::InitializeDirectX()
{
	// FIXME: Split this function into smaller functions

	// Create DirectX device	
	constexpr D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	constexpr D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL feature_level_received;
	HRESULT hrCreateDevice = S_OK;

	for (const auto& driver_type : driver_types)
	{
		hrCreateDevice = D3D11CreateDevice(
			nullptr, 
			driver_type, 
			nullptr,
			0, 
			feature_levels,
			static_cast<UINT>(std::size(feature_levels)),			
			D3D11_SDK_VERSION, 
			Device.GetAddressOf(), 
			&feature_level_received, 
			DeviceContext.GetAddressOf()
		);

		if (SUCCEEDED(hrCreateDevice))
		{
			break;
		}
	}
	
	if (FAILED(hrCreateDevice))
	{
		EventLogger::getInstance().logEvent("Failed to create D3D11 device");
		return false;
	}
	EventLogger::getInstance().logEvent("Successfully created D3D11 device");

	// Create DXGI Factory
	HRESULT hrCreateDXGIFactory = CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(DXGIFactory.GetAddressOf()));
	if (FAILED(hrCreateDXGIFactory))
	{
		EventLogger::getInstance().logEvent("Failed to create DXGI factory");
		return false;
	}
	EventLogger::getInstance().logEvent("Successfully created DXGI factory");

	// Enumerate adapters
	HRESULT hrEnumerateAdapters = DXGIFactory->EnumAdapters1(0, Adapter.GetAddressOf());
	if(FAILED(hrEnumerateAdapters))
	{
		EventLogger::getInstance().logEvent("Failed to enumerate adapters");
		return false;
	}
	EventLogger::getInstance().logEvent("Successfully enumerated adapters");

	// Enumerate outputs
	HRESULT hrEnumerateOutputs = Adapter->EnumOutputs(0, Output.GetAddressOf());
	if (FAILED(hrEnumerateOutputs))
	{
		EventLogger::getInstance().logEvent("Failed to enumerate outputs");
		return false;
	}
	EventLogger::getInstance().logEvent("Successfully enumerated outputs");

	// Query for Output1 interface
	HRESULT hrQueryOutput1 = Output.As(&Output1);
	if (FAILED(hrQueryOutput1))
	{
		EventLogger::getInstance().logEvent("Failed to query for IDXGIOutput1 interface");
		return false;
	}
	EventLogger::getInstance().logEvent("Successfully queried for IDXGIOutput1 interface");

	// Create desktop duplication
	HRESULT hrCreateDesktopDuplication = Output1->DuplicateOutput(Device.Get(), DesktopDuplication.GetAddressOf());
	if(FAILED(hrCreateDesktopDuplication))
	{
		EventLogger::getInstance().logEvent("Failed to create desktop duplication");
		return false;
	}
	EventLogger::getInstance().logEvent("Successfully created desktop duplication");

	return true;
}


