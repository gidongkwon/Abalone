#include "pch.h"
#include "InputLayouts.h"
#include "Effects.h"
#include "VertexTypes.h"

ID3D11InputLayout* InputLayouts::position_normal_tan_tex = nullptr;

void InputLayouts::init(ID3D11Device* device)
{
	Assert(Effects::basic_effect, "Effects must be initialized first");

	device->CreateInputLayout(
		vertex::PosNormalTanTex::input_element_desc,
		vertex::PosNormalTanTex::input_element_count,
		&Effects::basic_effect->vs_bytecode.bytecode[0],
		Effects::basic_effect->vs_bytecode.bytecode.size(),
		&position_normal_tan_tex
	);
}

void InputLayouts::destroy()
{
	position_normal_tan_tex->Release();
}
