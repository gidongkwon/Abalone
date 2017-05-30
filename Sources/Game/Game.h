#pragma once

#include "../Framework/App.h"
#include "Utils/TimerContainer.h"
#include "Framework/Renderer/MutableConstantBuffer.h"
#include "Framework/Models/OBJModel.h"
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
	Camera camera_;

	BasicModel sphere_;
	OBJModel board_model_;

	DirectionalLight directional_light_;
	SpotLight spot_light_;

	std::vector<BasicModel*> models_;
};
