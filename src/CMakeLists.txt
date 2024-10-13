include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

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
endforeach()

install(TARGETS ${TARGETS_IDIOMATIC_EXAMPLES} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

