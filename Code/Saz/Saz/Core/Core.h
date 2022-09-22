#pragma once

#include <memory>

#ifndef SAZ_PLATFORM_WINDOWS
#error Saz only supports Windows!
#endif

#ifdef SAZ_DEBUG
#if defined(SAZ_PLATFORM_WINDOWS)
#define SAZ_DEBUGBREAK() __debugbreak()
#elif defined(SAZ_PLATFORM_LINUX)
#include <signal.h>
#define SAZ_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define SAZ_ENABLE_ASSERTS
#else
#define SAZ_DEBUGBREAK()
#endif

#define SAZ_EXPAND_MACRO(x) x
#define SAZ_STRINGIFY_MACRO(x) #x

#ifdef SAZ_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define SAZ_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { SAZ##type##ERROR(msg, __VA_ARGS__); SAZ_DEBUGBREAK(); } }
#define SAZ_INTERNAL_ASSERT_WITH_MSG(type, check, ...) SAZ_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define SAZ_INTERNAL_ASSERT_NO_MSG(type, check) SAZ_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", SAZ_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define SAZ_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define SAZ_INTERNAL_ASSERT_GET_MACRO(...) SAZ_EXPAND_MACRO( SAZ_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SAZ_INTERNAL_ASSERT_WITH_MSG, SAZ_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define SAZ_ASSERT(...) SAZ_EXPAND_MACRO( SAZ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define SAZ_CORE_ASSERT(...) SAZ_EXPAND_MACRO( SAZ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define SAZ_ASSERT(...)
#define SAZ_CORE_ASSERT(...)
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