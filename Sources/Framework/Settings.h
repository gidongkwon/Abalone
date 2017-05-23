#pragma once

struct Settings
{
	Settings(std::string file_path);

	void load();
	void save() const;

	bool is_fullscreen;
	bool enable_msaa_4x;
	bool enable_vsync;
	int width;
	int height;

private:
	std::string file_path_;
};