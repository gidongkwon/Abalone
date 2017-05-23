#include "pch.h"
#include "Mesh.h"

Mesh::Mesh():
	vertex_buffer_(nullptr),
	index_buffer_(nullptr),
	vertex_stride_(0)
{
}

Mesh::~Mesh()
{
	vertex_buffer_->Release();
	index_buffer_->Release();
}

void Mesh::draw(ID3D11DeviceContext* context)
{
	uint32_t offset = 0;

	context->IASetVertexBuffers(0, 1, &vertex_buffer_, &vertex_stride_, &offset);
	context->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(index_count_, 0, 0);
}

void Mesh::set_indices(ID3D11Device* device, const uint32_t* indices, uint32_t count)
{
	index_count_ = count;

	D3D11_BUFFER_DESC index_buffer_desc;
	index_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	index_buffer_desc.ByteWidth = sizeof(uint32_t) * count;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA index_init_data;
	index_init_data.pSysMem = indices;

	throw_if_failed(
		device->CreateBuffer(&index_buffer_desc, &index_init_data, &index_buffer_)
	);
}

BasicModel::BasicModel():
	is_wireframe(false)
{
	XMStoreFloat4x4(&world, XMMatrixIdentity());
}
