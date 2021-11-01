#pragma once

#include <fmt/core.h>
#include <iostream>

using TagId_t = size_t;

struct Point
{
    int x{0};
    int y{0};
    int z{0};
};

struct Data
{
    TagId_t tag_id{0};
    Point point{};

    std::string to_string() const
    {
        return fmt::format("Tag: {};\tPosition: {}.{}.{}", tag_id, point.x, point.y, point.z);
    }
};

std::ostream& operator<<(std::ostream& os, const Data& d)
{
    os << fmt::format("Tag: {}; Position: {}.{}.{}", d.tag_id, d.point.x, d.point.y, d.point.z);
    return os;
}
