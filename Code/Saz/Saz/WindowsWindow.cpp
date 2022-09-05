#include "SazPCH.h"
#include "WindowsWindow.h"

#include "Saz/GameTime.h"
#include "Saz/Screen.h"

#include <Core/Input.h>
#include "glad/glad.h"

namespace
{
	Input::EKeyboard ToKeyboard(int32_t key)
	{
		switch (key)
		{
			// Alphabet
		case GLFW_KEY_A: return Input::EKeyboard::A;
		case GLFW_KEY_B: return Input::EKeyboard::B;
		case GLFW_KEY_C: return Input::EKeyboard::C;
		case GLFW_KEY_D: return Input::EKeyboard::D;
		case GLFW_KEY_E: return Input::EKeyboard::E;
		case GLFW_KEY_F: return Input::EKeyboard::F;
		case GLFW_KEY_G: return Input::EKeyboard::G;
		case GLFW_KEY_H: return Input::EKeyboard::H;
		case GLFW_KEY_I: return Input::EKeyboard::I;
		case GLFW_KEY_J: return Input::EKeyboard::J;
		case GLFW_KEY_K: return Input::EKeyboard::K;
		case GLFW_KEY_L: return Input::EKeyboard::L;
		case GLFW_KEY_M: return Input::EKeyboard::M;
		case GLFW_KEY_N: return Input::EKeyboard::N;
		case GLFW_KEY_O: return Input::EKeyboard::O;
		case GLFW_KEY_P: return Input::EKeyboard::P;
		case GLFW_KEY_Q: return Input::EKeyboard::Q;
		case GLFW_KEY_R: return Input::EKeyboard::R;
		case GLFW_KEY_S: return Input::EKeyboard::S;
		case GLFW_KEY_T: return Input::EKeyboard::T;
		case GLFW_KEY_U: return Input::EKeyboard::U;
		case GLFW_KEY_V: return Input::EKeyboard::V;
		case GLFW_KEY_W: return Input::EKeyboard::W;
		case GLFW_KEY_X: return Input::EKeyboard::X;
		case GLFW_KEY_Y: return Input::EKeyboard::Y;
		case GLFW_KEY_Z: return Input::EKeyboard::Z;

			// Numbers
		case GLFW_KEY_0: return Input::EKeyboard::Num_0;
		case GLFW_KEY_1: return Input::EKeyboard::Num_1;
		case GLFW_KEY_2: return Input::EKeyboard::Num_2;
		case GLFW_KEY_3: return Input::EKeyboard::Num_3;
		case GLFW_KEY_4: return Input::EKeyboard::Num_4;
		case GLFW_KEY_5: return Input::EKeyboard::Num_5;
		case GLFW_KEY_6: return Input::EKeyboard::Num_6;
		case GLFW_KEY_7: return Input::EKeyboard::Num_7;
		case GLFW_KEY_8: return Input::EKeyboard::Num_8;
		case GLFW_KEY_9: return Input::EKeyboard::Num_9;

			// Function
		case GLFW_KEY_F1:	return Input::EKeyboard::F1;
		case GLFW_KEY_F2:	return Input::EKeyboard::F2;
		case GLFW_KEY_F3:	return Input::EKeyboard::F3;
		case GLFW_KEY_F4:	return Input::EKeyboard::F4;
		case GLFW_KEY_F5:	return Input::EKeyboard::F5;
		case GLFW_KEY_F6:	return Input::EKeyboard::F6;
		case GLFW_KEY_F7:	return Input::EKeyboard::F7;
		case GLFW_KEY_F8:	return Input::EKeyboard::F8;
		case GLFW_KEY_F9:	return Input::EKeyboard::F9;
		case GLFW_KEY_F10:	return Input::EKeyboard::F10;
		case GLFW_KEY_F11:	return Input::EKeyboard::F11;
		case GLFW_KEY_F12:	return Input::EKeyboard::F12;
		case GLFW_KEY_F13:	return Input::EKeyboard::F13;
		case GLFW_KEY_F14:	return Input::EKeyboard::F14;
		case GLFW_KEY_F15:	return Input::EKeyboard::F15;
		case GLFW_KEY_F16:	return Input::EKeyboard::F16;
		case GLFW_KEY_F17:	return Input::EKeyboard::F17;
		case GLFW_KEY_F18:	return Input::EKeyboard::F18;
		case GLFW_KEY_F19:	return Input::EKeyboard::F19;
		case GLFW_KEY_F20:	return Input::EKeyboard::F20;
		case GLFW_KEY_F21:	return Input::EKeyboard::F21;
		case GLFW_KEY_F22:	return Input::EKeyboard::F22;
		case GLFW_KEY_F23:	return Input::EKeyboard::F23;
		case GLFW_KEY_F24:	return Input::EKeyboard::F24;
		case GLFW_KEY_F25:	return Input::EKeyboard::F25;

			// Number Pad
		case GLFW_KEY_KP_0:			return Input::EKeyboard::Numpad_0;
		case GLFW_KEY_KP_1:			return Input::EKeyboard::Numpad_1;
		case GLFW_KEY_KP_2:			return Input::EKeyboard::Numpad_2;
		case GLFW_KEY_KP_3:			return Input::EKeyboard::Numpad_3;
		case GLFW_KEY_KP_4:			return Input::EKeyboard::Numpad_4;
		case GLFW_KEY_KP_5:			return Input::EKeyboard::Numpad_5;
		case GLFW_KEY_KP_6:			return Input::EKeyboard::Numpad_6;
		case GLFW_KEY_KP_7:			return Input::EKeyboard::Numpad_7;
		case GLFW_KEY_KP_8:			return Input::EKeyboard::Numpad_8;
		case GLFW_KEY_KP_9:			return Input::EKeyboard::Numpad_9;
		case GLFW_KEY_KP_ADD:		return Input::EKeyboard::Numpad_Add;
		case GLFW_KEY_KP_DECIMAL:	return Input::EKeyboard::Numpad_Decimal;
		case GLFW_KEY_KP_DIVIDE:	return Input::EKeyboard::Numpad_Divide;
		case GLFW_KEY_KP_ENTER:		return Input::EKeyboard::Numpad_Enter;
		case GLFW_KEY_KP_EQUAL:		return Input::EKeyboard::Numpad_Equal;
		case GLFW_KEY_KP_MULTIPLY:	return Input::EKeyboard::Numpad_Multiply;
		case GLFW_KEY_KP_SUBTRACT:	return Input::EKeyboard::Numpad_Subtract;

			// Modifiers
		case GLFW_KEY_LEFT_ALT:			return Input::EKeyboard::Alt_L;
		case GLFW_KEY_LEFT_CONTROL:		return Input::EKeyboard::Control_L;
		case GLFW_KEY_LEFT_SHIFT:		return Input::EKeyboard::Shift_L;
		case GLFW_KEY_RIGHT_ALT:		return Input::EKeyboard::Alt_R;
		case GLFW_KEY_RIGHT_CONTROL:	return Input::EKeyboard::Control_R;
		case GLFW_KEY_RIGHT_SHIFT:		return Input::EKeyboard::Shift_R;

			// Arrow
		case GLFW_KEY_UP:		return Input::EKeyboard::Up;
		case GLFW_KEY_DOWN:		return Input::EKeyboard::Down;
		case GLFW_KEY_LEFT:		return Input::EKeyboard::Left;
		case GLFW_KEY_RIGHT:	return Input::EKeyboard::Right;

			// Others
		case GLFW_KEY_APOSTROPHE:		return Input::EKeyboard::Apostrophe;
		case GLFW_KEY_BACKSLASH:		return Input::EKeyboard::Backslash;
		case GLFW_KEY_BACKSPACE:		return Input::EKeyboard::Backspace;
		case GLFW_KEY_LEFT_BRACKET:		return Input::EKeyboard::Bracket_L;
		case GLFW_KEY_RIGHT_BRACKET:	return Input::EKeyboard::Bracket_R;
		case GLFW_KEY_CAPS_LOCK:		return Input::EKeyboard::Caps_Lock;
		case GLFW_KEY_COMMA:			return Input::EKeyboard::Comma;
		case GLFW_KEY_DELETE:			return Input::EKeyboard::Delete;
		case GLFW_KEY_END:				return Input::EKeyboard::End;
		case GLFW_KEY_ENTER:			return Input::EKeyboard::Enter;
		case GLFW_KEY_EQUAL:			return Input::EKeyboard::Equal;
		case GLFW_KEY_ESCAPE:			return Input::EKeyboard::Escape;
		case GLFW_KEY_GRAVE_ACCENT:		return Input::EKeyboard::Grave;
		case GLFW_KEY_HOME:				return Input::EKeyboard::Home;
		case GLFW_KEY_INSERT:			return Input::EKeyboard::Insert;
		case GLFW_KEY_MINUS:			return Input::EKeyboard::Minus;
		case GLFW_KEY_NUM_LOCK:			return Input::EKeyboard::Numpad_Num_Lock;
		case GLFW_KEY_PAGE_DOWN:		return Input::EKeyboard::Page_Down;
		case GLFW_KEY_PAGE_UP:			return Input::EKeyboard::Page_Up;
		case GLFW_KEY_PAUSE:			return Input::EKeyboard::Pause;
		case GLFW_KEY_PERIOD:			return Input::EKeyboard::Period;
		case GLFW_KEY_PRINT_SCREEN:		return Input::EKeyboard::Print_Screen;
		case GLFW_KEY_SCROLL_LOCK:		return Input::EKeyboard::Scroll_Lock;
		case GLFW_KEY_SEMICOLON:		return Input::EKeyboard::Semicolon;
		case GLFW_KEY_SLASH:			return Input::EKeyboard::Slash;
		case GLFW_KEY_SPACE:			return Input::EKeyboard::Space;
		case GLFW_KEY_TAB:				return Input::EKeyboard::Tab;
		}
		return Input::EKeyboard::Unknown;
	}

	Input::EMouse ToMouse(int32_t key)
	{
		switch (key)
		{
		case GLFW_MOUSE_BUTTON_1: return Input::EMouse::Button_1;
		case GLFW_MOUSE_BUTTON_2: return Input::EMouse::Button_2;
		case GLFW_MOUSE_BUTTON_3: return Input::EMouse::Button_3;

		case GLFW_MOUSE_BUTTON_4: return Input::EMouse::Button_4;
		case GLFW_MOUSE_BUTTON_5: return Input::EMouse::Button_5;
		case GLFW_MOUSE_BUTTON_6: return Input::EMouse::Button_6;
		case GLFW_MOUSE_BUTTON_7: return Input::EMouse::Button_7;
		case GLFW_MOUSE_BUTTON_8: return Input::EMouse::Button_8;
		}
		return Input::EMouse::Unknown;
	}
}

namespace Saz
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		SAZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowBase* WindowBase::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) : WindowBase(props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			s_GLFWInitialized = true;
			SAZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SAZ_CORE_ASSERT(status, "Failed to initialize Glad!");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::Update(const GameTime& gameTime)
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool isEnabled)
	{
		if (isEnabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = isEnabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetResizable(bool resizable) const
	{

	}

	const std::string& WindowsWindow::GetTitle() const
	{
		return m_Data.Title;
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{

	}

	unsigned int WindowsWindow::GetWidth() const
	{
		return m_Data.Width;
	}

	unsigned int WindowsWindow::GetHeight() const
	{
		return m_Data.Height;
	}

	bool WindowsWindow::ShouldClose() const
	{
		return true;
	}

	void WindowsWindow::GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const
	{

	}

	void WindowsWindow::GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const
	{

	}
}

