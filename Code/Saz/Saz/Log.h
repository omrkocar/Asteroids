#pragma once

#include <Saz/Core.h>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Saz {

	class SAZ_API Log
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
