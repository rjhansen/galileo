set(LIBSIGHTLINES sightlines.c)
add_library(sightlines STATIC)
target_sources(sightlines
    PUBLIC ${LIBSIGHTLINES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS . FILES
    ./sightlines.h
)
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()