#include "log/Profiler.h"

#include <unordered_map>

#include "log/log.h"

struct ProfilerFuncData {
	long min = std::numeric_limits<long>::max();
	long max = std::numeric_limits<long>::min();
};

static std::unordered_map<const char *, ProfilerFuncData> g_profilerData;

void ProfilerLog(const char * name, long us) {
	
	auto & func = g_profilerData[name];
	func.min = std::min(func.min, us);
	func.max = std::max(func.max, us);
}

void ProfilerPrint() {
	
	for(const auto & [key, val]: g_profilerData) {
		log_info("Func {}: min: {}μs max: {}μs", key, val.min, val.max);
	}
}

void ProfilerClear() {
	g_profilerData.clear();
}