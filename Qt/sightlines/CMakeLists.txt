set(LIBSIGHTLINES sightlines.c)
add_library(sightlines)
target_sources(sightlines
    PUBLIC ${LIBSIGHTLINES}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS . FILES
    ./sightlines.h
)
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()