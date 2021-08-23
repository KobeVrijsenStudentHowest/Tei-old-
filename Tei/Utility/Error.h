#pragma once

#include <stdexcept>

#if defined(__cpp_lib_source_location)
#include <source_location>
#else
#define source_location byte
#endif

#if defined(__cpp_lib_format)
#include <format>
#endif

namespace tei::internal::utility
{

	struct TeiRuntimeError : std::runtime_error
	{
		inline TeiRuntimeError(std::string_view message, std::source_location loc = {}) :
			std::runtime_error{
#if defined(__cpp_lib_format)
				std::format(
#if defined(DEBUG) || defined(_DEBUG)
					// Debug
					"[Error] {}: {}, {}: {}",
					loc.file_name(),
					loc.line(),
					loc.function_name(),
					message
#else
					// Release
					"[Error] {}: {}",
					loc.function_name(),
					message
#endif
				)
#else
				std::string{ (loc, message) }
#endif
			}
		{}
	};

}

#if !defined(__cpp_lib_source_location)
#undef source_location
#endif