#pragma once

#include <atomic>
#include "ImageBuffer.h"

#include <dxgi.h>
#include <wrl.h>
#include <d3d11.h>
#include <dxgi1_2.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class ScreenCapture
{
private:
	ImageBuffer& sharedImageBuffer;
	std::atomic<bool>& running;

	// DX11/12 stuff
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;
	ComPtr<IDXGIFactory1> DXGIFactory;
	ComPtr<IDXGIAdapter1> Adapter;
	ComPtr<IDXGIOutput> Output;
	ComPtr<IDXGIOutput1> Output1;
	ComPtr<IDXGIOutputDuplication> DesktopDuplication;
	
public:
	ScreenCapture(ImageBuffer& sharedImageBuffer, std::atomic<bool>& running);
	void capture_loop();
	bool InitializeDirectX();
};
