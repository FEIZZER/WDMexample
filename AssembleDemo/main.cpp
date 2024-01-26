
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
//int g_Count = 100;
//
//void MyPrint() {
//	return;
//}
//
//int Add(int a, int& b)
//{
//	return a + b;
//}
//
//int& GetCount() {
//	int num = 10;
//	num--;
//	return g_Count;
//}
int main()
{


	std::wstring ss = L"ch";
	printf("%d \n", ss.length());
	return 0;
	size_t nPos, nStart = 0;
	std::wstring pFilePath = L"E:\\workbench\\cppORc template\\x64\\Release\\11_问问|22_中文|344_问|44_";
	boost::filesystem::wpath wstrFilePath(pFilePath);
	boost::filesystem::wpath wstrFileDir = wstrFilePath.parent_path();
	boost::filesystem::wpath wstrFileName = wstrFilePath.filename();
	wprintf(L"parent_path:%s, file_name:%s\n", 
		wstrFileDir.wstring().c_str(), wstrFileName.wstring().c_str());
	std::vector<std::wstring> wstrTempNames;
	boost::split(wstrTempNames, wstrFileName.wstring(), boost::is_any_of(L"|"));

	boost::filesystem::wpath wstrDstFolder;
	boost::filesystem::wpath wstrDstPath;
	int underlineIdx;
	for (const auto& wstrTempName : wstrTempNames)
	{
		underlineIdx = wstrTempName.find(L'_');
		if (underlineIdx != 2 ||
			underlineIdx == wstrTempName.length() - 1)
		{
			wprintf(L"bad dormat:%s\n", wstrTempName.c_str());
			continue;
		}

		if (!boost::filesystem::exists(wstrFileDir / wstrTempName))
		{
			wprintf(L"not find file:%s\n", wstrTempName.c_str());
			continue;
		}

		// move "133501553471012662_fileName" to "133501553471012662\\fileName", modify later.
		wstrDstFolder = wstrFileDir / wstrTempName.substr(0, underlineIdx);
		wstrDstPath = wstrDstFolder / wstrTempName.substr(underlineIdx + 1, wstrTempName.length());
		if (!boost::filesystem::exists(wstrDstFolder))
		{
			boost::filesystem::create_directories(wstrDstFolder);
		}
		else
		{
			// timestamp prefix must unique!
			wprintf(L"duplicate prefix:%s\n", wstrTempName.c_str());
			continue;
		}

		boost::filesystem::rename(wstrFileDir / wstrTempName, wstrDstPath);
		wprintf(L"dst:%s\n", wstrDstPath.wstring().c_str());
	}
}