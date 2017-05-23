#include "Lights.hlsli"

// postfixes
// w = world
// l = local
// h = homogeneous

cbuffer ObjectData
{
	Material material;
};

cbuffer FrameData
{
	DirectionalLight directional_light;
	SpotLight spot_light;
	float3 eye_position_w;
	float pad;
};

struct VertexOut
{
	float4 pos_h : SV_POSITION;
	float3 pos_w : POSITION;
	float3 normal_w : NORMAL;
};

float4 main(VertexOut pin) : SV_TARGET
{
	pin.normal_w = normalize(pin.normal_w);

	float3 to_eye_w = normalize(eye_position_w - pin.pos_w);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;

	compute_directional_light(material, directional_light, pin.normal_w, to_eye_w, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;

	/*compute_point_light(material, point_light, pin.pos_w, pin.normal_w, to_eye_w, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;*/

	compute_spot_light(material, spot_light, pin.pos_w, pin.normal_w, to_eye_w, A, D, S);
	ambient += A;
	diffuse += D;
	specular += S;

	float4 lit_color = ambient + diffuse + specular;

	lit_color.a = material.diffuse.a;

	return lit_color;
}