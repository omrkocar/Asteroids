#pragma once

template<typename... Component>
struct ComponentGroup
{
};

namespace component 
{
	struct Rigidbody2DComponent; 
	struct TransformComponent;
	struct SpriteComponent;
	struct CircleRendererComponent;
	struct BoxCollider2DComponent;
	struct CameraComponent;

}
using AllComponents =
ComponentGroup<
	component::TransformComponent, 
	component::SpriteComponent, 
	component::CircleRendererComponent,
	component::CameraComponent, 
	component::Rigidbody2DComponent, 
	component::BoxCollider2DComponent>;