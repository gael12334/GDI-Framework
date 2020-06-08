#ifndef PRESETS_H__
#define PRESETS_H__
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <math.h>

#define malloc_cpp(type, count) (type*) new char[sizeof(type) * count]
#define free_cpp(ptr) delete[] (char*)ptr;

#define WIN32_EVENT_PARAM HWND win, UINT msg, WPARAM wp, LPARAM lp 
#define WIN32_MSGLOOP(msg) while(GetMessage(&msg, 0, 0, 0)) {TranslateMessage(&msg); DispatchMessage(&msg);}

#define BITMAP_FILENOTFOUND "Bitmap: File not found.", 1
#define BITMAP_UNSUPPORTEDFORMAT "Bitmap format is not supported.", 2
#define BITMAP_UNSUPPORTED_BITPERPIXEL "24 bits per pixel is the only format supported.", 3
#define WINDOW_BITBLTERROR "The bitblt has failed.", 4

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

namespace GDI {

	/// \brief Function returning an instance of type "T", with its byte all set at NULL (0). 
	template <class T> T MakeDummy() {
		uint sizeT = sizeof(T);
		char * dataT = new char[sizeT];
		for (uint i = 0; i < sizeT; i++)
			dataT[i] = 0;
		T instT = *(T*)dataT;
		delete[] dataT;
		return instT;
	}

	/// \brief Class handling simili 24 bits integers. Used for RGB codes.
	struct Int24 {
		char low, medium, high;
	};

	/// \brief Class informing about the last error occuring in the system.
	struct LastErrorInfo {
		static const char * message;
		static ushort error;

		inline static void SetError(const char * msg, ushort err) {
			message = msg;
			error = err;
		}
	};
	const char * LastErrorInfo::message = nullptr;
	ushort LastErrorInfo::error = 0;

	
}


#endif
