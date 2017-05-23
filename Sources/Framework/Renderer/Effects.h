#pragma once
#include "MutableConstantBuffer.h"
#include "Material.h"
#include "Lights.h"

struct ShaderBytecode
{
	ShaderBytecode(const std::string& path);

	std::vector<char> bytecode;
};

struct IEffect
{
	virtual ~IEffect() {}
	virtual void apply(ID3D11DeviceContext* context) = 0;
};

struct VSObjectData
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 world_view_proj;
	XMFLOAT4X4 world_inverse_transpose;
};

struct PSObjectData
{
	Material material;
};

struct PSFrameData
{
	DirectionalLight directional_light;
	SpotLight spot_light;
	XMFLOAT3 eye_position_w;
	float pad;
};

class BasicEffect: public IEffect
{
public:
	BasicEffect(ID3D11Device* device, const std::string& compiled_vs_path, const std::string& compiled_ps_path);
	~BasicEffect();

	void set_frame_data(ID3D11DeviceContext* context, PSFrameData ps_data);
	void set_object_data(ID3D11DeviceContext* context, VSObjectData vs_data, PSObjectData ps_data);
	void apply(ID3D11DeviceContext* context) override;
	
	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;

	ShaderBytecode vs_bytecode;
	ShaderBytecode ps_bytecode;

private:
	MutableConstantBuffer<VSObjectData> vs_buffer_object_;
	MutableConstantBuffer<PSObjectData> ps_buffer_object_;
	MutableConstantBuffer<PSFrameData> ps_buffer_frame_;
};

struct Effects
{
	static void init(ID3D11Device* device);
	static void destroy();

	static BasicEffect* basic_effect;
};