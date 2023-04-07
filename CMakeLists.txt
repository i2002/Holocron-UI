cmake_minimum_required(VERSION 3.15)

# NOTE: update executable name in .github/workflows/cmake.yml:44 when changing executable name in this file
# for now, the project name is used as the executable name
project(HolocronUI VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()


# By default, treat warnings as errors; override via cmake defines or via env vars
if(DEFINED WARNINGS_AS_ERRORS)
    # do nothing, already defined
elseif(DEFINED ENV{WARNINGS_AS_ERRORS})
    set(WARNINGS_AS_ERRORS $ENV{WARNINGS_AS_ERRORS})
else()
    set(WARNINGS_AS_ERRORS ON)
endif()

###############################################################################

# external dependencies with FetchContent
#
# NOTE: Also update env vars used for caching in .github/workflows/cmake.yml
#FetchContent_Declare(
#        SomeLib
#        GIT_REPOSITORY https://github.com/<SomeUser>/<SomeLib>.git
#        GIT_TAG        <some_git_hash> # <which tag/branch @ <some_date>>
#        GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
#)
#
#FetchContent_MakeAvailable(SomeLib)

# ---- SDL2 ----
include(FetchContent)
if (WIN32)
    FetchContent_Declare(SDL2
        URL     https://github.com/libsdl-org/SDL/releases/download/release-2.24.2/SDL2-devel-2.24.2-vc.zip
    )

    FetchContent_Declare(SDL2_image
        URL     https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.2/SDL2_image-devel-2.6.2-vc.zip
    )

    FetchContent_Declare(SDL2_mixer
        URL     https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.6.2/SDL2_mixer-devel-2.6.2-vc.zip
    )

    FetchContent_Declare(SDL2_ttf
        URL     https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.1/SDL2_ttf-devel-2.20.1-vc.zip
    )

    FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_mixer SDL2_ttf)
    list(APPEND CMAKE_PREFIX_PATH "${sdl2_SOURCE_DIR}/")
    list(APPEND CMAKE_PREFIX_PATH "${sdl2_image_SOURCE_DIR}/")
    list(APPEND CMAKE_PREFIX_PATH "${sdl2_mixer_SOURCE_DIR}/")
    list(APPEND CMAKE_PREFIX_PATH "${sdl2_ttf_SOURCE_DIR}/")
endif ()

# ---- Centurion ----
FetchContent_Declare(Centurion
    GIT_REPOSITORY https://github.com/albin-johansson/centurion
    GIT_TAG        v.7.2.0
)

FetchContent_GetProperties(Centurion)
if(NOT centurion_POPULATED)
    FetchContent_Populate(Centurion)
endif()

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml when changing name here
add_executable(${PROJECT_NAME} src/main.cpp src/Application.cpp src/Window.cpp src/Widget.cpp src/DemoWidget.cpp src/GridContainer.cpp src/GridContainer.h src/Container.cpp src/Container.h src/exceptions.h src/Utilities.cpp src/Utilities.h src/TextBox.h src/TextBox.cpp src/EventDispatcher.h src/BoxContainer.cpp src/BoxContainer.h src/Button.cpp src/Button.h src/TextInput.cpp src/TextInput.h)

###############################################################################

# target definitions

if(GITHUB_ACTIONS)
    message("NOTE: GITHUB_ACTIONS defined")
    target_compile_definitions(${PROJECT_NAME} PRIVATE GITHUB_ACTIONS)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE HOLOCRONUI_DEBUG_MESSAGES)

###############################################################################

# custom compiler flags
message("Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /wd4244 /wd4267 /external:anglebrackets /external:W0)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
        target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
    endif()
endif()

###############################################################################

# sanitizers
# disable them when releasing executables without explicitly requested debug info
# use generator expressions to set flags correctly in both single and multi config generators
set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:/fsanitize=address>")
elseif(APPLE)
    # if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # first check Apple since Apple is also a kind of Unix
    target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
    # endif()
elseif(UNIX)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        # we cannot mix sanitizers with Valgrind
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        if("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "14.")
            # check leaks on Linux since macOS does not support the leaks sanitizer yet
            # leaks sanitizer is enabled by default on Linux, so we do not need to enable it explicitly
            target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            target_link_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
        elseif("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "13.")
            # use semi-colons instead of spaces to separate arguments
            # it is recommended to quote generator expressions in order to avoid unintentional splitting
            target_compile_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins>")
            target_link_options(${PROJECT_NAME} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins;-Wl,-rpath,tools/llvm-project/build/lib>")
        else()
            message("No matching Clang version to add sanitizer flags!")
        endif()
    endif()
endif()

###############################################################################

# use SYSTEM so clang-tidy does not report warnings from these directories
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ext/<SomeHppLib>/include)
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${<SomeLib>_SOURCE_DIR}/include)
#target_link_directories(${PROJECT_NAME} PRIVATE ${<SomeLib>_BINARY_DIR}/lib)
#target_link_libraries(${PROJECT_NAME} <SomeLib>)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${centurion_SOURCE_DIR}/src/")
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)

###############################################################################

# post build steps

# copy resources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${PROJECT_SOURCE_DIR}/resources/open_sans/OpenSans-Regular.ttf $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# on windows: copy SDL dlls into binary dir
if (WIN32)
    get_target_property(SDL_BIN SDL2::SDL2 IMPORTED_LOCATION)
    get_target_property(SDL_image_BIN SDL2_image::SDL2_image IMPORTED_LOCATION)
    get_target_property(SDL_mixer_BIN SDL2_mixer::SDL2_mixer IMPORTED_LOCATION)
    get_target_property(SDL_ttf_BIN SDL2_ttf::SDL2_ttf IMPORTED_LOCATION)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_BIN} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_image_BIN} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_mixer_BIN} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL_ttf_BIN} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif ()

###############################################################################

# copy binaries to bin folder; these are uploaded as artifacts on each release
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(DIRECTORY some_dir1 some_dir2 DESTINATION bin)
# install(FILES some_file1.txt some_file2.md DESTINATION bin)

install(FILES "${PROJECT_SOURCE_DIR}/resources/open_sans/OpenSans-Regular.ttf" DESTINATION bin)

if (WIN32)
    install(FILES ${SDL_BIN} DESTINATION bin)
    install(FILES ${SDL_image_BIN} DESTINATION bin)
    install(FILES ${SDL_mixer_BIN} DESTINATION bin)
    install(FILES ${SDL_ttf_BIN} DESTINATION bin)
endif()
