include(../CMake/Compiler.cmake)

file(GLOB_RECURSE HDR_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR} "Source/*.cpp")

add_library(Oniun STATIC
    ${HDR_FILES}
    ${SRC_FILES}
)
target_include_directories(Oniun
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source

    # Thirdparty librarys
    PUBLIC
    ${ImGui_INCLUDE_DIRS}
    ${stb_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS}
    PRIVATE
    ${glfw_INCLUDE_DIRS}
    ${glad_INCLUDE_DIRS}
)
target_link_libraries(Oniun
    PUBLIC
    glm
    glad
    glfw
    ImGui
    stb
)
target_compile_definitions(Oniun
    PUBLIC
    ${REQUIRED_PREPROCESSOR_DEFINITIONS}
    ONIUN_RESOURCE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}"
    PRIVATE
	GLFW_INCLUDE_NONE
	GLFW_SCALE_TO_MONITOR
)
target_compile_options(Oniun
    PUBLIC
    ${PLATFORM_SPECIFIC_COMPILER_OPTIONS}
)
target_precompile_headers(Oniun
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Oniun.pch.h
)

