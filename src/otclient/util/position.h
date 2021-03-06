/*
 * Copyright (c) 2010-2011 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef POSITION_H
#define POSITION_H

#include <otclient/const.h>
#include <framework/util/types.h>
#include <framework/util/point.h>

class Position
{
public:
    Position() : x(-1), y(-1), z(-1) { }
    Position(int x, int y, int z) : x(x), y(y), z(z) { }

    static Position getPositionFromDirection(Otc::Direction direction) {
        switch(direction) {
        case Otc::North:
            return Position( 0, -1, 0);
        case Otc::East:
            return Position( 1,  0, 0);
        case Otc::South:
            return Position( 0,  1, 0);
        case Otc::West:
            return Position(-1,  0, 0);
        case Otc::NorthEast:
            return Position( 1, -1, 0);
        case Otc::SouthEast:
            return Position( 1,  1, 0);
        case Otc::SouthWest:
            return Position(-1,  1, 0);
        case Otc::NorthWest:
            return Position(-1, -1, 0);
        default:
            return Position();
        }
    }

    bool isValid() const { return x >= 0 && y >= 0 && z >= 0 && x < 65536 && y < 65536 && z < 15; }

    Position operator+(const Position& other) const { return Position(x + other.x, y + other.y, z + other.z);   }
    Position& operator+=(const Position& other) { x+=other.x; y+=other.y; z +=other.z; return *this; }
    Position operator-(const Position& other) const { return Position(x - other.x, y - other.y, z - other.z);   }
    Position& operator-=(const Position& other) { x-=other.x; y-=other.y; z-=other.z; return *this; }

    Position& operator=(const Position& other) { x = other.x; y = other.y; z = other.z; return *this; }
    bool operator==(const Position& other) const { return other.x == x && other.y == y && other.z == z; }
    bool operator!=(const Position& other) const { return other.x!=x || other.y!=y || other.z!=z; }

    bool isInRange(const Position& pos, int xdif, int ydif, int zdif = 1) {
        return std::abs(x-pos.x) <= xdif && std::abs(y-pos.y) <= ydif && std::abs(pos.z-z) <= zdif;
    }

    void up(int n = 1) { z-=n; }
    void down(int n = 1) { z+=n; }

    void perspectiveUp(int n = 1) { x+=n; y+=n; z-=n; }
    void coveredDown(int n = 1) { x-=n; y-=n; z+=n; }

    int x;
    int y;
    int z;
};

struct PositionHasher : std::unary_function<Position, std::size_t> {
    std::size_t operator()(const Position& pos) const {
        return ((((pos.x * 65536) + pos.y) * 15) + pos.z) % 1000000;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Position& pos)
{
    out << pos.x << " " << pos.y << " " << pos.z;
    return out;
}

inline std::istream& operator>>(std::istream& in, Position& pos)
{
    in >> pos.x >> pos.y >> pos.z;
    return in;
}

#endif
