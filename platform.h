// Will Eves 2022
// This project is helpfull if you need to
// manage large cross-platform c++ apps
// 
// To enable a platform in the build, define its support macro
// 
// Support Macros:
// 
// Windows : WINDOWS_SUPPORT
// Mac : MAC_SUPPORT
// Maccatalyst : MACCATALYST_SUPPORT
// iOS : IOS_SUPPORT
// Linux : LINUX_SUPPORT
// Unix : UNIX_SUPPORT
// Posix : POSIX_SUPPORT
// Android : ANDROID_SUPPORT

#include <iostream>
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define WINDOWS
	#define CONFIG
	#ifdef _WIN64
		#define WINDOWS
		#define CONFIG_64
	#else
		#define WINDOWS
		#define CONFIG_32
	#endif
	#elif __APPLE__
		#include <TargetConditionals.h>
		#if TARGET_IPHONE_SIMULATOR
			#define SIMULATOR
			#define IOS
		#elif TARGET_OS_MACCATALYST
			#define MACCATALYST
		#elif TARGET_OS_IPHONE
			#define IOS
		#elif TARGET_OS_MAC
			#define MAC
		#else
			#error "Unknown Apple platform"
		#endif
	#elif __linux__
		#define LINUX
	#elif __unix__
		#define UNIX
	#elif defined(_POSIX_VERSION)
		#define POSIX
	#elif __ANDROID__
		#define ANDROID
	#else
		#error "Unknown compiler"
#endif

namespace Platform {
	struct Color {
		int ci = 0;
		std::string cs = "";
		Color(int i) {
			ci = i;
		}
		Color(std::string s) {
			cs = s;
		}
	};

	void ShowConsole();
	void HideConsole();
	void CloseConsole();
	void ClearConsole();
	void SetConsoleColor(Color color);
	void Print(std::string data, bool brk = false);
}

#ifdef WINDOWS
	#ifndef WINDOWS_SUPPORT
		#error "Windows not supported!"
	#endif

#include <Windows.h>

#define CONSOLE_BLACK Platform::Color(0)
#define CONSOLE_RED Platform::Color(12)
#define CONSOLE_GREEN Platform::Color(10)
#define CONSOLE_BLUE Platform::Color(9)
#define CONSOLE_WHITE Platform::Color(15)
#define CONSOLE_YELLOW Platform::Color(14)

void Platform::ShowConsole() {
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
}

void Platform::HideConsole() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void Platform::CloseConsole() {
	system("exit");
}

void Platform::ClearConsole() {
	system("cls");
}

void Platform::SetConsoleColor(Color color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color.ci);
}

void Platform::Print(std::string data, bool brk) {
	std::cout << data;
	if (brk) std::cout << "\n";
}
#endif

#ifdef MAC
	#ifndef MAC_SUPPORT
		#error "Mac not supported!"
	#endif
#endif

#ifdef MACCATALYST
	#ifndef MACCATALYST_SUPPORT
		#error "Maccatalyst not supported!"
	#endif
#endif

#ifdef IOS
	#ifndef IOS_SUPPORT
		#error "iOS not supported!"
	#endif
#endif

#ifdef LINUX
	#ifndef LINUX_SUPPORT
		#error "Linux not supported"
	#endif

#include <stdlib.h>

#define CONSOLE_BLACK Platform::Color("\033[30m")
#define CONSOLE_RED Platform::Color("\033[31m")
#define CONSOLE_GREEN Platform::Color("\033[32m")
#define CONSOLE_BLUE Platform::Color("\033[34m")
#define CONSOLE_WHITE Platform::Color("\033[38m")
#define CONSOLE_YELLOW Platform::Color("\033[33m")

namespace Platform {
	std::string currentColor = "\033[38m";
}

void Platform::ShowConsole() {
	//lol
}

void Platform::HideConsole() {
	//lol
}

void Platform::CloseConsole() {
	system("exit");
}

void Platform::ClearConsole() {
	system("clear");
}

void Platform::SetConsoleColor(Color color) {
	currentColor = color.cs;
}

void Platform::Print(std::string data, bool brk) {
	std::cout << currentColor << data;
	if (brk) std::cout << "\n";
}
#endif

#ifdef UNIX
	#ifndef UNIX_SUPPORT
		#error "Unix not supported!"
	#endif
#endif

#ifdef POSIX
	#ifndef POSIX_SUPPORT
		#error "Posix not supported!"
	#endif
#endif

#ifdef ANDROID
	#ifndef ANDROID_SUPPORT
		#error "Android not supported!"
	#endif
#endif