//
// Created by jim on 5/25/25.
//
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

class Stats {
private:
        struct stat {
                Uint64 start = 0;
                Uint64 stop = 0;
        };

        std::map<std::string_view, stat> counters;
public:
        void start(const std::string_view& name, Uint64 time);
        void stop(const std::string_view& name, Uint64 time);
        void print(std::ostream& out = std::cout) const;
};

}

#endif //STATS_H
