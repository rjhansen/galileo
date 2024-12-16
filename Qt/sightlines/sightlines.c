#include "sightlines.h"
#include <math.h>
#include <stdio.h>

static double EARTH_RADIUS = 6371.0;

double get_sightline(const double ELEVATION, const double ALTITUDE, 
    char* new_message, const uint32_t NEW_MESSAGE_LEN, const char* DISTANCE_UNITS)
{
    const double ARCSIN_TERM = asin(cos(ELEVATION) * (EARTH_RADIUS / (EARTH_RADIUS + ALTITUDE)));
    const double COS_TERM = cos(ELEVATION + ARCSIN_TERM);
    const double NUMERATOR = (EARTH_RADIUS + ALTITUDE) * COS_TERM;
    const double DENOMINATOR = cos(ELEVATION);
    const double SIGHTLINE = (NUMERATOR / DENOMINATOR) * ALTITUDE;
#if _WIN32 || _WIN64
    _snprintf_s(new_message, new_message_len, _TRUNCATE,
                "Sightline is %.2f %s", SIGHTLINE, DISTANCE_UNITS);
#else
    snprintf(new_message, NEW_MESSAGE_LEN,
             "Sightline is %.2f %s", SIGHTLINE, DISTANCE_UNITS);
#endif
    return SIGHTLINE;
}
