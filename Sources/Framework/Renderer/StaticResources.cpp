#include "pch.h"
#include "StaticResources.h"
#include "VertexTypes.h"

void InputLayouts::init(ID3D11Device* device)
{
	position = device->CreateInputLayout(
		vertex::PosNormalTanTex::input_element_desc,
		vertex::PosNormalTanTex::input_element_count,
		);
}

void InputLayouts::destroy()
{
	position->Release();
}
