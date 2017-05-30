#pragma once

#include "Mesh.h"

// Simple .obj model container.
// Implemented for studying. We focused on loading vertices
// thus many other specs are ignored.
// Faces are must be triangulated.
class OBJModel: public BasicModel
{
public:
	OBJModel() = default;

	bool load(ID3D11Device* device, const std::string& obj_path);
};