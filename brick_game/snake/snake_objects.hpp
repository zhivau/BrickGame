#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <chrono>
#include <fstream>
#include <vector>

#define HEIGHT 20
#define WIDTH 10

namespace GameConstans {
static constexpr int kMsIntervals[10] = {500, 480, 460, 440, 430,
                                         420, 415, 410, 405, 400};
constexpr int kDotSize = 30;
}  // namespace GameConstans

struct Coordinate {
  int x;
  int y;

  Coordinate() : x(0), y(0) {}
  Coordinate(int x, int y) : x(x), y(y) {}
  Coordinate(const Coordinate& other) : x(other.x), y(other.y) {}

  bool operator==(const Coordinate& other) const {
    return x == other.x && y == other.y;
  }

  Coordinate& operator=(const Coordinate& other) {
    x = other.x;
    y = other.y;
    return *this;
  }
};

enum class Direction { UP = 0, RIGHT, DOWN, LEFT };

#endif
