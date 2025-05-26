// stats.cpp
#include "stats.hpp"
#include <iostream>

namespace life {

void stats::start(const std::string_view& name, Uint64 time) {
    counters[name].start = time;
}

void stats::stop(const std::string_view& name, Uint64 time) {
    counters[name].stop = time;
}

// A basic print implementation for testing purposes.
// This version directly calculates stop - start.
// If start > stop (e.g., stop not called, so stop=0), Uint64 will underflow.
void stats::print(std::ostream& out) {
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
}

} // namespace life