#pragma once

namespace vertex
{
	using namespace DirectX;

	struct Position
	{
		XMFLOAT3 position;

		static const int input_element_count = 1;
		static const D3D11_INPUT_ELEMENT_DESC input_element_desc[input_element_count];
	};

	struct PositionNormal
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;

		static const int input_element_count = 2;
		static const D3D11_INPUT_ELEMENT_DESC input_element_desc[input_element_count];
	};

	struct PosNormalTanTex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 tangent_u;
		XMFLOAT2 tex_coord;

		static const int input_element_count = 4;
		static const D3D11_INPUT_ELEMENT_DESC input_element_desc[input_element_count];
	};
}