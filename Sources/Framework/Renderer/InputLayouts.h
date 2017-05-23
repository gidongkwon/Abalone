#pragma once

struct InputLayouts
{
	static void init(ID3D11Device* device);
	static void destroy();

	// see VertexTypes.h
	static ID3D11InputLayout* position_normal_tan_tex;
};