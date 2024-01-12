
// AppHeader.h : header file
//

#pragma once

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif



#if defined(UHS_OS_WIN)

#include <sdkddkver.h>
//#include <sdkddkver.h>用于解决下面的编译警告：
//1 > Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately.For example :
//1 > -add - D_WIN32_WINNT = 0x0601 to the compiler command line; or
//1 > -add _WIN32_WINNT = 0x0601 to your project's Preprocessor Definitions.

#define BackSlash_Str								"\\"				// 反斜杠

#elif defined(UHS_OS_LINUX)

#define BackSlash_Str								"/"					// 反斜杠

#elif defined(UHS_OS_OSX)

#else

#endif


// if (0 == std::string.compare("IA64"))) // 字符串比较，大小写敏感
// if (0 == icompare(clsPath.getExtension(), "dmp")) // 字符串比较，大小写不敏感


#include <iostream>
#include <fstream>
#include <thread>
#include <codecvt>
#include <utility>
#include <map>
#include <list>
#include <vector>


//#include <boost/locale.hpp>

