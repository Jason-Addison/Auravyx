#include "MSVC/pch.h"
#include "Auravyx/Utility/Util.h"
#include <iostream>
#include <ShObjIdl_core.h>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <Logger/Log.h>
static HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void Util::logNNL(const int colour, const std::string& text)
{
	Log::out(text);
}

void Util::logNNL(const int colour, const std::string& who, const std::string& message)
{
	//logNNL(Colour::WHITE, "[");
	//logNNL(colour, who);
	//logNNL(Colour::WHITE, "] : ");
	//logNNL(Colour::WHITE, message);
}

void Util::log(const int colour, const std::string& text)
{
	Log::out(text);
}

void Util::log(const std::string& text)
{
	//log(Colour::WHITE, text);
}
void Util::log(const int out)
{
	//log(Colour::WHITE, std::to_string(out));
}
void Util::log(const float out)
{
	//log(Colour::WHITE, std::to_string(out));
}

void Util::log(const int colour, const std::string& who, const std::string& message)
{
	//logNNL(Colour::WHITE, "[");
	//logNNL(colour, who);
	//logNNL(Colour::WHITE, "] : ");
	//log(Colour::WHITE, message);
}

std::string Util::readTextFile(const std::string& location)
{
	std::ifstream inStream(location, std::ifstream::in);
	std::stringstream stream;
	stream << inStream.rdbuf();

	return stream.str();
}

int Util::typeOfFile(const std::string& path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			return 0;
		}
		else if (s.st_mode & S_IFREG)
		{
			return 1;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		return -1;
	}
}
std::vector<std::string> Util::listDirectory(const std::string& dir)
{
	std::vector<std::string> items;
	for (auto& p : std::filesystem::directory_iterator(dir))
	{
		std::ostringstream oss;
		oss << p.path();
		items.emplace_back(oss.str());
	}
	return items;
}

void Util::writeToFile(const std::string& dir, const std::string& data)
{
	std::ofstream file;
	file.open(dir);
	file << data;
	file.close();
}

int Util::createFolder(std::string& dir)
{
	std::wstring wdir(dir.length(), L' ');
	std::copy(dir.begin(), dir.end(), dir.begin());
	//if (CreateDirectory(wdir.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		return 1;
	}
	//else
	{
		return 0;
	}
}

std::string Util::replaceAll(std::string& str, const std::string& a, const std::string& b)
{
	try
	{
		std::replace(str.begin(), str.end(), a.at(0), b.at(0));
		return str;
	}
	catch (std::out_of_range e)
	{
		//Util::log(Colour::RED, "Failed to replace all in string '" + str + "'!");
	}
	return NULL;
}

std::string Util::removeAll(std::string str, const char a)
{
	try
	{
		str.erase(std::remove(str.begin(), str.end(), a), str.end());
		return str;
	}
	catch (std::out_of_range e)
	{
		//Util::log(Colour::RED, "Failed to remove all in string '" + str + "'!");
	}
	return NULL;
}

size_t Util::countCharsInString(const std::string& string, const char c)
{
	return std::count(string.begin(), string.end(), c);
}

bool Util::isCommentedLine(const std::string& line)
{
	int slashCount = 0;
	for (int i = 0; i < line.length(); i++)
	{
		if (line.at(i) == '/')
		{
			//std::cout << line.at(i) << "|\n";
		}
		if (isspace(line.at(i)))
		{
			if (slashCount > 0)
			{
				return false;
			}
		}
		else if (line.at(i) == '/')
		{
			slashCount++;
			if (slashCount >= 2)
			{
				return true;
			}
		}
	}
	return false;
}

std::string Util::getFileNameNoEXT(const std::string& path, const std::string& ext)
{
	char sep = '/';

	int os = 0;
	if (os)
	{
		sep = '\\';
	}

	unsigned long long k = path.rfind(sep, path.length());
	std::string fileNameNoExt = path;
	if (k != std::string::npos)
	{
		fileNameNoExt = fileNameNoExt.substr(k + 1, fileNameNoExt.length() - k);
		fileNameNoExt = fileNameNoExt.substr(fileNameNoExt.length() - ext.length());
	}
	return fileNameNoExt;
}

bool Util::endsWith(const std::string& fullString, const std::string& ending)
{
	if (fullString.length() >= ending.length())
	{
		if (fullString.compare(fullString.length() - ending.length(), ending.length(), ending))
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}
std::vector<std::string> Util::splitString(const std::string& string, const std::string& splitter)
{
	std::vector<std::string> splitStr;
	int lastIndex = 0;
	for (int i = 0; i < string.length(); i++)
	{
		if (string.at(i) == splitter.at(0))
		{
			std::string split = string.substr(lastIndex, i - lastIndex);
			lastIndex = i + 1;
			if (split.length() > 0)
			{
				splitStr.emplace_back(split);
			}
		}
	}
	std::string split = string.substr(lastIndex, string.length() - lastIndex);
	if (split.length() > 0)
	{
		splitStr.emplace_back(split);
	}
	return splitStr;
}
std::vector<std::string> Util::splitString(const std::string& string, const char splitter)
{
	std::vector<std::string> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = string.find(splitter, prev)) != std::string::npos)
	{
		strings.push_back(string.substr(prev, pos - prev));
		prev = pos + 1;
	}

	strings.push_back(string.substr(prev));
	return strings;
}
std::string Util::removeFromString(std::string& string, const std::string& remove, const std::string& replace)
{
	for (int i = 0; i < string.length(); i++)
	{
		if (string.at(i) == remove.at(0))
		{
			string.at(i) = replace.at(0);
		}
	}
	return string;
}
bool Util::startsWith(const std::string& string, const std::string& startsWith)
{
	for (int i = 0; i < startsWith.length(); i++)
	{
		if (startsWith.at(i) != string.at(i))
		{
			return false;
		}
	}
	return true;
}

std::string Util::createUUID()
{
	UUID uuid;
	UuidCreate(&uuid);

	char* str;
	UuidToStringA(&uuid, (RPC_CSTR*)& str);
	//RpcStringFreeA((RPC_CSTR*)&str);
	return std::string(str);
}

bool Util::pointAABB(const float px, const float py, const float x, const float y, const float width, const float height)
{
	if (px >= x && py >= y &&
		px <= x + width && py <= y + height)
	{
		return true;
	}
	return false;
}

std::string Util::removeDecimal(const double x, const int precision)
{
	std::stringstream ss;
	ss << std::fixed;
	ss.precision(precision); 
	ss << x;
	return ss.str();
}

std::string Util::getFile(const COMDLG_FILTERSPEC& pFilter, const LPCWSTR& title, const LPCWSTR& defaultExtension, const LPCWSTR& dir)
{
	return getFile(pFilter, title, defaultExtension, dir, 0);
}

std::string Util::getFolder(const LPCWSTR& title, const LPCWSTR& dir)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		CoInitialize(NULL);

		IFileOpenDialog* pFileOpen;

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		pFileOpen->SetFileTypeIndex(1);
		IShellItem* psiFolder;
		LPCWSTR szFilePath = dir;
		HRESULT hr2 = SHCreateItemFromParsingName(szFilePath, NULL, IID_PPV_ARGS(&psiFolder));
		if (SUCCEEDED(hr))
		{
			hr2 = pFileOpen->SetFolder(psiFolder);
			pFileOpen->SetOptions(FOS_PICKFOLDERS);
		}

		if (SUCCEEDED(hr))
		{
			pFileOpen->SetTitle(title);

			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						//std::string rtn = CW2A(pszFilePath);
						CoTaskMemFree(pszFilePath);
						//return rtn;
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return "Not found";
}

std::string Util::getFile(const COMDLG_FILTERSPEC& pFilter, const LPCWSTR& title, const LPCWSTR& defaultExtension, const LPCWSTR& dir, const int mode)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		CoInitialize(NULL);

		IFileOpenDialog* pFileOpen;

		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		pFileOpen->SetFileTypes(1, &pFilter);
		pFileOpen->SetDefaultExtension(defaultExtension);
		pFileOpen->SetFileTypeIndex(1);
		IShellItem* psiFolder;
		LPCWSTR szFilePath = dir;
		HRESULT hr2 = SHCreateItemFromParsingName(szFilePath, NULL, IID_PPV_ARGS(&psiFolder));
		if (SUCCEEDED(hr))
		{
			hr2 = pFileOpen->SetFolder(psiFolder);
			if (mode == 1)
			{
				pFileOpen->SetOptions(FOS_PICKFOLDERS);
			}
		}

		if (SUCCEEDED(hr))
		{
			pFileOpen->SetTitle(title);

			hr = pFileOpen->Show(NULL);

			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						//std::string rtn = CW2A(pszFilePath);
						CoTaskMemFree(pszFilePath);
						//return rtn;
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return "Not found";
}

bool Util::saveFile()
{
	/*LPWSTR fileName = L"D:/Music";
	HWND hwndOwner = WindowManager::getWindow()->getHWND();
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize = sizeof(ofn);
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;
	ofn.hInstance = GetModuleHandle(0);
	ofn.hwndOwner = hwndOwner;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFile = fileName;
	ofn.nFilterIndex = 1;
	ofn.lpstrFilter = L"Text file\0*.txt\0Any file\0*.*\0";
	ofn.lpstrDefExt = L"txt";*/
	return 0;// GetSaveFileName(&ofn);
}
