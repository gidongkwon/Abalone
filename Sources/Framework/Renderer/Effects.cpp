#include "pch.h"
#include "Effects.h"
#include "InputLayouts.h"

using namespace DirectX;

ShaderBytecode::ShaderBytecode(const std::string& path)
{
	std::ifstream shader_file(path, std::ios::in | std::ios::binary | std::ios::ate);

	assert(shader_file.is_open(), "No shader file");

	size_t size = (size_t)shader_file.tellg();
	bytecode.resize(size);

	shader_file.seekg(std::ios::beg);
	shader_file.read(&bytecode[0], size);
	shader_file.close();
}

BasicEffect::BasicEffect(ID3D11Device* device, const std::string& compiled_vs_path, const std::string& compiled_ps_path):
	vs_bytecode(compiled_vs_path), ps_bytecode(compiled_ps_path)
{
	// TODO: Refactor shader creation to use cache
	throw_if_failed(
		device->CreateVertexShader(
			&vs_bytecode.bytecode[0], vs_bytecode.bytecode.size(), nullptr, &vertex_shader
		)
	);

	throw_if_failed(
		device->CreatePixelShader(
			&ps_bytecode.bytecode[0], ps_bytecode.bytecode.size(), nullptr, &pixel_shader
		)
	);

	vs_buffer_object_.create(device);
	ps_buffer_object_.create(device);
	ps_buffer_frame_.create(device);
}

BasicEffect::~BasicEffect()
{
	vertex_shader->Release();
	pixel_shader->Release();
}

void BasicEffect::set_frame_data(ID3D11DeviceContext* context, PSFrameData ps_data)
{
	ps_buffer_frame_.set_data(context, ps_data);

	context->PSSetConstantBuffers(1, 1, &ps_buffer_frame_.constant_buffer);
}

void BasicEffect::set_object_data(ID3D11DeviceContext* context, VSObjectData vs_data, PSObjectData ps_data)
{
	XMStoreFloat4x4(&vs_data.world, XMMatrixTranspose(XMLoadFloat4x4(&vs_data.world)));
	XMStoreFloat4x4(&vs_data.world_view_proj, XMMatrixTranspose(XMLoadFloat4x4(&vs_data.world_view_proj)));
	XMStoreFloat4x4(&vs_data.world_inverse_transpose, XMMatrixTranspose(XMLoadFloat4x4(&vs_data.world_inverse_transpose)));

	vs_buffer_object_.set_data(context, vs_data);
	ps_buffer_object_.set_data(context, ps_data);

	context->VSSetConstantBuffers(0, 1, &vs_buffer_object_.constant_buffer);
	context->PSSetConstantBuffers(0, 1, &ps_buffer_object_.constant_buffer);
}

void BasicEffect::apply(ID3D11DeviceContext* context)
{
	context->VSSetShader(vertex_shader, nullptr, 0);
	context->PSSetShader(pixel_shader, nullptr, 0);

	context->IASetInputLayout(InputLayouts::position_normal_tan_tex);
}

BasicEffect* Effects::basic_effect = nullptr;

void Effects::init(ID3D11Device* device)
{
	basic_effect = new BasicEffect(
		device,
		"Resources/Shaders/Compiled/BasicVS.cso",
		"Resources/Shaders/Compiled/BasicPS.cso"
	);
}

void Effects::destroy()
{
	delete basic_effect;
}
