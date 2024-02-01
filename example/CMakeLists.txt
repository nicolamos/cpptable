CPMAddPackage("gh:gsl-lite/gsl-lite#v0.41.0")

set(MP_UNITS_USE_FMTLIB YES)

FetchContent_Declare(
    mp-units
    GIT_REPOSITORY "https://github.com/mpusz/mp-units.git"
    #GIT_TAG v2.1.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mp-units
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/mp-units-build
)

FetchContent_Populate(mp-units)

if(mp-units_POPULATED)
    add_subdirectory(${mp-units_SOURCE_DIR}/src ${mp-units_BINARY_DIR})
endif()

set(CPPTABLE_EXE_NAME example CACHE INTERNAL "Name of the executable")

set(
    ${CPPTABLE_EXE_NAME}_SOURCES
    src/main.cpp
)

add_executable(${CPPTABLE_EXE_NAME} ${${CPPTABLE_EXE_NAME}_SOURCES})

target_link_libraries(
    ${CPPTABLE_EXE_NAME}
PUBLIC
    ${PROJECT_NAME}::${CPPTABLE_LIB_NAME}
    fmt::fmt
    mp-units::mp-units
)

target_compile_options(
    ${CPPTABLE_EXE_NAME}
PRIVATE
    ${CPPTABLE_WARNING_FLAGS}
)

set_target_properties(
    ${CPPTABLE_EXE_NAME}
    PROPERTIES VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)

install(
    TARGETS ${CPPTABLE_EXE_NAME}
    OPTIONAL
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
