#include "engine/pch.h"
#include "engine/system/fileutil.h"
#include "engine/system/check.h"

#include <shlobj.h>
#include <Windows.h>
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib") 

namespace raid
{

std::wstring FileUtil::sm_ProjectName = L"Raidnite";
std::wstring FileUtil::sm_AppDataFolder = L"";

void FileUtil::Init()
{
	if (sm_AppDataFolder.empty())
	{
		wchar_t wszPath[MAX_PATH];
		if (SUCCEEDED(::SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, wszPath))) 
		{
			sm_AppDataFolder = wszPath;
			sm_AppDataFolder += L'\\';
			sm_AppDataFolder += sm_ProjectName;
		}
	}
}

void FileUtil::Reset()
{
	sm_AppDataFolder.clear();
	Init();
}

void FileUtil::SetProjectName(const std::wstring& name)
{
	sm_ProjectName = name;
	Reset();
}

std::wstring FileUtil::GetAppDataFolder()
{
	Init();
	return sm_AppDataFolder;
}

bool FileUtil::RemoveFile(const std::wstring& path)
{
	if (::DeleteFileW(path.c_str()))
	{
		return true;
	}
	else 
	{
		DWORD error = GetLastError();
		if (error == ERROR_FILE_NOT_FOUND) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool FileUtil::CreateDirectoryForFile(const std::wstring& path)
{
	stry
	{
		wchar_t wszDirectory[MAX_PATH];
		wcscpy_s(wszDirectory, path.c_str());

		// Returns 0 if nothing was modified
		scheckall(::PathRemoveFileSpecW(wszDirectory) != 0);

		// Verify we created a directory or it already exists
		DWORD err = ::SHCreateDirectoryExW(NULL, wszDirectory, NULL);
		scheckall(err == ERROR_SUCCESS || err == ERROR_ALREADY_EXISTS);

		return true;
	}
	sfinally
	{
		return false;
	}
}

} // namespace raid