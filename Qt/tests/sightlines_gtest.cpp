#include <gtest/gtest.h>
#include "sightlines.h"

TEST(sightlines, elevation_too_low) {
  EXPECT_TRUE(get_sightline(0.0, 100.0) < 0);
}

TEST(sightlines, elevation_too_high) {
  EXPECT_TRUE(get_sightline(1.6, 100.0) < 0);
}

TEST(sightlines, altitude_too_low) {
  EXPECT_TRUE(get_sightline(1.0, 0.0) < 0);
}

TEST(sightlines, altitude_too_high) {
  EXPECT_TRUE(get_sightline(1.0, 100000.0) < 0);
}