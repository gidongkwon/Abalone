// postfixes
// w = world
// l = local
// h = homogeneous

cbuffer ObjectData
{
	float4x4 world;
	float4x4 world_view_proj;
	float4x4 world_inverse_transpose;
};

struct VertexIn
{
	float3 pos_l : POSITION;
	float3 normal_l : NORMAL;
	float3 tangent_u : TANGENT;
	float2 tex_coord : TEXCOORD;
};

struct VertexOut
{
	float4 pos_h : SV_POSITION;
	float3 pos_w : POSITION;
	float3 normal_w : NORMAL;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	// Transform to world space.
	vout.pos_w = mul(float4(vin.pos_l, 1.0f), world).xyz;
	vout.normal_w = mul(vin.normal_l, (float3x3)world_inverse_transpose);

	// Transform to homogeneous clip space.
	vout.pos_h = mul(float4(vin.pos_l, 1.0f), world_view_proj);

	return vout;
}