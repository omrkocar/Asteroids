#pragma once

#include <Saz/Core/Core.h>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Saz {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

//Core log macros
#define SAZ_CORE_TRACE(...)		::Saz::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SAZ_CORE_INFO(...)		::Saz::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SAZ_CORE_WARN(...)		::Saz::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SAZ_CORE_ERROR(...)		::Saz::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SAZ_CORE_CRITICAL(...)	::Saz::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define SAZ_TRACE(...)		::Saz::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SAZ_INFO(...)		::Saz::Log::GetClientLogger()->info(__VA_ARGS__)
#define SAZ_WARN(...)		::Saz::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SAZ_ERROR(...)		::Saz::Log::GetClientLogger()->error(__VA_ARGS__)
#define SAZ_CRITICAL(...)	::Saz::Log::GetClientLogger()->critical(__VA_ARGS__)
