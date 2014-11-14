
project(${MEDUZZZA_ENG_LIB})

find_package(Qt5Core REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB SOURCE_LIB src/*.cpp)
file(GLOB HEADER_LIB src/*.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE_LIB})

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${CLAMAV_CFLAGS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${CLAMAV_LIBS})
target_link_libraries(${PROJECT_NAME} clamav)

qt5_use_modules(${PROJECT_NAME} Core)