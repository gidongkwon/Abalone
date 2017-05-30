#include "pch.h"
#include "OBJModel.h"
#include "Framework/Renderer/VertexTypes.h"

bool OBJModel::load(ID3D11Device* device, const std::string& obj_path)
{
	std::ifstream obj_file(obj_path);
	if (obj_file.fail())
		return false;

	char buffer[256] = { 0, };
	int vertex_count = 0;
	int tex_coord_count = 0;
	int normal_count = 0;
	int face_count = 0;

	while (!obj_file.eof()) {
		obj_file.getline(buffer, sizeof(buffer));

		if (buffer[0] == 'v' && buffer[1] == ' ')
			vertex_count++;
		else if (buffer[0] == 'v' && buffer[1] == 't')
			tex_coord_count++;
		else if (buffer[0] == 'v' && buffer[1] == 'n')
			normal_count++;
		else if (buffer[0] == 'f' && buffer[1] == ' ')
			face_count++;
	}

	obj_file.clear();
	obj_file.seekg(std::ios::beg);

	memset(buffer, '\0', sizeof(buffer));

	uint32_t vertex_index = 0;
	uint32_t tex_coord_index = 0;
	uint32_t normal_index = 0;
	uint32_t face_index = 0;
	std::vector<XMFLOAT3> positions(vertex_count);
	std::vector<XMFLOAT2> tex_coords(tex_coord_count);
	std::vector<XMFLOAT3> normals(normal_count);

	std::vector<vertex::PosNormalTanTex> vertices(face_count * 3);
	std::vector<uint32_t> indices(face_count * 3);

	while (!obj_file.eof()) {
		// Not using getline() this time, to read actual value by >>
		obj_file.get(buffer[0]);
		obj_file.get(buffer[1]);

		if (buffer[0] == 'v' && buffer[1] == ' ')
		{
			auto& position = positions[vertex_index];
			obj_file >> position.x >> position.y >> position.z;
			obj_file.get(); // \n
			
			vertex_index++;
		}
		else if (buffer[0] == 'v' && buffer[1] == 't')
		{
			auto& tex_coord = tex_coords[tex_coord_index];
			obj_file >> tex_coord.x >> tex_coord.y;
			obj_file.get(); // \n

			tex_coord_index++;
		}
		else if (buffer[0] == 'v' && buffer[1] == 'n')
		{
			auto& normal = normals[normal_index];
			obj_file >> normal.x >> normal.y >> normal.z;
			obj_file.get(); // \n

			normal_index++;
		}
		else if (buffer[0] == 'f' && buffer[1] == ' ')
		{
			// Read rest of the line and parse it with sscanf
			obj_file.getline(buffer, sizeof(buffer));

			uint32_t vertex_indices[3], tex_coord_indices[3], normal_indices[3];

			sscanf_s(buffer, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertex_indices[0], &tex_coord_indices[0], &normal_indices[0],
				&vertex_indices[1], &tex_coord_indices[1], &normal_indices[1],
				&vertex_indices[2], &tex_coord_indices[2], &normal_indices[2]);

			for (int i = 0; i < 3; i++)
			{
				auto& vertex = vertices[face_index * 3 + i];

				vertex.position = positions[vertex_indices[i] - 1];
				vertex.tex_coord = tex_coords[tex_coord_indices[i] - 1];
				vertex.normal = normals[normal_indices[i] - 1];

				// TODO: Set tangent_u
			}

			face_index++;
		}
		else
		{
			obj_file.getline(buffer, sizeof(buffer));
		}

		memset(buffer, '\0', sizeof(buffer));
	}

	for (auto i = 0; i < indices.size(); i++)
		indices[i] = i;

	mesh.set_vertices(device, vertices.data(), vertices.size());
	mesh.set_indices(device, indices.data(), indices.size());

	return true;
}
