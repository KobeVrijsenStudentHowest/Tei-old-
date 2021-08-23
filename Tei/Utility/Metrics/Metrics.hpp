#pragma once

#include "Metrics.h"

#ifdef METRICS

namespace metrics
{

	template <typename T>
	util::AtomicStack<typename MetricLog<T>::Data> MetricLog<T>::m_Log{};

	template <typename T>
	std::unique_ptr<std::ostream> MetricLog<T>::m_Stream{};

	template <typename T>
	inline MetricLog<T>::~MetricLog()
	{
		*m_Stream << '[';
		Print(Begin{ Clock::now(), std::this_thread::get_id(), METRICS_SOURCE_LOCATION });

		for (Data const& data : m_Log)
			std::visit([](auto const& data) { Print(data); }, data);

		Print(End{ Clock::now(), std::this_thread::get_id() });
		*m_Stream << "{}]";
		m_Stream.reset();
	}

	template <typename T>
	inline MetricLog<T>::BlockTimer::~BlockTimer()
	{
		m_Log.push(End{ Clock::now(), std::this_thread::get_id() });
	}

	template <typename T>
	inline void MetricLog<T>::Register(METRICS_SOURCE_LOCATION_T src)
	{
		m_Log.push(Event{ Clock::now(), std::this_thread::get_id(), src });
	}

	template <typename T>
	MetricLog<T>::BlockTimer MetricLog<T>::TimeBlock(METRICS_SOURCE_LOCATION_T src)
	{
		m_Log.push(Begin{ Clock::now(), std::this_thread::get_id(), src });
		return {};
	}

	template <typename T>
	inline MetricLog<T>::MetricLog(std::ostream* stream)
	{
		m_Stream = std::unique_ptr<std::ostream>{ stream };
	}

	template <typename T>
	inline void MetricLog<T>::Print(Event const& data)
	{
		char buffer[4096]{};
		int pos{};
		auto time = data.time.time_since_epoch().count() / 1000;
		auto filename = strrchr(data.src.file_name(), '\\') + 1;
		pos += sprintf_s(
			buffer,
R"({ 
	"name":"%s - %s",
	"ph":"X",
	"ts":%lld,
	"dur":1,
	"pid":0,
	"tid":%u,
	"args":
	{
		"file_name":"%s",
		"function_name":"%s",
		"line":%d
	}
},)",
			filename,
			data.src.function_name(),
			time,
			std::bit_cast<uint32_t>(data.id),
			filename,
			data.src.function_name(),
			data.src.line()
		);
		*m_Stream << std::string_view{ buffer, size_t(pos) };
	}

	template <typename T>
	inline void MetricLog<T>::Print(Begin const& data)
	{
		char buffer[4096]{};
		int pos{};
		auto time = data.time.time_since_epoch().count() / 1000;
		auto filename = strrchr(data.src.file_name(), '\\') + 1;
		pos += sprintf_s(
			buffer,
R"({
	"name":"%s - %s",
	"cat":"",
	"ph":"B",
	"ts":"%lld",
	"pid":0,
	"tid":%u,
	"args":
	{
		"file_name":"%s",
		"function_name":"%s",
		"line":%u
	}
},)",
			filename,
			data.src.function_name(),
			time,
			std::bit_cast<uint32_t>(data.id),
			filename,
			data.src.function_name(),
			data.src.line()
		);
		*m_Stream << std::string_view{ buffer, size_t(pos) };
	}

	template <typename T>
	inline void MetricLog<T>::Print(End const& data)
	{
		char buffer[4096]{};
		int pos{};
		auto now = data.time.time_since_epoch().count() / 1000;
		pos += sprintf_s(
			buffer,
R"({
	"ph":"E",
	"ts":"%lld",
	"pid":0,
	"tid":%u,
	"args":{}
},)",
			now,
			std::bit_cast<uint32_t>(data.id)
		);
		*m_Stream << std::string_view{ buffer, size_t(pos) };
	}

}

#endif