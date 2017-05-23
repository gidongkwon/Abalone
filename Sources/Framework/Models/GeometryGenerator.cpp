#include "pch.h"
#include "GeometryGenerator.h"
#include "Mesh.h"

using namespace DirectX;
using Vertex = vertex::PosNormalTanTex;

void GeometryGenerator::create_box(Mesh* out, ID3D11Device* device, float width, float height, float depth)
{
	// Create vertices - not efficient, for study.

	Vertex v[24];

	float half_width = 0.5f * width;
	float half_height = 0.5f * height;
	float half_depth = 0.5f * depth;

	// constants for texture coordinate
	const XMFLOAT2 top_left = { 0.0f, 0.0f };
	const XMFLOAT2 bottom_left = { 0.0f, 1.0f };
	const XMFLOAT2 top_right = { 1.0f, 0.0f };
	const XMFLOAT2 bottom_right = { 1.0f, 1.0f };
	
	// front & back
	//   7 ---- 6
	//  /|     /|
	// 1 ---- 2 |
	// | 4 ---| 5
	// |/     |/
	// 0 ---- 3

	// front face
	v[0].position = { -half_width, -half_height, -half_depth };
	v[0].tex_coord = bottom_left;
	
	v[1].position = { -half_width, +half_height, -half_depth };
	v[1].tex_coord = top_left;

	v[2].position = { +half_width, +half_height, -half_depth };
	v[2].tex_coord = top_right;

	v[3].position = { +half_width, -half_height, -half_depth };
	v[3].tex_coord = bottom_right;

	v[0].normal = v[1].normal = v[2].normal = v[3].normal = { 0.0f, 0.0f, -1.0f };
	v[0].tangent_u = v[1].tangent_u = v[2].tangent_u = v[3].tangent_u = { 1.0f, 0.0f, 0.0f };

	// back face
	v[4].position = { -half_width, -half_height, +half_depth };
	v[4].tex_coord = bottom_right;
	
	v[5].position = { +half_width, -half_height, +half_depth };
	v[5].tex_coord = bottom_left;

	v[6].position = { +half_width, +half_height, +half_depth };
	v[6].tex_coord = top_left;

	v[7].position = { -half_width, +half_height, +half_depth };
	v[7].tex_coord = top_right;

	v[4].normal = v[5].normal = v[6].normal = v[7].normal = { 0.0f, 0.0f, 1.0f };
	v[4].tangent_u = v[5].tangent_u = v[6].tangent_u = v[7].tangent_u = { -1.0f, 0.0f, 0.0f };

	// top & bottom
	//   9 ----10
	//  /|     /|
	// 8 ----11 |
	// | 15---|14
	// |/     |/
	// 12----13

	// top face
	v[8].position = v[1].position;
	v[8].tex_coord = bottom_left;
	
	v[9].position = v[7].position;
	v[9].tex_coord = top_left;

	v[10].position = v[6].position;
	v[10].tex_coord = top_right;

	v[11].position = v[2].position;
	v[11].tex_coord = bottom_right;

	v[8].normal = v[9].normal = v[10].normal = v[11].normal = { 0.0f, 1.0f, 0.0f };
	v[8].tangent_u = v[9].tangent_u = v[10].tangent_u = v[11].tangent_u = { 1.0f, 0.0f, 0.0f };

	// bottom face
	v[12].position = v[0].position;
	v[12].tex_coord = bottom_right;
	
	v[13].position = v[3].position;
	v[13].tex_coord = bottom_left;

	v[14].position = v[5].position;
	v[14].tex_coord = top_left;

	v[15].position = v[4].position;
	v[15].tex_coord = top_right;

	v[12].normal = v[13].normal = v[14].normal = v[15].normal = { 0.0f, -1.0f, 0.0f };
	v[12].tangent_u = v[13].tangent_u = v[14].tangent_u = v[15].tangent_u = { -1.0f, 0.0f, 0.0f };

	// left & right
	//   17----22
	//  /|     /|
	// 18----21 |
	// | 16---|23
	// |/     |/
	// 19----20

	// left face
	v[16].position = v[4].position;
	v[16].tex_coord = bottom_left;
	
	v[17].position = v[7].position;
	v[17].tex_coord = top_left;

	v[18].position = v[1].position;
	v[18].tex_coord = top_right;

	v[19].position = v[0].position;
	v[19].tex_coord = bottom_right;

	v[16].normal = v[17].normal = v[18].normal = v[19].normal = { -1.0f, 0.0f, 0.0f };
	v[16].tangent_u = v[17].tangent_u = v[18].tangent_u = v[19].tangent_u = { 0.0f, 0.0f, -1.0f };

	// right face
	v[20].position = v[3].position;
	v[20].tex_coord = bottom_left;
	
	v[21].position = v[2].position;
	v[21].tex_coord = top_left;

	v[22].position = v[6].position;
	v[22].tex_coord = top_right;

	v[23].position = v[5].position;
	v[23].tex_coord = bottom_right;

	v[20].normal = v[21].normal = v[22].normal = v[23].normal = { 1.0f, 0.0f, 0.0f };
	v[20].tangent_u = v[21].tangent_u = v[22].tangent_u = v[23].tangent_u = { 0.0f, 0.0f, 1.0f };

	out->set_vertices(device, v, 24);

	// Create indices.
	// +0 +1 +2
	// +0 +2 +3

	uint32_t i[36];

	// front face
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	
	// back face
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// top face
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// bottom face
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// left face
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// right face
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	out->set_indices(device, i, 36);
}

void GeometryGenerator::create_sphere(Mesh* out, ID3D11Device* device, float radius, uint32_t slice_count, uint32_t stack_count)
{
	// Create vertices.

	std::vector<Vertex> vertices;

	Vertex top;
	top.position = { 0.0f, +radius, 0.0f };
	top.normal = { 0.0f, 1.0f, 0.0f };
	top.tangent_u = { 1.0f, 0.0f, 0.0f };
	top.tex_coord = { 0.0f, 0.0f };

	Vertex bottom;
	bottom.position = { 0.0f, -radius, 0.0f };
	bottom.normal = { 0.0f, -1.0f, 0.0f };
	top.tangent_u = { 1.0f, 0.0f, 0.0f };
	top.tex_coord = { 0.0f, 1.0f };

	vertices.push_back(top);

	float phi_step = XM_PI / stack_count;
	float theta_step = XM_2PI / slice_count;
	
	for (uint32_t i = 1; i <= stack_count - 1; ++i)
	{
		float phi = i * phi_step;

		// ring vertices
		for (uint32_t j = 0; j <= slice_count; ++j)
		{
			float theta = j * theta_step;

			Vertex v;

			// spherical to cartesian
			v.position.x = radius * sin(phi) * cos(theta);
			v.position.y = radius * cos(phi);
			v.position.z = radius * sin(phi) * sin(theta);

			v.tangent_u.x = -radius * sin(phi) * sin(theta);
			v.tangent_u.y = 0.0f;
			v.tangent_u.z = +radius * sin(phi) * cos(theta);

			XMVECTOR T = XMLoadFloat3(&v.tangent_u);
			XMStoreFloat3(&v.tangent_u, XMVector3Normalize(T));

			XMVECTOR P = XMLoadFloat3(&v.position);
			XMStoreFloat3(&v.normal, XMVector3Normalize(P));

			v.tex_coord.x = theta / XM_2PI;
			v.tex_coord.y = phi / XM_PI;

			vertices.push_back(v);
		}
	}

	vertices.push_back(bottom);
	out->set_vertices(device, &vertices[0], vertices.size());

	// Create indices.

	std::vector<uint32_t> indices;

	// top stack
	for (uint32_t i = 1; i <= slice_count; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	uint32_t base_index = 1;
	uint32_t ring_vertex_count = slice_count + 1;

	// inner stacks
	for (uint32_t i = 0; i < stack_count - 2; ++i)
	{
		for (uint32_t j = 0; j < slice_count; ++j)
		{
			auto current_stack_index = i * ring_vertex_count;
			auto next_stack_index = (i + 1) * ring_vertex_count;

			// 0--1
			// | /
			// |/
			// 2
			indices.push_back(base_index + current_stack_index + j);
			indices.push_back(base_index + current_stack_index + j + 1);
			indices.push_back(base_index + next_stack_index + j);

			//    4
			//   /|
			//  / |
			// 3--5
			indices.push_back(base_index + next_stack_index + j);
			indices.push_back(base_index + current_stack_index + j + 1);
			indices.push_back(base_index + next_stack_index + j + 1);
		}
	}

	// bottom stack
	uint32_t bottom_index = (uint32_t)vertices.size() - 1;
	base_index = bottom_index - ring_vertex_count;

	for (uint32_t i = 1; i < slice_count; ++i) {
		indices.push_back(bottom_index);
		indices.push_back(base_index + i);
		indices.push_back(base_index + i + 1);
	}

	out->set_indices(device, &indices[0], indices.size());
}

void GeometryGenerator::create_grid(Mesh* out, ID3D11Device* device, float width, float depth, uint32_t column, uint32_t row)
{
	uint32_t vertex_count = row * column;
	uint32_t face_count = (row - 1) * (column - 1) * 2;

	// Create vertices.

	std::vector<Vertex> vertices(vertex_count);

	float half_width = 0.5f * width;
	float half_depth = 0.5f * depth;

	float dx = width / (row - 1);
	float dz = depth / (column - 1);
	
	float du = 1.0f / (row - 1);
	float dv = 1.0f / (column - 1);

	for (uint32_t i = 0; i < column; ++i)
	{
		float z = half_depth - i * dz;

		for (uint32_t j = 0; j < row; ++j)
		{
			float x = -half_width + j * dx;

			auto& v = vertices[i * row + j];
			
			v.position = XMFLOAT3(x, 0.0f, z);
			v.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			v.tangent_u = XMFLOAT3(1.0f, 0.0f, 0.0f);

			v.tex_coord = { j * du, i * dv };
		}
	}

	out->set_vertices(device, &vertices[0], vertices.size());

	// Create indices.

	std::vector<uint32_t> indices(face_count * 3);

	uint32_t k = 0;

	for (uint32_t i = 0; i < column - 1; ++i)
	{
		for (uint32_t j = 0; j < row - 1; ++j)
		{
			auto this_row = i * row;
			auto next_row = (i + 1) * row;

			// 0--1
			// | /
			// |/
			// 2
			indices[k] = this_row + j;
			indices[k + 1] = this_row + j + 1;
			indices[k + 2] = next_row + j;

			//    4
			//   /|
			//  / |
			// 3--5
			indices[k + 3] = next_row + j;
			indices[k + 4] = this_row + j + 1;
			indices[k + 5] = next_row + j + 1;

			k += 6;
		}
	}

	out->set_indices(device, &indices[0], indices.size());
}
