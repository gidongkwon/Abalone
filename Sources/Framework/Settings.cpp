#include "pch.h"
#include "Settings.h"

Settings::Settings(std::string file_path):
	file_path_(file_path),
	is_fullscreen(false),
	width(800),
	height(600),
	enable_msaa_4x(true),
	enable_vsync(true)
{
	load();
}

// no error handling
// just load settings regardless of the input order
void Settings::load()
{
	using namespace std;
		
	const static regex is_bool("^true|false$");
	const static regex is_float("^[-+]?[0-9]+\\.[0-9]+$");
	const static regex is_int("^[-+]?[0-9]+$");

	ifstream file(file_path_);

	if (!file.is_open())
		cout << "No settings file." << endl;

	unordered_map<string, variant<string, bool, int, float>> value_map;

	string key;
	string value;

	while (!file.eof()) {
		file >> key >> value;

		cout << key << ": " << value << endl;
		
		if (regex_match(value, is_bool))
			value_map[key] = (value == "true");
		else if (regex_match(value, is_float))
			value_map[key] = stof(value);
		else if (regex_match(value, is_int))
			value_map[key] = stoi(value);
		else
			value_map[key] = value;
	}

	cout << endl;

	is_fullscreen = get<bool>(value_map["fullscreen"]);
	width = get<int>(value_map["width"]);
	height = get<int>(value_map["height"]);
	enable_msaa_4x = get<bool>(value_map["msaa4x"]);
	enable_vsync = get<bool>(value_map["vsync"]);
}

void Settings::save() const
{
	using namespace std;

	ofstream file(file_path_);

	file << boolalpha;

	file << "fullscreen " << is_fullscreen << '\n';
	file << "width " << width << '\n';
	file << "height " << height << '\n';
	file << "msaa4x " << enable_msaa_4x << '\n';
	file << "vsync " << enable_vsync;
}
