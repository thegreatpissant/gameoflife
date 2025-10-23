// stats.cpp
#include "Stats.hpp"
#include <iostream>

namespace life {

void Stats::start(const std::string_view& name, const Uint64 time) {
    counters[name].start = time;
}

void Stats::stop(const std::string_view& name, const Uint64 time) {
    counters[name].stop = time;
}

void Stats::set(const std::string_view& name, const Uint64 value) {
	values[name] = value;
}
// A basic print implementation for testing purposes.
// This version directly calculates stop - start.
// If start > stop (e.g., stop not called, so stop=0), Uint64 will underflow.
void Stats::print(std::ostream& out) const {
    out << "Statistics:" << std::endl;
    if (counters.empty()) {
        // Optionally, print a message if no stats, or just the header.
        // For these tests, we'll assume it just prints the header if empty.
        return;
    }
    for (const auto& pair : counters) {
        const std::string_view& name = pair.first;
        const stat& s = pair.second;
        out << name << ": " << (s.stop - s.start) << " ms" << std::endl;
    }
    for (const auto& pair : values) {
        const std::string_view& name = pair.first;
        const Uint64& v = pair.second;
        out << name << ": " << v << std::endl;
    }
}

} // namespace life
