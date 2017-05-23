#pragma once

#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOHELP
#define WIN32_LEAN_AND_MEAN
#define _CRTDBG_MAP_ALLOC

#include <Windows.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <variant>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <memory>
#include <chrono>
#include <functional>

#include <cstdlib>
#include <crtdbg.h>

using namespace DirectX;

// DBG_NEW for crtdbg
#ifdef _DEBUG
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

// assert
#ifdef _DEBUG
#define Assert(condition, msg) if(condition) {} else { __debugbreak(); }
#else
#define Assert(condition) ()
#endif

// Helper class for COM exceptions.
class com_exception: public std::exception
{
public:
	com_exception(HRESULT hr): result(hr) {}

	virtual const char* what() const override
	{
		static char s_str[64] = { 0 };
		sprintf_s(s_str, "Failure with HRESULT of %08X", result);
		return s_str;
	}

private:
	HRESULT result;
};

// Helper utility converts D3D API failures into exceptions.
inline void throw_if_failed(HRESULT hr)
{
	if (FAILED(hr))
		throw com_exception(hr);
}

inline void release_com(IUnknown* com)
{
	if (com)
		com->Release();
}