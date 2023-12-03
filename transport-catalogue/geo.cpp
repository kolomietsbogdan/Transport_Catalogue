#include <cmath>

#include "geo.h"

namespace geo {
    static const double EARTH_RADIUS = 6371000.;
    static const double PI = 3.1415926535;
    double ComputeDistance(Coordinates from, Coordinates to) {
        using namespace std;
        if (from == to) {
            return 0;
        }
        static const double dr = PI / 180.;
        return acos(sin(from.lat * dr) * sin(to.lat * dr) + cos(from.lat * dr) * cos(to.lat * dr) * cos(abs(from.lng - to.lng) * dr)) * EARTH_RADIUS;
    }
}
