cmake_minimum_required(VERSION 3.13)
project(AlgoVisualize)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib/sfml/include)
link_directories(lib/sfml/lib)

include_directories(lib/imgui)
include_directories(lib/imgui-sfml)

add_executable(AlgoVisualize src/main.cpp lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp
        lib/imgui-sfml/imgui-SFML.cpp lib/imgui/imgui_widgets.cpp src/Field.cpp src/Field.h
        src/path_finders/AStar.cpp src/visual/FieldDrawer.cpp src/visual/FieldDrawer.h
        src/path_finders/PathFinder.cpp src/path_finders/PathFinder.h src/visual/FieldInterface.cpp
        src/visual/FieldInterface.h src/path_finders/BreadthFirstSearch.cpp
        src/path_finders/BreadthFirstSearch.h src/PathFinderManager.cpp
        src/PathFinderManager.h src/utils/SafeVector.cpp src/utils/SafeVector.h
        src/visual/Camera.cpp src/visual/Camera.h src/visual/UserInterface.cpp
        src/visual/UserInterface.h src/field_generators/MazeGenerator.cpp
        src/field_generators/MazeGenerator.h src/utils/utils.cpp src/utils/utils.h
        src/path_finders/FastSearch.cpp src/path_finders/FastSearch.h src/path_finders/TestSearch.h
        src/path_finders/TestSearch.cpp src/field_generators/RoomMazeGenerator.cpp
        src/field_generators/RoomMazeGenerator.h src/field_generators/EllerMazeGenerator.cpp
        src/field_generators/EllerMazeGenerator.h)
target_link_libraries(AlgoVisualize sfml-graphics sfml-window sfml-system GL pthread)