#include "SazPCH.h"
#include "Window.h"

#include "Saz/GameTime.h"
#include "Saz/Screen.h"

#include <Core/Input.h>

namespace
{
	Input::EKeyboard ToKeyboard(KeyboardKey key)
	{
		switch (key)
		{
		// Alphabet
		case KEY_A: return Input::EKeyboard::A;
		case KEY_B: return Input::EKeyboard::B;
		case KEY_C: return Input::EKeyboard::C;
		case KEY_D: return Input::EKeyboard::D;
		case KEY_E: return Input::EKeyboard::E;
		case KEY_F: return Input::EKeyboard::F;
		case KEY_G: return Input::EKeyboard::G;
		case KEY_H: return Input::EKeyboard::H;
		case KEY_I: return Input::EKeyboard::I;
		case KEY_J: return Input::EKeyboard::J;
		case KEY_K: return Input::EKeyboard::K;
		case KEY_L: return Input::EKeyboard::L;
		case KEY_M: return Input::EKeyboard::M;
		case KEY_N: return Input::EKeyboard::N;
		case KEY_O: return Input::EKeyboard::O;
		case KEY_P: return Input::EKeyboard::P;
		case KEY_Q: return Input::EKeyboard::Q;
		case KEY_R: return Input::EKeyboard::R;
		case KEY_S: return Input::EKeyboard::S;
		case KEY_T: return Input::EKeyboard::T;
		case KEY_U: return Input::EKeyboard::U;
		case KEY_V: return Input::EKeyboard::V;
		case KEY_W: return Input::EKeyboard::W;
		case KEY_X: return Input::EKeyboard::X;
		case KEY_Y: return Input::EKeyboard::Y;
		case KEY_Z: return Input::EKeyboard::Z;

			// Numbers
		case KEY_ZERO: return Input::EKeyboard::Num_0;
		case KEY_ONE: return Input::EKeyboard::Num_1;
		case KEY_TWO: return Input::EKeyboard::Num_2;
		case KEY_THREE: return Input::EKeyboard::Num_3;
		case KEY_FOUR: return Input::EKeyboard::Num_4;
		case KEY_FIVE: return Input::EKeyboard::Num_5;
		case KEY_SIX: return Input::EKeyboard::Num_6;
		case KEY_SEVEN: return Input::EKeyboard::Num_7;
		case KEY_EIGHT: return Input::EKeyboard::Num_8;
		case KEY_NINE: return Input::EKeyboard::Num_9;

			// Function
		case KEY_F1:	return Input::EKeyboard::F1;
		case KEY_F2:	return Input::EKeyboard::F2;
		case KEY_F3:	return Input::EKeyboard::F3;
		case KEY_F4:	return Input::EKeyboard::F4;
		case KEY_F5:	return Input::EKeyboard::F5;
		case KEY_F6:	return Input::EKeyboard::F6;
		case KEY_F7:	return Input::EKeyboard::F7;
		case KEY_F8:	return Input::EKeyboard::F8;
		case KEY_F9:	return Input::EKeyboard::F9;
		case KEY_F10:	return Input::EKeyboard::F10;
		case KEY_F11:	return Input::EKeyboard::F11;
		case KEY_F12:	return Input::EKeyboard::F12;

			// Number Pad
		case KEY_KP_0:			return Input::EKeyboard::Numpad_0;
		case KEY_KP_1:			return Input::EKeyboard::Numpad_1;
		case KEY_KP_2:			return Input::EKeyboard::Numpad_2;
		case KEY_KP_3:			return Input::EKeyboard::Numpad_3;
		case KEY_KP_4:			return Input::EKeyboard::Numpad_4;
		case KEY_KP_5:			return Input::EKeyboard::Numpad_5;
		case KEY_KP_6:			return Input::EKeyboard::Numpad_6;
		case KEY_KP_7:			return Input::EKeyboard::Numpad_7;
		case KEY_KP_8:			return Input::EKeyboard::Numpad_8;
		case KEY_KP_9:			return Input::EKeyboard::Numpad_9;
		case KEY_KP_ADD:		return Input::EKeyboard::Numpad_Add;
		case KEY_KP_DECIMAL:	return Input::EKeyboard::Numpad_Decimal;
		case KEY_KP_DIVIDE:		return Input::EKeyboard::Numpad_Divide;
		case KEY_KP_ENTER:		return Input::EKeyboard::Numpad_Enter;
		case KEY_KP_EQUAL:		return Input::EKeyboard::Numpad_Equal;
		case KEY_KP_MULTIPLY:	return Input::EKeyboard::Numpad_Multiply;
		case KEY_KP_SUBTRACT:	return Input::EKeyboard::Numpad_Subtract;

			// Modifiers
		case KEY_LEFT_ALT:			return Input::EKeyboard::Alt_L;
		case KEY_LEFT_CONTROL:		return Input::EKeyboard::Control_L;
		case KEY_LEFT_SHIFT:		return Input::EKeyboard::Shift_L;
		case KEY_RIGHT_ALT:			return Input::EKeyboard::Alt_R;
		case KEY_RIGHT_CONTROL:		return Input::EKeyboard::Control_R;
		case KEY_RIGHT_SHIFT:		return Input::EKeyboard::Shift_R;

			// Arrow
		case KEY_UP:		return Input::EKeyboard::Up;
		case KEY_DOWN:		return Input::EKeyboard::Down;
		case KEY_LEFT:		return Input::EKeyboard::Left;
		case KEY_RIGHT:		return Input::EKeyboard::Right;

			// Others
		case KEY_APOSTROPHE:		return Input::EKeyboard::Apostrophe;
		case KEY_BACKSLASH:			return Input::EKeyboard::Backslash;
		case KEY_BACKSPACE:			return Input::EKeyboard::Backspace;
		case KEY_LEFT_BRACKET:		return Input::EKeyboard::Bracket_L;
		case KEY_RIGHT_BRACKET:		return Input::EKeyboard::Bracket_R;
		case KEY_CAPS_LOCK:			return Input::EKeyboard::Caps_Lock;
		case KEY_COMMA:				return Input::EKeyboard::Comma;
		case KEY_DELETE:			return Input::EKeyboard::Delete;
		case KEY_END:				return Input::EKeyboard::End;
		case KEY_ENTER:				return Input::EKeyboard::Enter;
		case KEY_EQUAL:				return Input::EKeyboard::Equal;
		case KEY_ESCAPE:			return Input::EKeyboard::Escape;
		case KEY_GRAVE:				return Input::EKeyboard::Grave;
		case KEY_HOME:				return Input::EKeyboard::Home;
		case KEY_INSERT:			return Input::EKeyboard::Insert;
		case KEY_MINUS:				return Input::EKeyboard::Minus;
		case KEY_NUM_LOCK:			return Input::EKeyboard::Numpad_Num_Lock;
		case KEY_PAGE_DOWN:			return Input::EKeyboard::Page_Down;
		case KEY_PAGE_UP:			return Input::EKeyboard::Page_Up;
		case KEY_PAUSE:				return Input::EKeyboard::Pause;
		case KEY_PERIOD:			return Input::EKeyboard::Period;
		case KEY_PRINT_SCREEN:		return Input::EKeyboard::Print_Screen;
		case KEY_SCROLL_LOCK:		return Input::EKeyboard::Scroll_Lock;
		case KEY_SEMICOLON:			return Input::EKeyboard::Semicolon;
		case KEY_SLASH:				return Input::EKeyboard::Slash;
		case KEY_SPACE:				return Input::EKeyboard::Space;
		case KEY_TAB:				return Input::EKeyboard::Tab;
		}
		return Input::EKeyboard::Unknown;
	}

	Input::EMouse ToMouse(MouseButton key)
	{
		switch (key)
		{
			case MOUSE_BUTTON_LEFT:			return Input::EMouse::Left;
			case MOUSE_BUTTON_RIGHT:		return Input::EMouse::Right;
			case MOUSE_BUTTON_MIDDLE:		return Input::EMouse::Middle;
			case MOUSE_BUTTON_SIDE:			return Input::EMouse::Button_4;
		}

		return Input::EMouse::Unknown;
	}
}

namespace Saz
{
	Window::Window(const Saz::WindowProps& props)
		: Saz::WindowBase(props)
	{
		m_Window = std::make_unique<raylib::Window>(props.m_Size.x, props.m_Size.y, props.m_Title);
	}

	Window::~Window()
	{
		
	}

	void Window::Init()
	{
		
	}

	void Window::Update(const Saz::GameTime& gameTime)
	{
		m_MousePos.x = static_cast<float>(GetMouseX());
		m_MousePos.y = static_cast<float>(GetMouseY());
		m_MouseDelta = vec2(GetMouseDelta().x, GetMouseDelta().y);
	}

	void Window::BeginDrawing()
	{
		::BeginDrawing();
		m_Window->ClearBackground(::BLUE);
	}

	void Window::EndDrawing()
	{
		::EndDrawing();
	}

	bool Window::ShouldClose() const
	{
		return WindowShouldClose();
	}

	void Window::GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const
	{
		for (int32_t i = KEY_NULL; i < KEY_KB_MENU; ++i)
		{
			const KeyboardKey key = static_cast<KeyboardKey>(i);
			if (IsKeyDown(key))
				out_Keys.insert(ToKeyboard(key));
		}
	}

	void Window::GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const
	{
		for (int32_t i = MOUSE_BUTTON_LEFT; i < MOUSE_BUTTON_BACK; ++i)
		{
			const MouseButton key = static_cast<MouseButton>(i);
			if (IsMouseButtonDown(key))
				out_Keys.insert(ToMouse(key));

			out_Delta = m_MouseDelta;
			out_Position = m_MousePos;
		}
	}
}