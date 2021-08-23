#pragma once

//#define METRICS

#ifndef METRICS
#if defined(DEBUG) || defined(_DEBUG)
#define METRICS
#endif
#endif

#ifdef METRICS

#include <chrono>
#include <thread>
#include <variant>
#include <fstream>

#include "AtomicStack.h"

#if defined(__cpp_lib_source_location)

#include <source_location>

#define METRICS_SOURCE_LOCATION_T std::source_location
#define METRICS_SOURCE_LOCATION std::source_location::current()

#else

namespace metrics
{

	struct source_location
	{
		uint_least32_t _line;
		const char* _file_name;
		const char* _function_name;

		constexpr uint_least32_t line() const noexcept { return _line; }
		constexpr const char* file_name() const noexcept { return _file_name; }
		constexpr const char* function_name() const noexcept { return _function_name; }
	};

	constexpr inline static source_location SourceLocation(
		uint_least32_t line,
		const char* file_name,
		const char* function_name
	) noexcept
	{
		source_location src{};
		src._line = line;
		src._file_name = file_name;
		src._function_name = function_name;
		return src;
	}

#define METRICS_SOURCE_LOCATION_T metrics::source_location
#define METRICS_SOURCE_LOCATION metrics::SourceLocation(__LINE__, __FILE__, __func__)

}

#endif

namespace metrics
{

	using Clock = std::chrono::steady_clock;

	template <typename = void>
	class MetricLog
	{

		class BlockTimer;

	public:

		MetricLog(std::ostream* stream);
		~MetricLog();
		
		static void Register(METRICS_SOURCE_LOCATION_T src);
		static BlockTimer TimeBlock (METRICS_SOURCE_LOCATION_T src);

	private:

		class BlockTimer final
		{
		public:
			~BlockTimer();
		};

		struct Event
		{
			Clock::time_point time;
			std::thread::id id;
			METRICS_SOURCE_LOCATION_T src;
		};
		struct Begin : Event
		{};
		struct End : Event
		{};
		using Data = std::variant<Event, Begin, End>;

		static void Print(Event const& data);
		static void Print(Begin const& data);
		static void Print(End   const& data);

		static std::unique_ptr<std::ostream> m_Stream;
		static util::AtomicStack<Data> m_Log;

	};

#define METRICS_REGISTER     metrics::MetricLog<>::Register(METRICS_SOURCE_LOCATION);
#define METRICS_TIMEBLOCK    auto _blocktimer = metrics::MetricLog<>::TimeBlock(METRICS_SOURCE_LOCATION);
#define METRICS_INITLOG(...) metrics::MetricLog<> _metriclog{ new std::ofstream{ __VA_ARGS__ } };

}

#else

#define METRICS_SOURCE_LOCATION_T
#define METRICS_SOURCE_LOCATION
#define METRICS_REGISTER    
#define METRICS_TIMEBLOCK   
#define METRICS_INITLOG(...)

#endif