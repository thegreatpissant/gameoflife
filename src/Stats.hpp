#ifndef STATS_H
#define STATS_H

#include <map>
#include <string_view>
#include <iostream>
#include "SDL3/SDL_stdinc.h"

namespace life {

constexpr std::string_view ITERATE = "Iterate";
constexpr std::string_view RENDER = "Render";
constexpr std::string_view PRESENT = "Present";
constexpr std::string_view CALL = "Call";
constexpr std::string_view FRAMERATE = "Frame Rate";

class Stats {
private:
        struct stat {
                Uint64 start = 0;
                Uint64 stop = 0;
        };

        std::map<std::string_view, stat> counters;
        std::map<std::string_view, Uint64> values;
public:
        void start(const std::string_view& name, Uint64 time);
        void stop(const std::string_view& name, Uint64 time);
	void set(const std::string_view& name, Uint64 value);
        void print(std::ostream& out = std::cout) const;
};

}

#endif //STATS_H
