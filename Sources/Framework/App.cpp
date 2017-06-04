#include "pch.h"
#include "App.h"
#include "Window.h"
#include "DeviceResources.h"
#include "Renderer/VertexTypes.h"
#include "Renderer/StaticResources.h"

App::App(std::string name, std::string setting_file_path):
	game_timer(new GameTimer()),
	settings(new Settings(setting_file_path)),
	device_resources(new DeviceResources(settings)),
	window(new Window(settings, device_resources)),
	name_(name)
{
	window->init(name_);
	device_resources->init(window->h_window);

	window->resize_callback = [&]()
	{ 
		on_resize();
	};

	StaticResources::init_all(device_resources->device);
}

App::~App()
{
	StaticResources::destroy_all();

	delete game_timer;
	delete settings;
	delete device_resources;
	delete window;
}

void App::run()
{
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			game_timer->update();
			update(game_timer->delta_time());
			render();
		}
	}
}