#include "sightlines.h"
#include <math.h>

static double EARTH_RADIUS = 6371.0;

/* takes elevation in rads and altitude in kms, returns -1.0
   on invalid inputs. Elevation must be between 0.01 and 
   pi/2 - 0.01*, altitude between 100 and 50000. */
double get_sightline(const double ELEVATION, const double ALTITUDE)
{
    if (ELEVATION < 0.01 || ELEVATION > 1.5607963267948965 ||
        ALTITUDE < 100.0 || ALTITUDE > 50000.0)
        return -1.0;
    
    const double NUMERATOR = (EARTH_RADIUS + ALTITUDE) * 
        cos(ELEVATION + 
        asin(cos(ELEVATION) * (EARTH_RADIUS / 
        (EARTH_RADIUS + ALTITUDE))));
    const double DENOMINATOR = cos(ELEVATION);
    const double SIGHTLINE = (NUMERATOR / DENOMINATOR);
    return SIGHTLINE;
}
