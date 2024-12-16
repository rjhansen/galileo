cmake_minimum_required(VERSION 3.30.0 FATAL_ERROR)

# Before we begin defining our project, we start by defining globals CMake
# will use throughout.  Here, we say that all our C++ code will follow the
# C++23 standard without any vendor-specific extensions (e.g., no GNUisms),
# the C11 standard without any vendor-specific extensions (etc.), and the
# homepage for the project.
#
# For reasons unknown to me, it's best to set language-specific variables
# before the project is declared.

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_EXTENSIONS Off)
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/rjhansen/Galileo")

# Now the project itself is declared.  It gets given a name, a version, a
# description, we specify what languages we use, and what homepage to use
# for it.

project(Galileo 
    VERSION 1.0.0 
    DESCRIPTION "A sightline calculator" 
    LANGUAGES CXX C 
    HOMEPAGE_URL CMAKE_PROJECT_HOMEPAGE_URL)

# Galileo depends on Qt6.  Specifically, the Core and Widgets components.
# Only really large projects get broken up into components.  Details on
# which components exist, etc., can be found in a web search for
# "cmake [framework] find_package example".
#
# Qt uses tools called moc, uic, and rcc, in the compilation process.
# CMake can run these tools automatically as appropriate.  Why this isn't
# the default is a mystery to me.

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Galileo doesn't depend on GTest or Boost, excepting that we will need
# them to write unit tests involving either unit test framework.  You WILL
# be using one or the other in production code.  If your dev box doesn't
# have both available, that's an environment defect: fix it.

find_package(Boost 1.80 REQUIRED COMPONENTS unit_test_framework REQUIRED)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(gui)
add_subdirectory(sightlines)

target_include_directories(${PROJECT_NAME} PUBLIC sightlines)
target_link_libraries(${PROJECT_NAME} PUBLIC sightlines Qt6::Widgets)

install(TARGETS sightlines FILE_SET HEADERS)

# Our testing regimen begins here. 
enable_testing()
add_executable(sightlines_gtest tests/sightlines_gtest.cpp)
target_link_libraries(sightlines_gtest sightlines GTest::gtest_main)
add_test(NAME sightlines_gtest COMMAND sightlines_gtest)

add_executable(sightlines_btest tests/sightlines_btest.cpp)
target_include_directories(sightlines_btest PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(sightlines_btest sightlines ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME sightlines_btest COMMAND sightlines_btest)

