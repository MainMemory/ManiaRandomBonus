// ManiaRandomBonus.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\mania-mod-loader\ManiaModLoader\include\ManiaModLoader.h"
#include "..\mod-loader-common\ModLoaderCommon\IniFile.hpp"
#include <algorithm>

using std::string;
using std::unordered_map;

short normal()
{
	return rand() & 31;
}

short original()
{
	return 32;
}

short mania()
{
	return 33;
}

short normal_original()
{
	return rand() & 1 ? original() : normal();
}

short normal_mania()
{
	return rand() & 1 ? mania() : normal();
}

short original_mania()
{
	return rand() & 1 ? mania() : original();
}

short all()
{
	switch (rand() % 3)
	{
	default:
		return normal();
	case 1:
		return original();
	case 2:
		return mania();
	}
}

unordered_map<string, short(__cdecl *)()> optfuncmap = {
	{ "normal", normal },
	{ "original", original },
	{ "mania", mania },
	{ "normal+original", normal_original },
	{ "normal+mania", normal_mania },
	{ "original+mania", original_mania },
	{ "all", all }
};

extern "C"
{
	__declspec(dllexport) void Init(const char *path)
	{
		char pathbuf[MAX_PATH];
		strcpy_s(pathbuf, path);
		strcat_s(pathbuf, "\\config.ini");
		const IniFile *cfg = new IniFile(pathbuf);
		string option = cfg->getString("", "Mode");
		delete cfg;
		std::transform(option.begin(), option.end(), option.begin(), tolower);
		WriteCall((void*)0x458014, optfuncmap[option]);
		WriteData<4>((void*)0x458022, 0x90u);
	}

	__declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}