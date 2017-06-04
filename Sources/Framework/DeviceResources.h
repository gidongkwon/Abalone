#pragma once
#include "Settings.h"

class DeviceResources
{
public:
	DeviceResources(Settings* settings);
	~DeviceResources();

	void init(HWND h_window);
	void clear();
	void present();
	void on_resize(int width, int height);

	bool is_inited() const;

	uint32_t msaa_4x_quality;

	ID3D11Device* device;
	ID3D11DeviceContext* immediate_context;
	IDXGISwapChain* swap_chain;
	ID3D11Texture2D* depth_stencil_buffer;
	ID3D11RenderTargetView* render_target_view;
	ID3D11DepthStencilView* depth_stencil_view;
	D3D11_VIEWPORT viewport;

#ifdef _DEBUG
	ID3D11Debug* debug_device;
#endif
private:
	Settings* settings_;
	bool inited_;
};