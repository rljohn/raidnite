#pragma once

#include "engine/types.h"

namespace raid 
{

class FileUtil
{
public:

	static void SetProjectName(const std::wstring& name);

	static std::wstring GetAppDataFolder();

	static bool RemoveFile(const std::wstring& path);

	static bool CreateDirectoryForFile(const std::wstring& path);

private:

	static void Reset();
	static void Init();

	static std::wstring sm_ProjectName;
	static std::wstring sm_AppDataFolder;
};

} // namespace raid
