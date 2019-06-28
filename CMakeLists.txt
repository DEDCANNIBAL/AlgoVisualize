cmake_minimum_required(VERSION 3.13)
project(AlgoVisualize)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib/SFML/include)
link_directories(lib/SFML/lib)

include_directories(lib/imgui)
include_directories(lib/imgui-sfml)

add_executable(AlgoVisualize main.cpp lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp
        lib/imgui-sfml/imgui-SFML.cpp lib/imgui/imgui_widgets.cpp Field.cpp Field.h FieldDrawer.cpp FieldDrawer.h PathFinder.cpp PathFinder.h FieldInterface.cpp FieldInterface.h BreadthFirstSearch.cpp BreadthFirstSearch.h PathFinderManager.cpp PathFinderManager.h SafeVector.cpp SafeVector.h)
target_link_libraries(AlgoVisualize sfml-graphics sfml-window sfml-system GL pthread)