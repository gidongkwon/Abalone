#pragma once

#include "Settings.h"
#include "GameTimer.h"
#include "Window.h"

class DeviceResources;

class App
{
public:
	App(std::string name, std::string setting_file_path);
	virtual ~App();

	void run();

	GameTimer* game_timer;
	Settings* settings;

	DeviceResources* device_resources;
	Window* window;

protected:
	virtual void update(float dt) = 0;
	virtual void render() = 0;
	virtual void on_resize() = 0;

	std::string name_;
};

