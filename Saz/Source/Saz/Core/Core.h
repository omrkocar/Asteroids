#pragma once

#include <memory>

#ifndef SAZ_PLATFORM_WINDOWS
#error Saz only supports Windows!
#endif

#ifdef SAZ_ENABLE_ASSERTS
#define SAZ_ASSERT(x, ...) { if(!(x)) { SAZ_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define SAZ_CORE_ASSERT(x, ...) { if(!(x)) { SAZ_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define SAZ_ASSERT(x, ...)
#define SAZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SAZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Saz
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}