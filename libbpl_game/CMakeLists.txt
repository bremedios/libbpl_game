cmake_minimum_required(VERSION 3.28)

project(bpl_game)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL2_TTF)
find_package(SDL2_IMAGE REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)

include_directories(include "../../libbpl_graphics/libbpl_graphics/include" "../../libbpl_controls/libbpl_controls/include" ../../libbpl_sys/libbpl_sys/include)

set(PUBLIC_HEADER include/bpl/game/GameStateMachine.h
        include/bpl/game/PlatformerToken.h
        include/bpl/game/PlatformerEnemyToken.h
        include/bpl/game/GameWindow.h
        include/bpl/game/Timer.h
        include/bpl/game/Score.h)
set(SOURCE_FILES src/GameStateMachine.cpp
        src/PlatformerToken.cpp
        src/PlatformerEnemyToken.cpp
        src/GameWindow.cpp
        src/Timer.cpp
        src/Score.cpp)

add_library(bpl_game SHARED ${SOURCE_FILES} ${PUBLIC_HEADER} )

target_link_libraries(bpl_game PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf SDL2_image::SDL2_image bpl_graphics bpl_controls)

install(TARGETS bpl_game DESTINATION usr/lib)
install(FILES include/bpl/game/GameStateMachine.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/PlatformerToken.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/PlatformerEnemyToken.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/GameWindow.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/GameState.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/Timer.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/Score.h DESTINATION usr/local/include/bpl/game)
install(FILES include/bpl/game/Timer.h DESTINATION usr/local/include/bpl/game)