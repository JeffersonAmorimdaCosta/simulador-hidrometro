# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(SimuladorHidrometro CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

file(GLOB SOURCES src/*.cpp)
add_executable(SimuladorHidrometro ${SOURCES})

target_link_libraries(SimuladorHidrometro 
    PRIVATE 
        ${OpenCV_LIBS}   
)
target_include_directories(SimuladorHidrometro PRIVATE include)

if (WIN32)
    set(EXE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/SimuladorHidrometro.exe")
else()
    set(EXE_PATH "${CMAKE_CURRENT_BINARY_DIR}/simulador")
endif()

add_custom_target(
    run
    DEPENDS SimuladorHidrometro
    COMMAND ${EXE_PATH}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    COMMENT "Compilando e executando o simulador..."
)