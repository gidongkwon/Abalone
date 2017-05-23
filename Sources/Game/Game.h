#pragma once

#include "../Framework/App.h"
#include "Utils/TimerContainer.h"
#include "Framework/Renderer/MutableConstantBuffer.h"
#include "Framework/Models/Mesh.h"
#include "Framework/Renderer/Camera.h"
#include "Framework/Renderer/Lights.h"

class Game: public App
{
public:
	Game();
	~Game();

	void update(float dt) override;
	void render() override;
	void on_resize() override;

private:
	void start_title_update();

	TimerContainer timers_;
	Camera camera;

	BasicModel sphere_;
	BasicModel grid_;

	DirectionalLight directional_light;
	SpotLight spot_light;

	std::vector<BasicModel*> models;
};
