#pragma once

#include <Saz/WindowBase.h>

namespace raylib
{
	class Window;
}

namespace Input
{
	enum class EKeyboard;
	enum class EMouse;
}

namespace Saz
{
	class SAZ_API Window final : public Saz::WindowBase
	{
	public:
		explicit Window(const Saz::WindowProps& props);
		~Window() override;

		void Init();
		void Update(const Saz::GameTime& gameTime);

		void BeginDrawing();
		void EndDrawing();

		ivec2 GetSize() const override { return m_Size; }
		void SetSize(ivec2 newSize) override { m_Size = newSize; }

		bool ShouldClose() const override;

		void GatherKeyboard(Set<Input::EKeyboard>& out_Keys) const;
		void GatherMouse(Set<Input::EMouse>& out_Keys, vec2& out_Delta, vec2& out_Position) const;

		bool HasResized() const { return m_HasResized; }
		void SetResized(bool value) { m_HasResized = value; }

	private:
		String m_Title = { };
		ivec2 m_Size = { };

		std::unique_ptr<raylib::Window> m_Window;

		vec2 m_MousePos = vec2(0.0f);
		vec2 m_MouseDelta = vec2(0.0f);

		bool m_HasResized = false;
	};
}