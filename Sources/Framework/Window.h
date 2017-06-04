#pragma once

#include "Settings.h"
#include "DeviceResources.h"

// WinAPI wrapper class providing window control.
class Window
{
public:
	Window(Settings* settings, DeviceResources* d3d_resources);
	~Window();

	void init(const std::string& name);

	int width() const;
	int height() const;
	float aspect_ratio() const;
	bool is_minimized() const;
	bool is_inited() const;

	void on_resize() const;

	void toggle_fullscreen();
	void set_fullscreen(bool is_fullscreen);

	void set_title(const std::string& name);

	HWND h_window;

private:
	friend class App;

	Settings* settings_;
	DeviceResources* device_resources_;

	bool inited_;

	int width_;
	int height_;

	int prev_windowed_x_;
	int prev_windowed_y_;
	int prev_windowed_width_;
	int prev_windowed_height_;

	bool maximized_;
	bool minimized_;
	bool resizing_;
	bool updating_x_y_;

	std::function<void()> resize_callback;

	static LRESULT CALLBACK s_window_proc(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param);
	LRESULT CALLBACK window_proc(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param);
};
