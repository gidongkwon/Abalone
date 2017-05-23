struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 direction;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 attenuation;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;

	float3 position;
	float range;

	float3 direction;
	float spot;

	float3 attenuation;
	float pad;
};

struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; // w = specular power
	float4 reflect;
};

void compute_directional_light(
	Material material, DirectionalLight light,
	float3 normal, float3 to_eye,
	out float4 ambient, out float4 diffuse,	out float4 specular
)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 light_vec = -light.direction;

	ambient = material.ambient * light.ambient;
	
	float diffuse_factor = dot(light_vec, normal);

	[flatten]
	if (diffuse_factor > 0.0f)
	{
		float3 v = reflect(-light_vec, normal);
		float specular_factor = pow(max(dot(v, to_eye), 0.0f), material.specular.w);

		diffuse = diffuse_factor * material.diffuse * light.diffuse;
		specular = specular_factor * material.specular * light.specular;
	}
}

void compute_point_light(
	Material material, PointLight light,
	float3 position, float3 normal, float3 to_eye,
	out float4 ambient, out float4 diffuse, out float4 specular
)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 light_vec = light.position - position;

	float d = length(light_vec);

	if (d > light.range)
		return;

	light_vec /= d;

	ambient = material.ambient * light.ambient;

	float diffuse_factor = dot(light_vec, normal);

	[flatten]
	if (diffuse_factor > 0.0f)
	{
		float3 v = reflect(-light_vec, normal);
		float specular_factor = pow(max(dot(v, to_eye), 0.0f), material.specular.w);

		diffuse = diffuse_factor * material.diffuse * light.diffuse;
		specular = specular_factor * material.specular * light.specular;
	}

	float attenuation = 1.0f / dot(light.attenuation, float3(1.0f, d, d*d));

	diffuse *= attenuation;
	specular *= attenuation;
}

void compute_spot_light(
	Material material, SpotLight light,
	float3 position, float3 normal, float3 to_eye,
	out float4 ambient, out float4 diffuse, out float4 specular
)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 light_vec = light.position - position;

	float d = length(light_vec);

	if (d > light.range)
		return;

	light_vec /= d;

	ambient = material.ambient * light.ambient;

	float diffuse_factor = dot(light_vec, normal);

	[flatten]
	if (diffuse_factor > 0.0f) {
		float3 v = reflect(-light_vec, normal);
		float specular_factor = pow(max(dot(v, to_eye), 0.0f), material.specular.w);

		diffuse = diffuse_factor * material.diffuse * light.diffuse;
		specular = specular_factor * material.specular * light.specular;
	}

	float spot = pow(max(dot(-light_vec, light.direction), 0.0f), light.spot);

	float attenuation = spot / dot(light.attenuation, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= attenuation;
	specular *= attenuation;
}