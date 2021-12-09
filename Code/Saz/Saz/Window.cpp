#include "SazPCH.h"
#include "Window.h"

#include "Saz/GameTime.h"
#include "Saz/Screen.h"

#include <Core/Input.h>

namespace Saz
{
	Window::Window(const Saz::WindowProps& props)
		: Saz::WindowBase(props)
	{
		m_Window = new raylib::Window(props.m_Size.x, props.m_Size.y, props.m_Title);
	}

	Window::~Window()
	{
		delete m_Window;
	}

	void Window::Init()
	{
		
	}

	void Window::Update(const Saz::GameTime& gameTime)
	{
		
	}

	void Window::BeginDrawing()
	{
		::BeginDrawing();
		m_Window->ClearBackground(::RAYWHITE);

		RayDrawText("Congrats! You created your first window!", 190, 200, 20, ::LIGHTGRAY);
	}

	void Window::EndDrawing()
	{
		::EndDrawing();
	}

	bool Window::ShouldClose() const
	{
		return false;
	}

	void Window::GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const
	{
		
	}

	void Window::GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const
	{
		
	}
}