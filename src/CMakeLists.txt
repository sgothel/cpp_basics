include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/catch2_jau
)

set (catch2_LIB_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/catch2/catch_amalgamated.cpp
)

add_library (catch2 STATIC ${catch2_LIB_SRCS})
target_compile_options(catch2 PRIVATE -DCATCH_AMALGAMATED_CUSTOM_MAIN=1 -Wno-error=unused-but-set-variable -Wno-unused-but-set-variable)
set_target_properties(catch2 PROPERTIES CXX_CLANG_TIDY "")
# install(TARGETS catch2 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# These examples use the standard separate compilation
file(GLOB_RECURSE SOURCES_IDIOMATIC_EXAMPLES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "lesson*.cpp" "cs*lesson*.cpp" "example*.cpp")

string( REPLACE ".cpp" "" BASENAMES_IDIOMATIC_EXAMPLES "${SOURCES_IDIOMATIC_EXAMPLES}" )

set( TARGETS_IDIOMATIC_EXAMPLES "" )
foreach( name ${BASENAMES_IDIOMATIC_EXAMPLES} )
    set( TARGETS_IDIOMATIC_EXAMPLES "${TARGETS_IDIOMATIC_EXAMPLES};${name}" )
    if(${name} STREQUAL "lesson01_function")
        add_executable(${name} ${name}.cpp module_lesson01.cpp)
    else()
        add_executable(${name} ${name}.cpp)
    endif()
    target_compile_options(${name} PUBLIC "${cpp_basics_CXX_FLAGS}")
    target_link_options(${name} PUBLIC "${cpp_basics_EXE_LINKER_FLAGS}")
    target_link_libraries(${name} catch2)
    add_dependencies(${name} catch2)
    add_test (NAME ${name} COMMAND ${name})
    if(DEFINED CMAKE_CXX_CLANG_TIDY)
        set_target_properties(${name} PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--checks;-bugprone-chained-comparison")
    endif()
endforeach()

install(TARGETS ${TARGETS_IDIOMATIC_EXAMPLES} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

