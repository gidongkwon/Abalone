#include "pch.h"
#include "DeviceResources.h"

DeviceResources::DeviceResources(Settings & settings):
	settings_(settings),
	inited_(false),
	device(nullptr),
	immediate_context(nullptr),
	swap_chain(nullptr),
	depth_stencil_buffer(nullptr),
	render_target_view(nullptr),
	depth_stencil_view(nullptr),
	debug_device(nullptr)
{
}

DeviceResources::~DeviceResources()
{
	swap_chain->Release();
	depth_stencil_buffer->Release();
	render_target_view->Release();
	depth_stencil_view->Release();

	if (immediate_context)
		immediate_context->ClearState();

	device->Release();
	immediate_context->Release();

#ifdef _DEBUG
	debug_device->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	debug_device->Release();
#endif
}

void DeviceResources::init(HWND h_window)
{
	int width = settings_.width;
	int height = settings_.height;
	uint32_t device_flag = 0;

#ifdef _DEBUG
	device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL feature_level;
	
	throw_if_failed(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		device_flag,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&device,
		&feature_level,
		&immediate_context
	));

	if (feature_level != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBoxA(nullptr, "Direct3D Feature Level 11 Unsupported", nullptr, 0);
		return;
	}

	device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa_4x_quality);
	assert(msaa_4x_quality > 0, "msaa quality should be higher than 0");

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (settings_.enable_vsync)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	if (settings_.enable_msaa_4x)
	{
		swap_chain_desc.SampleDesc.Count = 4;
		swap_chain_desc.SampleDesc.Quality = msaa_4x_quality - 1;
	}
	else
	{
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
	}

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = h_window;
	swap_chain_desc.Windowed = true;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;

	IDXGIDevice* dxgi_device = nullptr;
	throw_if_failed(
		device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_device))
	);

	IDXGIAdapter* dxgi_adapter = nullptr;
	throw_if_failed(
		dxgi_device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgi_adapter))
	);

	IDXGIFactory* dxgi_factory = nullptr;
	throw_if_failed(
		dxgi_adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgi_factory))
	);

	throw_if_failed(
		dxgi_factory->CreateSwapChain(device, &swap_chain_desc, &swap_chain)
	);

	// This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
	throw_if_failed(
		dxgi_factory->MakeWindowAssociation(h_window, DXGI_MWA_NO_ALT_ENTER)
	);

	dxgi_device->Release();
	dxgi_adapter->Release();
	dxgi_factory->Release();

	throw_if_failed(
		device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug_device))
	);

	inited_ = true;

	on_resize(width, height);
}

void DeviceResources::clear()
{
	immediate_context->ClearRenderTargetView(render_target_view, Colors::Black);
	immediate_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeviceResources::present()
{
	if (settings_.enable_vsync)
		swap_chain->Present(1, 0);
	else
		swap_chain->Present(0, 0);
}

void DeviceResources::on_resize(int new_width, int new_height)
{
	assert(inited_, "Call init first");

	release_com(render_target_view);
	release_com(depth_stencil_view);
	release_com(depth_stencil_buffer);

	throw_if_failed(
		swap_chain->ResizeBuffers(1, new_width, new_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0)
	);

	ID3D11Texture2D* back_buffer;
	throw_if_failed(
		swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer))
	);
	throw_if_failed(
		device->CreateRenderTargetView(back_buffer, 0, &render_target_view)
	);
	back_buffer->Release();

	D3D11_TEXTURE2D_DESC depth_stencil_desc;
	depth_stencil_desc.Width = new_width;
	depth_stencil_desc.Height = new_height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (settings_.enable_msaa_4x)
	{
		depth_stencil_desc.SampleDesc.Count = 4;
		depth_stencil_desc.SampleDesc.Quality = msaa_4x_quality - 1;
	}
	else
	{
		depth_stencil_desc.SampleDesc.Count = 1;
		depth_stencil_desc.SampleDesc.Quality = 0;
	}

	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	throw_if_failed(device->CreateTexture2D(&depth_stencil_desc, nullptr, &depth_stencil_buffer));
	throw_if_failed(device->CreateDepthStencilView(depth_stencil_buffer, nullptr, &depth_stencil_view));

	immediate_context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)new_width;
	viewport.Height = (float)new_height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	immediate_context->RSSetViewports(1, &viewport);
}

bool DeviceResources::is_inited() const
{
	return inited_;
}
