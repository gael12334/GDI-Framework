#ifndef EVENT_H__
#define EVENT_H__

#include "Shapes.h"

#define swcase(tag) case tag:
#define swdef() default:
#define swend break

namespace GDI {

	enum EventType {
		GDI_KEYUP = WM_KEYFIRST,
		GDI_KEYDOWN = WM_KEYDOWN,

		GDI_MOUSELEFT_UP = WM_LBUTTONUP,
		GDI_MOUSELEFT_DOWN = WM_LBUTTONDOWN,
		GDI_MOUSELEFT_DOUBLECLICK = WM_LBUTTONDBLCLK,

		GDI_MOUSERIGHT_UP = WM_MBUTTONUP,
		GDI_MOUSERIGHT_DOWN = WM_MBUTTONDOWN,
		GDI_MOUSERIGHT_DOUBLECLICK = WM_RBUTTONDBLCLK,

		GDI_MOUSEMIDDLE_UP = WM_MBUTTONUP,
		GDI_MOUSEMIDDLE_DOWN = WM_MBUTTONDOWN,
		GDI_MOUSEMIDDLE_DOUBLECLICK = WM_MBUTTONDBLCLK,
		GDI_MOUSEMIDDLE_SCROLL = WM_MOUSEWHEEL,

		GDI_MOUSEMOTION = WM_MOUSEMOVE,
		GDI_QUIT = WM_QUIT
	};

	enum VKType {
		GDIKEY_MOUSELEFT = VK_LBUTTON,
		GDIKEY_MOUSERIGHT = VK_RBUTTON,
		GDIKEY_MOUSEMIDDLE = VK_MBUTTON,
		
		GDIKEY_BACKSPACE = VK_BACK,
		GDIKEY_TAB = VK_TAB,
		GDIKEY_ENTER = VK_RETURN,
		GDIKEY_SHIFT = VK_SHIFT,
		GDIKEY_CTRL = VK_CONTROL,
		GDIKEY_CAPSLOCK = VK_CAPITAL,
		GDIKEY_ESCAPE = VK_ESCAPE,
		GDIKEY_SPACE = VK_SPACE,
		GDIKEY_PAGEUP = VK_PRIOR,
		GDIKEY_PAGEDOWN = VK_NEXT,
		GDIKEY_END = VK_END,
		GDIKEY_HOME = VK_HOME,
		GDIKEY_INSERT = VK_INSERT,
		GDIKEY_PRINTSCREEN = VK_SNAPSHOT,
		GDIKEY_DELETE = VK_DELETE,

		GDIKEY_LARROW = VK_LEFT,
		GDIKEY_RARROW = VK_RIGHT,
		GDIKEY_DARROW = VK_DOWN,
		GDIKEY_UARROW = VK_UP,

		GDIKEY_0 = 0X30,
		GDIKEY_1 = 0X31,
		GDIKEY_2 = 0X32,
		GDIKEY_3 = 0X33, 
		GDIKEY_4 = 0X34,
		GDIKEY_5 = 0X35,
		GDIKEY_6 = 0X36,
		GDIKEY_7 = 0X37,
		GDIKEY_8 = 0X38,
		GDIKEY_9 = 0X39,
		
		GDIKEY_A = 0X41, //65 (SEE ASCII CHART)
		GDIKEY_B = 0X42, 
		GDIKEY_C = 0X43,
		GDIKEY_D = 0X44,
		GDIKEY_E = 0X45,
		GDIKEY_F = 0X46,
		GDIKEY_G = 0X47,
		GDIKEY_H = 0X48,
		GDIKEY_I = 0X49,
		GDIKEY_J = 0X4A,
		GDIKEY_K = 0X4B,
		GDIKEY_L = 0X4C,
		GDIKEY_M = 0X4D,
		GDIKEY_N = 0X4E,
		GDIKEY_O = 0X4F,
		GDIKEY_P = 0X50,
		GDIKEY_Q = 0X51,
		GDIKEY_R = 0X52,
		GDIKEY_S = 0X53,
		GDIKEY_T = 0X54,
		GDIKEY_U = 0X55,
		GDIKEY_V = 0X56,
		GDIKEY_W = 0X57,
		GDIKEY_X = 0X58,
		GDIKEY_Y = 0X59,
		GDIKEY_Z = 0X5A,

		GDIKEY_MULTIPLY = VK_MULTIPLY,
		GDIKEY_DIVIDE = VK_DIVIDE,
		GDIKEY_DECIMAL = VK_DECIMAL,
		GDIKEY_ADD = VK_ADD,
		GDIKEY_SUBSTRACT = VK_SUBTRACT,
	};

	class Event {
		static MSG info;
	public:

		static char KeyChar() {
			switch (info.message) {
				swcase(EventType::GDI_KEYDOWN) {
					VKType key = (VKType)info.wParam;

					if ((key >= VKType::GDIKEY_0) && (key <= VKType::GDIKEY_9))
						return key;

					if ((key >= VKType::GDIKEY_A) && (key <= VKType::GDIKEY_Z))
						return key;

					switch (key) {
						swcase(VKType::GDIKEY_ENTER)
							return '\r';

						swcase(VKType::GDIKEY_TAB)
							return '\t';

						swcase(VKType::GDIKEY_SPACE)
							return ' ';

						swdef()
							return '\0';
					}
					swend;
				}

				swdef()
					return '\0';
			}
		}
	
		static inline VKType KeyPressed() {
			if (info.message == EventType::GDI_KEYDOWN)
				return (VKType)info.wParam;
		}

		static inline EventType Type() {
			return (EventType)info.message;
		}

		static inline bool PollEvent() {
			return GetMessage(&info, 0, 0, 0);
		}
	
		static inline POINT MousePosition() {
			return info.pt;
		}
		
		static inline MSG Instance() {
			return info;
		}
	};

	MSG Event::info; 
}

#endif
