#pragma once

struct RenderStates
{
	static void init(ID3D11Device* device);
	static void destroy();

	static ID3D11RasterizerState* solid_rs;
	static ID3D11RasterizerState* wireframe_rs;
};