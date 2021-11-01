#pragma once

#include <chrono>
#include <iostream>

namespace config
{

constexpr auto SERVER_ADDRESS{"tcp://localhost:5555"};
constexpr auto TOPIC{"data"};
constexpr int QOS = 2;
constexpr auto WAIT_FOR = std::chrono::milliseconds(250);
constexpr auto MAX_TAG_AMOUNT = 10;        // max 10 tags
constexpr auto MAX_MEASUREMENT_UNIT = 100; // max 100 measurement units

}; // namespace config
