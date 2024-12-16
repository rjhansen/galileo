set(LIBSIGHTLINES sightlines.c)
add_library(sightlines SHARED)
target_sources(sightlines
    PUBLIC ${LIBSIGHTLINES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS . FILES
    ./sightlines.h
)
