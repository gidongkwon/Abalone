#include "pch.h"
#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::solid_rs = nullptr;
ID3D11RasterizerState* RenderStates::wireframe_rs = nullptr;

void RenderStates::init(ID3D11Device* device)
{
	D3D11_RASTERIZER_DESC solid_desc;
	solid_desc.FillMode = D3D11_FILL_SOLID;
	solid_desc.CullMode = D3D11_CULL_BACK;
	solid_desc.FrontCounterClockwise = false;
	solid_desc.DepthClipEnable = true;
	solid_desc.DepthBias = 0;
	solid_desc.DepthBiasClamp = 0.0f;
	solid_desc.SlopeScaledDepthBias = 0.0f;
	solid_desc.ScissorEnable = false;
	solid_desc.MultisampleEnable = false;
	solid_desc.AntialiasedLineEnable = false;

	throw_if_failed(
		device->CreateRasterizerState(&solid_desc, &solid_rs)
	);

	D3D11_RASTERIZER_DESC wireframe_desc = {};
	wireframe_desc.FillMode = D3D11_FILL_WIREFRAME;
	wireframe_desc.CullMode = D3D11_CULL_NONE;
	wireframe_desc.FrontCounterClockwise = false;
	wireframe_desc.DepthClipEnable = true;

	throw_if_failed(
		device->CreateRasterizerState(&wireframe_desc, &wireframe_rs)
	);
}

void RenderStates::destroy()
{
	solid_rs->Release();
	wireframe_rs->Release();
}
