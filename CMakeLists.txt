cmake_minimum_required(VERSION 3.15)

# NOTE: update executable name in .github/workflows/cmake.yml:44 when changing executable name in this file
# for now, the project name is used as the executable name
project(HolocronUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


option(SDL2IMAGE_INSTALL "" OFF) 
option(SDL2TTF_INSTALL "" OFF) 
option(SDL2NET_INSTALL "" OFF)
option(SDL2IMAGE_SAMPLES "" OFF) 
option(SDL2NET_SAMPLES "" OFF) 
option(SDL2TTF_SAMPLES "" OFF) 
option(SDL2MIXER_SAMPLES "" OFF)
option(SDL2TTF_VENDORED "" ON)


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
  
include(FetchContent)
set(SDL_SHARED ON)
set(SDL_STATIC OFF)
FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG        release-2.24.1
        GIT_SHALLOW    1
)
FetchContent_MakeAvailable(SDL2)

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG        release-2.6.2
)

set(SDL2MIXER_OPUS OFF)
FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
        GIT_TAG        release-2.6.2
)

FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
        GIT_TAG        release-2.20.1
)


FetchContent_MakeAvailable(SDL2_image SDL2_ttf SDL2_mixer)
set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2-shared SDL2_image)
set(SDL2_MIXER_LIBRARY ${SDL2_mixer_BINARY_DIR}/bin)
set(SDL2_MIXER_INCLUDE_DIR ${SDL2_mixer_SOURCE_DIR}/include)

set(SDL2_TTF_LIBRARY ${SDL2_ttf_BINARY_DIR}/bin)
set(SDL2_TTF_INCLUDE_DIR ${SDL2_ttf_SOURCE_DIR}/include)

# set(SDL2_MIXER_FOUND, TRUE)
message("AA: ${SDL2_MIXER_INCLUDE_DIR}")
set(SDL2_MIXER_PATH, ${SDL2_mixer_SOURCE_DIR})
FetchContent_Declare(
        Centurion
        GIT_REPOSITORY https://github.com/albin-johansson/centurion
        GIT_TAG        v.7.2.0
)
FetchContent_MakeAvailable(Centurion)

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml when changing name here
add_executable(${PROJECT_NAME} main.cpp)

###############################################################################

# target definitions

if(GITHUB_ACTIONS)
  message("NOTE: GITHUB_ACTIONS defined")
  target_compile_definitions(${PROJECT_NAME} PRIVATE GITHUB_ACTIONS)
endif()

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

# target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SDL_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${Centurion_SOURCE_DIR}/include)
# target_link_directories(${PROJECT_NAME} PRIVATE ${SDL_BINARY_DIR}/lib)
target_link_directories(${PROJECT_NAME} PRIVATE ${Centurion_BINARY_DIR}/lib)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main ${SDL_LIBRARIES})

###############################################################################

# copy binaries to bin folder; these are uploaded as artifacts on each release
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(DIRECTORY some_dir1 some_dir2 DESTINATION bin)
# install(FILES some_file1.txt some_file2.md DESTINATION bin)
