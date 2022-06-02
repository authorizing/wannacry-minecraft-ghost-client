#include "configs.h"
#include "base64.h"

#include "../../modules/modules.h"
#include "../../helpers/xor.h"

#include <fstream>
#include <sstream>
#include <shlobj.h>
#include <iomanip>

C_ConfigManager* cfg_manager = new C_ConfigManager();

void C_ConfigManager::setup()
{
	setup_item(&aim::max_angle, 180.f, "aim.angle");
	setup_item(&aim::aim_speed, 10.f, "aim.speed");
	setup_item(&aim::aim_distance, 6.f, "aim.distance");

	setup_item(&aclicker::key, 0, "clicker.key");
}

void C_ConfigManager::add_item(void * pointer, const char * name, const std::string& type) {
	items.push_back(new C_ConfigItem(std::string(name), pointer, type));
}

void C_ConfigManager::setup_item(int * pointer, int value, const std::string& name) 
{
	add_item(pointer, name.c_str(), xor("int"));
	*pointer = value;
}

void C_ConfigManager::setup_item(bool * pointer, bool value, const std::string& name)
{
	add_item(pointer, name.c_str(), xor("bool"));
	*pointer = value;
}

void C_ConfigManager::setup_item(float * pointer, float value, const std::string& name)
{
	add_item(pointer, name.c_str(), xor("float"));
	*pointer = value;
}

void C_ConfigManager::setup_item(std::vector< int > * pointer, int size, const std::string& name) 
{
	add_item(pointer, name.c_str(), xor("vector<int>"));
	pointer->clear();

	for (int i = 0; i < size; i++)
		pointer->push_back(FALSE);
}

void C_ConfigManager::setup_item(std::vector< std::string >* pointer, const std::string& name)
{ 
	add_item(pointer, name.c_str(), xor("vector<string>"));
}

void C_ConfigManager::setup_item(std::string * pointer, const std::string& value, const std::string& name)
{
	add_item(pointer, name.c_str(), xor("string"));
	*pointer = value;
}

void C_ConfigManager::save(std::string config)
{
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + xor("\\wannacry\\configs\\");
			file = std::string(path) + xor("\\wannacry\\configs\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();
	json allJson;

	for (auto it : items)
	{
		json j;

		j[xor("name")] = it->name;
		j[xor("type")] = it->type;

		if (!it->type.compare(xor("int"))) 
			j[xor("value")] = (int)*(int*)it->pointer;
		else if (!it->type.compare(xor("float"))) 
			j[xor("value")] = (float)*(float*)it->pointer;
		else if (!it->type.compare(xor("bool"))) 
			j[xor("value")] = (bool)*(bool*)it->pointer;
		else if (!it->type.compare(xor("vector<int>")))
		{
			auto& ptr = *(std::vector<int>*)(it->pointer);
			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j[xor("value")] = ja.dump();
		}
		else if (!it->type.compare(xor("vector<string>")))
		{
			auto& ptr = *(std::vector<std::string>*)(it->pointer);
			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j[xor("value")] = ja.dump();
		}
		else if (!it->type.compare(xor("string"))) 
			j[xor("value")] = (std::string)*(std::string*)it->pointer;

		allJson.push_back(j);
	}

	std::string data;

	Base64 base64;
	auto json_dump = allJson.dump();
	base64.encode(json_dump, &data);

	std::ofstream ofs;
	ofs.open(file + '\0', std::ios::out | std::ios::trunc);

	ofs << std::setw(4) << data << std::endl;
	ofs.close();
}

void C_ConfigManager::load(std::string config)
{
	static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem* 
	{
		for (int i = 0; i < (int)items.size(); i++) //-V202
			if (!items[i]->name.compare(name))
				return items[i];

		return nullptr;
	};

	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() ->void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + xor("\\wannacry\\configs\\");
			file = std::string(path) + xor("\\wannacry\\configs\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();
	std::string data;

	std::ifstream ifs;
	ifs.open(file + '\0');

	ifs >> data;
	ifs.close();

	if (data.empty())
		return;

	Base64 base64;

	std::string decoded_data;
	base64.decode(data, &decoded_data);

	std::ofstream ofs;
	ofs.open(file + '\0', std::ios::out | std::ios::trunc);

	ofs << decoded_data;
	ofs.close();

	json allJson;

	std::ifstream ifs_final;
	ifs_final.open(file + '\0');

	ifs_final >> allJson;
	ifs_final.close();

	auto json_dump = allJson.dump();
	base64.encode(json_dump, &data);

	std::ofstream final_ofs;
	final_ofs.open(file + '\0', std::ios::out | std::ios::trunc);

	final_ofs << data;
	final_ofs.close();

	for (auto it = allJson.begin(); it != allJson.end(); ++it)
	{
		json j = *it;

		std::string name = j[xor("name")];
		std::string type = j[xor("type")];

		auto item = find_item(items, name);

		if (item)
		{
			if (!type.compare(xor("int")))
				*(int*)item->pointer = j[xor("value")].get<int>(); //-V206
			else if (!type.compare(xor("float")))
				*(float*)item->pointer = j[xor("value")].get<float>();
			else if (!type.compare(xor("bool")))
				*(bool*)item->pointer = j[xor("value")].get<bool>();
			else if (!type.compare(xor("vector<int>")))
			{
				auto ptr = static_cast<std::vector <int>*> (item->pointer);
				ptr->clear();

				json ja = json::parse(j[xor("value")].get<std::string>().c_str());

				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					ptr->push_back(*it);
			}
			else if (!type.compare(xor("vector<string>")))
			{
				auto ptr = static_cast<std::vector <std::string>*> (item->pointer);
				ptr->clear();

				json ja = json::parse(j[xor("value")].get<std::string>().c_str());

				for (json::iterator it = ja.begin(); it != ja.end(); ++it)
					ptr->push_back(*it);
			}
			else if (!type.compare(xor("string")))
				*(std::string*)item->pointer = j[xor("value")].get<std::string>();
		}
	}
}

void C_ConfigManager::remove(std::string config)
{
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + xor("\\wannacry\\configs\\");
			file = std::string(path) + xor("\\wannacry\\configs\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();

	std::string path = file + '\0';
	std::remove(path.c_str());
}

void C_ConfigManager::config_files()
{
	std::string folder;

	auto get_dir = [&folder]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
			folder = std::string(path) + xor("\\wannacry\\configs\\");

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();
	files.clear();

	std::string path = folder + xor("/*.wc");
	WIN32_FIND_DATA fd;

	HANDLE hFind = FindFirstFile(path.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(fd.cFileName);
		} 
		while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}
}
