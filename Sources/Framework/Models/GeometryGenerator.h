#pragma once

#include "Framework/Renderer/VertexTypes.h"

class Mesh;

class GeometryGenerator
{
public:
	static void create_box(Mesh* out, ID3D11Device* device, float width, float height, float depth);
	static void create_sphere(Mesh* out, ID3D11Device* device, float radius, uint32_t slice_count, uint32_t stack_count);
	static void create_grid(Mesh* out, ID3D11Device* device, float width, float depth, uint32_t column, uint32_t row);
};