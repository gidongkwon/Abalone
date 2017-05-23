#pragma once
#include "Framework/Renderer/Material.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void draw(ID3D11DeviceContext* context);

	template<class VertexType>
	void set_vertices(ID3D11Device* device, const VertexType* vertices, uint32_t count);

	void set_indices(ID3D11Device* device, const uint32_t* indices, uint32_t count);

private:
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;

	uint32_t vertex_stride_;
	uint32_t index_count_;
};

template<class VertexType>
void Mesh::set_vertices(ID3D11Device* device, const VertexType* vertices, uint32_t count)
{
	release_com(vertex_buffer_);

	vertex_stride_ = sizeof(VertexType);

	D3D11_BUFFER_DESC vertex_buffer_desc;

	vertex_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vertex_buffer_desc.ByteWidth = vertex_stride_ * count;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = 0;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertex_init_data;
	vertex_init_data.pSysMem = vertices;

	throw_if_failed(
		device->CreateBuffer(&vertex_buffer_desc, &vertex_init_data, &vertex_buffer_)
	);
}

struct BasicModel
{
	BasicModel();

	Material material;
	Mesh mesh;
	
	XMFLOAT4X4 world;
	bool is_wireframe;
};