cmake_minimum_required(VERSION 3.10)
project(SimuladorHidrometro)

file(GLOB SOURCES src/*.cpp)

add_executable(SimuladorHidrometro ${SOURCES})
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