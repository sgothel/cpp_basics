#
# jaulib cmake build settings, modularized to be optionally included by parent projects
#

include_guard(GLOBAL)

macro(JaulibPreset)
    # Poor man's IDE integration, hard-coded presets for undefined variables
    message(STATUS "JaulibPreset: Start")
    if( (NOT DEFINED JAU_CMAKE_ENFORCE_PRESETS) AND (DEFINED ENV{JAU_CMAKE_ENFORCE_PRESETS}) )
        set (JAU_CMAKE_ENFORCE_PRESETS $ENV{JAU_CMAKE_ENFORCE_PRESETS})
        message(STATUS "JaulibPreset: JAU_CMAKE_ENFORCE_PRESETS -> ${JAU_CMAKE_ENFORCE_PRESETS} (env)")
    endif()
    if( JAU_CMAKE_ENFORCE_PRESETS OR 
        ( (NOT DEFINED CMAKE_INSTALL_PREFIX) AND (NOT DEFINED CMAKE_CXX_CLANG_TIDY) ) )
        message(STATUS "JaulibPreset: Enforcing hardcoded CMake Presets!")
        if(JAU_CMAKE_ENFORCE_PRESETS)
            message(STATUS "JaulibPreset: ... triggered by CMake variable JAU_CMAKE_ENFORCE_PRESETS ${JAU_CMAKE_ENFORCE_PRESETS}.")
        elseif( (NOT DEFINED CMAKE_INSTALL_PREFIX) AND (NOT DEFINED CMAKE_CXX_CLANG_TIDY) )
            message(STATUS "JaulibPreset: ... triggered by undefined CMAKE_INSTALL_PREFIX && CMAKE_CXX_CLANG_TIDY.")
        endif()
        if (DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
            message(STATUS "JaulibPreset: Parallel build: CMAKE_BUILD_PARALLEL_LEVEL = $ENV{CMAKE_BUILD_PARALLEL_LEVEL}.")
        else()
            message(STATUS "JaulibPreset: Parallel build: Consider setting environment variable CMAKE_BUILD_PARALLEL_LEVEL.")
        endif()
        #
        # Defaulting presets: clang, clang-tidy, C++20, CMAKE_BUILD_TYPE, +testing
        #
        if( (NOT DEFINED CMAKE_INSTALL_PREFIX) )
            set(JAU_CMAKE_OVERRIDE_INSTALL_PREFIX ON)
            message(STATUS "JaulibPreset: Setting CMAKE_INSTALL_PREFIX earmarked")
        endif()
        if(NOT DEFINED CMAKE_CXX_STANDARD)
            set(CMAKE_CXX_STANDARD 20)
            message(STATUS "JaulibPreset: Setting CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD}")
        endif()
        if(NOT DEFINED CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE "Debug")
            message(STATUS "JaulibPreset: Setting CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
        endif()
        if( (NOT DEFINED BUILD_TESTING) )
            set(BUILD_TESTING ON)
            message(STATUS "JaulibPreset: Setting BUILD_TESTING ${BUILD_TESTING}")
        endif()
        if( (NOT DEFINED CMAKE_C_COMPILER) )
            set(CMAKE_C_COMPILER "clang")
            message(STATUS "JaulibPreset: Setting CMAKE_C_COMPILER ${CMAKE_C_COMPILER}")
        endif()
        if( (NOT DEFINED CMAKE_CXX_COMPILER) )
            set(CMAKE_CXX_COMPILER "clang++")
            message(STATUS "JaulibPreset: Setting CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER}")
        endif()
        if( (NOT DEFINED CMAKE_CXX_CLANG_TIDY) )
            set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-p;${CMAKE_BINARY_DIR}")
            message(STATUS "JaulibPreset: Setting CMAKE_CXX_CLANG_TIDY ${CMAKE_CXX_CLANG_TIDY}")
        endif()
    endif()
    message(STATUS "JaulibPreset: End")
endmacro()

macro(JaulibSetup)
message(STATUS "JaulibSetup: Start: ${PROJECT_NAME}")

set(ENV{LANG} en_US.UTF-8)
set(ENV{LC_MEASUREMENT} en_US.UTF-8)

# Determine OS_AND_ARCH as library appendix, e.g. 'direct_bt-linux-amd64'
string(TOLOWER ${CMAKE_SYSTEM_NAME} OS_NAME)
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} OS_ARCH0)
if(${OS_ARCH0} STREQUAL "arm")
    set(OS_ARCH "armhf")
elseif(${OS_ARCH0} STREQUAL "armv7l")
    set(OS_ARCH "armhf")
elseif(${OS_ARCH0} STREQUAL "aarch64")
    set(OS_ARCH "arm64")
elseif(${OS_ARCH0} STREQUAL "x86_64")
    set(OS_ARCH "amd64")
elseif( ( ${OS_ARCH0} STREQUAL "x86" ) OR ( ${OS_ARCH0} STREQUAL "i386" ) ( ${OS_ARCH0} STREQUAL "i686" ) )
    set(OS_ARCH "i586")
else()
    set(OS_ARCH ${OS_ARCH0})
endif()
set(OS_AND_ARCH ${OS_NAME}-${OS_ARCH})
set(os_and_arch_slash ${OS_NAME}/${OS_ARCH})
set(os_and_arch_dot ${OS_NAME}.${OS_ARCH})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(TOOLSET "clang")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(TOOLSET "gcc")
else()
    string(TOLOWER ${CMAKE_CXX_COMPILER_ID} TOOLSET)
endif()
if(JAU_CMAKE_OVERRIDE_INSTALL_PREFIX)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/dist-debug")
    else()
        set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/dist-default")
    endif()
    message(STATUS "JaulibSetup: Setting CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")
endif()
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}-${OS_AND_ARCH}-${TOOLSET}")

message (STATUS "JaulibSetup: OS_NAME ${OS_NAME}")
message (STATUS "JaulibSetup: OS_ARCH ${OS_ARCH} (${CMAKE_SYSTEM_PROCESSOR})")
message (STATUS "JaulibSetup: OS_AND_ARCH ${OS_AND_ARCH}")
message (STATUS "JaulibSetup: TOOLSET ${TOOLSET}")
message (STATUS "JaulibSetup: CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG ON)
endif()
if(NOT DEFINED CC_INSTRUMENTATION)
    set(CC_INSTRUMENTATION OFF)
endif()
if(NOT DEFINED CC_INSTRUMENTATION_UNDEFINED)
    set(CC_INSTRUMENTATION_UNDEFINED OFF)
endif()
if(NOT DEFINED CC_INSTRUMENTATION_THREAD)
    set(CC_INSTRUMENTATION_THREAD OFF)
endif()
if(CC_INSTRUMENTATION OR CC_INSTRUMENTATION_UNDEFINED OR CC_INSTRUMENTATION_THREAD)
    set(DEBUG ON)
endif()
message(STATUS "JaulibSetup: Compiler = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "JaulibSetup: Build Type = ${CMAKE_BUILD_TYPE}")
message(STATUS "JaulibSetup: DEBUG = ${DEBUG}")
message(STATUS "JaulibSetup: CC_INSTRUMENTATION = All ${CC_INSTRUMENTATION}, Undef ${CC_INSTRUMENTATION_UNDEFINED}, Thread ${CC_INSTRUMENTATION_THREAD}")

if(DEFINED CMAKE_CXX_CLANG_TIDY)
    message(STATUS "JaulibSetup: clang-tidy preset: ${CMAKE_CXX_CLANG_TIDY}")
else()
    message(STATUS "JaulibSetup: clang-tidy not used")
endif()

if(DEFINED CMAKE_CXX_STANDARD)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
    message(STATUS "JaulibSetup: CMAKE_CXX_STANDARD (preset): ${CMAKE_CXX_STANDARD}, CMAKE_CXX_STANDARD_REQUIRED: ${CMAKE_CXX_STANDARD_REQUIRED}")
else()
    set (CMAKE_CXX_STANDARD 20)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
    message(STATUS "JaulibSetup: CMAKE_CXX_STANDARD (default): ${CMAKE_CXX_STANDARD}, CMAKE_CXX_STANDARD_REQUIRED: ${CMAKE_CXX_STANDARD_REQUIRED}")
endif()

#
# Post initial setup / var-check
#

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for all
set (CC_FLAGS_WARNING "-Wall -Wextra -Werror")
set (GCC_FLAGS_WARNING_FORMAT "-Wformat=2 -Wformat-overflow=2 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat-y2k")
set (GCC_FLAGS_WARNING "-Wall -Wextra -Wshadow -Wtype-limits -Wsign-compare -Wcast-align=strict -Wnull-dereference -Winit-self ${GCC_FLAGS_WARNING_FORMAT} -Werror")
# causes issues in jau::get_int8(..): "-Wnull-dereference"
set (GCC_FLAGS_WARNING_NO_ERROR "-Wno-error=array-bounds -Wno-error=null-dereference -Wno-multichar")

# too pedantic, but nice to check once in a while
# set (DISABLED_CC_FLAGS_WARNING "-Wsign-conversion")

# debug only
set (GCC_FLAGS_STACK "-fstack-protector-strong")
set (GCC_FLAGS_SANITIZE_ALL "-fsanitize-address-use-after-scope -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=undefined -fsanitize=leak -fsanitize-recover=address")
set (GCC_FLAGS_SANITIZE_UNDEFINED "-fsanitize=undefined -fsanitize-recover=address")
set (GCC_FLAGS_SANITIZE_THREAD "-fsanitize-address-use-after-scope -fsanitize=undefined -fsanitize=thread -fsanitize-recover=address")
# -fsanitize=address cannot be combined with -fsanitize=thread
# -fsanitize=pointer-compare -fsanitize=pointer-subtract must be combined with -fsanitize=address
# -fsanitize=thread TSAN's lacks ability to properly handle GCC's atomic macros (like helgrind etc), can't check SC-DRF!

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # shorten __FILE__ string and the like ..
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS_WARNING} ${GCC_FLAGS_WARNING_NO_ERROR} -fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=/")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CC_FLAGS_WARNING}")
endif()

message(STATUS "JaulibSetup: USE_STRIP = ${USE_STRIP} (pre-set)")

if(DEBUG)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -DDEBUG -fno-omit-frame-pointer ${GCC_FLAGS_STACK} -no-pie")
        set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -no-pie")
        set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DDEBUG")
    endif()
    if(CC_INSTRUMENTATION)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS_SANITIZE_ALL}")
        endif()
    elseif(CC_INSTRUMENTATION_UNDEFINED)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS_SANITIZE_UNDEFINED}")
        endif()
    elseif(CC_INSTRUMENTATION_THREAD)
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_FLAGS_SANITIZE_THREAD}")
        endif()
    endif(CC_INSTRUMENTATION)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    if(GPROF)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -pg")
    elseif(PERF_ANALYSIS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb")
    else()
        find_program(STRIP strip)
        if (STRIP STREQUAL "STRIP-NOTFOUND")
            set(USE_STRIP OFF)
            message(STATUS "JaulibSetup: USE_STRIP:=false, strip not found")
        elseif(NOT DEFINED USE_STRIP)
            set(USE_STRIP ON)
            message(STATUS "JaulibSetup: USE_STRIP:=true, !DEBUG and not set")
        endif()
    endif()
endif(DEBUG)

message(STATUS "JaulibSetup: USE_STRIP = ${USE_STRIP} (final)")

if(${OS_NAME} STREQUAL "freebsd")
    set (SYS_INCLUDE_DIRS
      /usr/include
      /usr/local/include
    )
    set(CMAKE_SYSTEM_PREFIX_PATH "/usr;/usr/local")
else()
    set (SYS_INCLUDE_DIRS
      /usr/include
    )
endif()

if(${OS_NAME} STREQUAL "linux")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_STANDARD_LIBRARIES  "${CMAKE_CXX_STANDARD_LIBRARIES} -latomic")
    else()
        set(CMAKE_CXX_STANDARD_LIBRARIES  "${CMAKE_CXX_STANDARD_LIBRARIES} -latomic")
    endif()
endif()

set (LIB_INSTALL_DIR "lib${LIB_SUFFIX}" CACHE PATH "Installation path for libraries")

message(STATUS "JaulibSetup: CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message(STATUS "JaulibSetup: CMAKE_SHARED_LINKER_FLAGS = ${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "JaulibSetup: CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "JaulibSetup: CMAKE_CXX_STANDARD_LIBRARIES = ${CMAKE_CXX_STANDARD_LIBRARIES}")
message(STATUS "JaulibSetup: LIB_INSTALL_DIR = ${LIB_INSTALL_DIR}")

# Set CMAKE_INSTALL_XXXDIR (XXX {BIN LIB ..} if not defined
# (was: CMAKE_LIB_INSTALL_DIR)
include(GNUInstallDirs)

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

string(TIMESTAMP BUILD_TSTAMP "%Y-%m-%d %H:%M:%S")

message(STATUS "JaulibSetup: End: ${PROJECT_NAME}")

endmacro()

