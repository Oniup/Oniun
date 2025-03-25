include(../CMake/Compiler.cmake)

file(GLOB_RECURSE HDR_FILES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.h")
file(GLOB_RECURSE SRC_FILES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.cpp")

add_executable(
    OniunEditor
    ${HDR_FILES}
    ${SRC_FILES}
)

target_include_directories(
    OniunEditor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${Oniun_INCLUDE_DIRS}
)
target_link_libraries(
    OniunEditor PUBLIC
    Oniun
)

if (WIN32)
    set_target_properties(
        OniunEditor PROPERTIES
        WIN32_EXECUTABLE "$<CONFIG:Release>"
    )

    if (MSVC)
        set_target_properties(
            OniunEditor PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY "${BINARY_OUTPUT_DIRECTORY}/"
        )
    endif()
endif()
