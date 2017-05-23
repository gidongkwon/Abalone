#include "pch.h"
#include "Game.h"
#include "Framework/DeviceResources.h"
#include "Framework/Window.h"
#include "Framework/Models/GeometryGenerator.h"
#include "Framework/Models/Mesh.h"
#include "Framework/Renderer/Effects.h"
#include "Framework/Renderer/StaticResources.h"
#include "Utils/MathUtil.h"

using namespace DirectX;

Game::Game():
	App("Abalone", "Resources/settings"),
	timers_(50)
{
	start_title_update();

	auto W = XMMatrixIdentity();
	W *= XMMatrixTranslation(0, 1.0f, 0);
	XMStoreFloat4x4(&sphere_.world, W);
	XMStoreFloat4x4(&grid_.world, XMMatrixIdentity());

	GeometryGenerator::create_grid(&grid_.mesh, device_resources->device, 30, 30, 30, 30);
	GeometryGenerator::create_sphere(&sphere_.mesh, device_resources->device, 1.0f, 24, 24);
	//GeometryGenerator::create_box(&sphere_.mesh, device_resources->device, 1.0f, 1.0f, 1.0f);

	models.push_back(&grid_);
	models.push_back(&sphere_);

	camera.position.y = 4;
	camera.position.z = -8;
	camera.look_at(XMVectorZero());


	directional_light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	directional_light.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directional_light.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	directional_light.direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);


	spot_light.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	spot_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spot_light.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	spot_light.attenuation = XMFLOAT3(1.0f, 0.0f, 0.0f);
	spot_light.spot = 10.0f;
	spot_light.range = 10.0f;

	spot_light.position = XMFLOAT3(0.0f, 5.0f, 0.0f);
	spot_light.direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	/*spot_light.position = camera.position;

	XMVECTOR direction = XMVector3Normalize(-XMLoadFloat3(&camera.position));
	XMStoreFloat3(&spot_light.direction, direction);*/

	grid_.material.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	grid_.material.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	grid_.material.specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	sphere_.material.ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	sphere_.material.diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	sphere_.material.specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	on_resize();
}

Game::~Game()
{
}

void Game::update(float dt)
{
	timers_.update(dt);

	XMMATRIX w = XMLoadFloat4x4(&sphere_.world);
	w *= XMMatrixRotationY(XMConvertToRadians(45 * dt));
	XMStoreFloat4x4(&sphere_.world, w);
}

void Game::render()
{
	auto* context = device_resources->immediate_context;
	auto* effect = Effects::basic_effect;

	device_resources->clear();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	effect->set_frame_data(context, { directional_light, spot_light, camera.position });

	for (auto* model : models) {
		if (model->is_wireframe)
			context->RSSetState(RenderStates::wireframe_rs);
		else
			context->RSSetState(RenderStates::solid_rs);

		XMMATRIX world = XMLoadFloat4x4(&model->world);
		XMMATRIX world_view_proj = world * camera.view_proj();
		XMMATRIX world_inverse_transpose = MathUtil::inverse_transpose(world);

		VSObjectData vs_data;
		vs_data.world = model->world;
		XMStoreFloat4x4(&vs_data.world_view_proj, world_view_proj);
		XMStoreFloat4x4(&vs_data.world_inverse_transpose, world_inverse_transpose);

		effect->set_object_data(context, vs_data, { model->material });
		effect->apply(context);

		model->mesh.draw(context);
	}

	device_resources->present();
}

void Game::on_resize()
{
	camera.set_lens(0.25f * XM_PI, window->aspect_ratio(), 1.0f, 1000.f);
}

void Game::start_title_update()
{
	timers_.get().repeat(1.f, -1, [&]()
	{
		std::ostringstream ss;
		ss.precision(5);
		ss << std::fixed 
			<< "Abalone    FPS: " << game_timer->fps() 
			<< "    Frame Time: " << game_timer->delta_time();
		window->set_title(ss.str());
	});
}
