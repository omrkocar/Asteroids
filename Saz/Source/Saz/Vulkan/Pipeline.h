#pragma once

#include <Saz/Vulkan/Device.h>
#include <Core/Vector.h>

#include <Vulkan/vulkan.h>
#include <Core/String.h>

namespace vulkan
{
	class Pipeline
	{
	public:
		Pipeline(Device& device);
		~Pipeline();

	private:
		void CreateGraphicsPipeline();

		static DynamicArray<char> ReadFile(const String& filename);
	private:
		Device& m_Device;
	};
}