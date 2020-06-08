#ifndef WINDOW_H__
#define WINDOW_H__

#include "Event.h"

namespace GDI {
	class Window {
		Bitmap* data;
		HWND hwnd;
		HDC hdc;
		HINSTANCE hinst;

		// GDI BlitBlt members.
		HDC hdc_t;
		HBITMAP bitmap;
		HGDIOBJ old;

		inline static LRESULT CALLBACK dummy_function(WIN32_EVENT_PARAM) {
			return DefWindowProc(win, msg, wp, lp);
		}

	public:

		Window(const char * title, Point position, Point size, HWND parent = nullptr, DWORD extendedStyle = NULL, HMENU menu = nullptr, LPVOID parameter = nullptr) {
			data = new Bitmap({ 0, 0, 0, 255 }, size);
			HINSTANCE hinst = (HINSTANCE)GetModuleHandle(NULL);
			LPCTSTR className = "windowClass";

			WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_OWNDC, dummy_function, 0, 0, GetModuleHandle(NULL), LoadIcon(0,IDI_APPLICATION), LoadCursor(0,IDC_ARROW), HBRUSH(RGB(0, 0, 0)), 0, className, LoadIcon(0,IDI_APPLICATION) };
			RegisterClassEx(&wcex);

			hwnd = CreateWindowEx(extendedStyle, className, title, WS_VISIBLE | WS_TILED, CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y, parent, menu, hinst, parameter);
			hdc = GetDC(hwnd);
		}

		~Window() {
			delete data;
			DeleteDC(hdc);
			DestroyWindow(hwnd);
		}

		void Refresh() {
			hdc_t = CreateCompatibleDC(hdc);
			bitmap = CreateDIBSection(hdc_t, &data->Info(), DIB_RGB_COLORS, (void**)data->Pixels(), NULL, 0);
			old = SelectObject(hdc_t, bitmap);
		}

		void Present() {
			Point size = data->Size();
			if (SetDIBits(hdc_t, bitmap, 0, -size.y, data->Pixels(), &data->Info(), DIB_RGB_COLORS))
				LastErrorInfo::SetError(WINDOW_BITBLTERROR);
			else {
				bool r = BitBlt(hdc, 0, 0, size.x, size.y, hdc_t, 0, 0, SRCCOPY);
				r = 1;
			}
			SelectObject(hdc_t, old);
			DeleteDC(hdc_t);
			DeleteObject(old);
			DeleteObject(bitmap);
			GdiFlush();
			UpdateWindow(hwnd);
		}

		inline void Notify() {
			MSG msg = Event::Instance();
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		void DrawBitmap(Bitmap* image, RECT viewOnDest, RECT regionOfSrc) {

		}

	};
}
#endif
