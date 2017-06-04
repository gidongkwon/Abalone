#include "pch.h"
#include "Window.h"
#include "../Utils/StringUtil.h"

Window::Window(Settings* settings, DeviceResources* d3d_resources):
	settings_(settings),
	device_resources_(d3d_resources),
	inited_(false),
	width_(0), height_(0),
	prev_windowed_x_(0), prev_windowed_y_(0), prev_windowed_width_(0), prev_windowed_height_(0),
	maximized_(false),
	minimized_(false),
	resizing_(false),
	updating_x_y_(true)
{
}

Window::~Window()
{
}

void Window::init(const std::string& name)
{
	prev_windowed_width_ = width_ = settings_->width;
	prev_windowed_height_ = height_ = settings_->height;

	auto wname = widen(name);

	HINSTANCE h_instance = GetModuleHandleW(nullptr);
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = &Window::s_window_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = sizeof(Window*);
	wc.hInstance = h_instance;
	//window_class.hIcon = LoadIcon(h_instance, L"IDI_ICON");
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wname.c_str();
	//window_class.hIconSm = LoadIcon(h_instance, L"IDI_ICON");
	wc.hIconSm = nullptr;

	if (!RegisterClassExW(&wc))
		throw "RegisterClass Failed";

	RECT rect = { 0, 0, width_, height_ };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	h_window = CreateWindowExW(0, wname.c_str(), wname.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, h_instance, static_cast<void*>(this));

	if (!h_window)
		throw "CreateWindow Failed";

	ShowWindow(h_window, SW_SHOW);
	set_fullscreen(settings_->is_fullscreen);

	inited_ = true;
}

int Window::width() const
{
	return width_;
}

int Window::height() const
{
	return height_;
}

float Window::aspect_ratio() const
{
	return (float)width_ / height_;
}

bool Window::is_minimized() const
{
	return minimized_;
}

bool Window::is_inited() const
{
	return inited_;
}

void Window::on_resize() const
{
	if (device_resources_->viewport.Width == width_ && device_resources_->viewport.Height == height_)
		return;
	device_resources_->on_resize(width_, height_);
	resize_callback();
}

void Window::toggle_fullscreen()
{
	set_fullscreen(!settings_->is_fullscreen);
}

void Window::set_fullscreen(bool is_fullscreen)
{
	settings_->is_fullscreen = is_fullscreen;

	if (is_fullscreen)
	{
		updating_x_y_ = false;

		SetWindowLongPtrW(h_window, GWL_STYLE, 0);
		SetWindowLongPtrW(h_window, GWL_EXSTYLE, WS_EX_TOPMOST);

		SetWindowPos(h_window, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowWindow(h_window, SW_SHOWMAXIMIZED);
	}
	else
	{
		SetWindowLongPtrW(h_window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowLongPtrW(h_window, GWL_EXSTYLE, 0);

		ShowWindow(h_window, SW_SHOWNORMAL);

		// Calling ShowWindow() triggers WM_MOVE, so we set updating_x_y_ as true after that.
		updating_x_y_ = true;

		SetWindowPos(h_window, HWND_TOP, prev_windowed_x_, prev_windowed_y_, prev_windowed_width_, prev_windowed_height_, SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	settings_->save();
}

void Window::set_title(const std::string & name)
{
	SetWindowTextW(h_window, widen(name).c_str());
}

LRESULT Window::s_window_proc(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(l_param);
		LPVOID self = cs->lpCreateParams;
		SetWindowLongPtrW(h_window, 0, reinterpret_cast<LONG_PTR>(self));
	}

	auto* self = reinterpret_cast<Window*>(GetWindowLongPtrW(h_window, 0));
	return self->window_proc(h_window, msg, w_param, l_param);
}

LRESULT Window::window_proc(HWND h_window, UINT msg, WPARAM w_param, LPARAM l_param)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE); // No beep on ALT+ENTER.

	case WM_SYSKEYDOWN:
		if (w_param == VK_RETURN && (l_param & 0x60000000))
			toggle_fullscreen();
		break;

	case WM_GETMINMAXINFO:
	{
		auto* info = reinterpret_cast<MINMAXINFO*>(l_param);
		info->ptMinTrackSize.x = 320;
		info->ptMinTrackSize.y = 200;

		break;
	}

	case WM_SIZE:
		width_ = LOWORD(l_param);
		height_ = HIWORD(l_param);

		if (!device_resources_->is_inited())
			return 0;

		switch (w_param)
		{
		case SIZE_MINIMIZED:
			maximized_ = false;
			minimized_ = true;
			break;

		case SIZE_MAXIMIZED:
			maximized_ = true;
			minimized_ = false;
			break;

		case SIZE_RESTORED:
			maximized_ = false;
			minimized_ = false;

			if (!resizing_)
				on_resize();
			break;
		}

		break;

	case WM_ENTERSIZEMOVE:
		resizing_ = true;
		break;

	case WM_EXITSIZEMOVE:
		resizing_ = false;

		prev_windowed_width_ = width_;
		prev_windowed_height_ = height_;

		on_resize();
		break;

	case WM_MOVE:
		if (!updating_x_y_)
			return 0;
		prev_windowed_x_ = LOWORD(l_param);
		prev_windowed_y_ = HIWORD(l_param);
		break;
	}

	return DefWindowProcW(h_window, msg, w_param, l_param);
}