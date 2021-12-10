#pragma once

#include <Saz/NameComponent.h>
#include <Saz/CameraComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/LevelComponent.h>
#include <Saz/MovementComponent.h>
#include <Saz/RenderComponent.h>
#include <Saz/TransformComponent.h>

namespace ecs
{
	using ComponentsList = TypeList<
		component::NameComponent,
		component::CameraComponent,
		component::InputComponent,
		component::LevelComponent,
		component::MovementComponent,
		component::RenderComponent,
		component::TransformComponent>;

	constexpr ComponentsList s_ComponentList;
}