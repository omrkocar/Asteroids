#include <Saz/SazPCH.h>
#include <Catch2/catch.hpp>

#include <Saz/EntityWorld.h>
#include <Saz/System.h>
#include <Saz/InputComponent.h>
#include <Saz/InputSystem.h>
#include <Saz/GLFW/Window.h>


TEST_CASE("Input System")
{
	ecs::EntityWorld world;

	world.RegisterComponent<component::InputComponent>();

	Saz::WindowProps props;
	Saz::glfw::Window window(props);
	world.RegisterSystem<ecs::InputSystem>(window);

	auto entity = world.CreateEntity();
	world.AddComponent<component::InputComponent>(entity);
}
