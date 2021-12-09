#include "SazPCH.h"
#include "Window.h"

#include "Saz/GameTime.h"
#include "Saz/Screen.h"

#include <Core/Input.h>
#include <raylib/src/raylib.h>

namespace Saz
{
	Window::Window(const Saz::WindowProps& props)
		: Saz::WindowBase(props)
	{
		InitWindow(props.m_Size.x, props.m_Size.y, props.m_Title);
	}

	Window::~Window()
	{
		
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
		ClearBackground(::RAYWHITE);

		DrawPoly(Vector2{ 300, 300 }, 4, 500, 0.0, ::BLUE);
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