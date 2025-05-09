/*   (c)2025 Vainstein K.   */
#include "common.h"
#include <chrono>


class Timer {
	using Clock_t = std::chrono::high_resolution_clock;
	using Timepoint_t = std::chrono::time_point<Clock_t>;

	char const *const _tag;
	Timepoint_t const _started;
public:
	void report (char const *const occasion = "", int const lineNo=0) {
		Timepoint_t::duration const ela{(Clock_t::now() - _started)};
		//
		using millisDuration_t = std::chrono::duration<double, std::chrono::milliseconds::period>;
		millisDuration_t const millisEla = millisDuration_t(ela);
		using microsDuration_t = std::chrono::duration<double, std::chrono::microseconds::period>;
		microsDuration_t const microsEla = microsDuration_t(ela);
		using nanosDuration_t = std::chrono::duration<long long, std::chrono::nanoseconds::period>;
		nanosDuration_t  const nanosEla =  nanosDuration_t(ela);
		//
#define prettyTimFmt "\e[33;1;3mt\e[0m/\e[33m%s\e[0m/"
#define prettyElaFmt "\n\t\t%12.6f ms = %9.3f us = %9lld ns."
		if (lineNo)
			printf(prettyTimFmt "@%3d,%-14s " prettyElaFmt "\n\n",
			       _tag, lineNo, occasion, millisEla.count(), microsEla.count(), nanosEla.count());
		else
			printf(prettyTimFmt " " prettyElaFmt "\n\n",
			       _tag,          millisEla.count(), microsEla.count(), nanosEla.count());
#undef prettyTimFmt
#undef prettyElaFmt
	}

	Timer() =delete;

	Timer (char const *tag) : _tag(tag), _started(Clock_t::now()) { }

	~Timer() { report(); }
};

#define tReport(preDtorOccasion)               report("\"" preDtorOccasion "\"",__LINE__)
