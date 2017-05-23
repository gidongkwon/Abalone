#pragma once

template<class T>
class MutableConstantBuffer
{
public:
	MutableConstantBuffer() = default;
	~MutableConstantBuffer()
	{
		constant_buffer->Release();
	}

	void create(ID3D11Device* device)
	{
		D3D11_BUFFER_DESC constant_buffer_desc = {};
		constant_buffer_desc.ByteWidth = sizeof(T);
		constant_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constant_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constant_buffer_desc.MiscFlags = 0;
		constant_buffer_desc.StructureByteStride = 0;

		throw_if_failed(
			device->CreateBuffer(&constant_buffer_desc, nullptr, &constant_buffer)
		);
	}

	void set_data(ID3D11DeviceContext* context, const T& value)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_resource;

		throw_if_failed(
			context->Map(constant_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource)
		);

		*(T*)mapped_resource.pData = value;

		context->Unmap(constant_buffer, 0);
	}

	ID3D11Buffer* constant_buffer;
};