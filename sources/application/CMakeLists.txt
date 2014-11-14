
project(${MEDUZZZA_APP_BIN})

find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB SOURCE_LIB src/*.cpp EXCLUDE main.cpp)
file(GLOB SOURCE_LIB src/*.cpp)
file(GLOB SOURCE_BIN src/main.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${MEDUZZZA_ENG_SRC_DIR})

add_library(${MEDUZZZA_APP_LIB} STATIC ${SOURCE_LIB})
add_executable(${PROJECT_NAME} ${SOURCE_BIN})
target_link_libraries(${MEDUZZZA_APP_BIN} ${MEDUZZZA_APP_LIB} ${MEDUZZZA_ENG_LIB})
qt5_use_modules(${MEDUZZZA_APP_LIB} Core Sql Network)
qt5_use_modules(${PROJECT_NAME} Core Sql Network)
