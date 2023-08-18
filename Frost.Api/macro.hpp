#pragma once
#if defined(FROST_BUILD_DLL)
#define FROST_API __declspec(dllexport)
#else
#define FROST_API __declspec(dllimport)
#endif