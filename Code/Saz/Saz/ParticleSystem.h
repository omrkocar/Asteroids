#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "Rendering/OrthographicCamera.h"

namespace Saz 
{ 
	class Shader; 
	class OrthographicCamera;
	class GameTime;
}

// Temporary
namespace Saz
{
	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};


	class ParticleSystem
	{
	public:
		ParticleSystem();

		void OnUpdate(const Saz::GameTime& gameTime);
		void Stop();
		void OnRender(Saz::OrthographicCamera& camera);

		void Emit(const ParticleProps& particleProps);
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;
	};
}
