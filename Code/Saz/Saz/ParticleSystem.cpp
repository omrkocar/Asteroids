
#include "SazPCH.h"
#include "ParticleSystem.h"

#include "Core/Random.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include "Saz/Rendering/Renderer2D.h"

namespace Saz
{
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Range(0.0f, 360.0f) * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		// Size
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		// Life
		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

	void ParticleSystem::OnUpdate(const Saz::GameTime& gameTime)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining < 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= gameTime.GetDeltaTime();
			particle.Position += particle.Velocity * (float)gameTime.GetDeltaTime();
			particle.Rotation += 0.01f * gameTime.GetDeltaTime();
		}
	}

	void ParticleSystem::Stop()
	{
		for (auto& particle : m_ParticlePool)
		{
			particle.Active = false;
		}

		Saz::Renderer2D::EndScene();
	}

	void ParticleSystem::OnRender(Saz::OrthographicCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
			glm::vec3 pos = { particle.Position.x, particle.Position.y, 0.2f };
			Saz::Renderer2D::DrawRotatedQuad(pos, { size, size }, particle.Rotation, color);
		}

		Renderer2D::EndScene();
	}
}
