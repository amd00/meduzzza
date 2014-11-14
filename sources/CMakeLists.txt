
set(MEDUZZZA_ENG_LIB meduzzza-engine)
set(MEDUZZZA_APP_LIB meduzzza-libS)
set(MEDUZZZA_APP_BIN meduzzza-application)
set(MEDUZZZA_GUI_BIN meduzzza-gui)

set(MEDUZZZA_ENG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/clamavqtengine/src)
set(MEDUZZZA_APP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/application/src)

add_definitions(-Wall -fPIC -O2 -std=c++11)

add_subdirectory(clamavqtengine)
add_subdirectory(application)
add_subdirectory(gui)