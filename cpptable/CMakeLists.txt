set(CPPTABLE_LIB_NAME cpptable CACHE INTERNAL "cpptable library name")

add_library(
    ${CPPTABLE_LIB_NAME} INTERFACE
    include/cpptable/cpptable.hpp
    include/cpptable/format.hpp
    include/cpptable/nullable.hpp
    include/cpptable/utils.hpp
)
add_library(${PROJECT_NAME}::${CPPTABLE_LIB_NAME} ALIAS ${CPPTABLE_LIB_NAME})

target_include_directories(
    ${CPPTABLE_LIB_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(
    ${CPPTABLE_LIB_NAME}
    PROPERTIES
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${CPPTABLE_LIB_NAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${CPPTABLE_LIB_NAME}-config.cmake
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}/${CPPTABLE_LIB_NAME}
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${CPPTABLE_LIB_NAME}-config-version.cmake
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${CPPTABLE_LIB_NAME}-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${CPPTABLE_LIB_NAME}-config-version.cmake
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}/${CPPTABLE_LIB_NAME}
)

install(
    TARGETS ${CPPTABLE_LIB_NAME}
    EXPORT ${CPPTABLE_LIB_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT ${CPPTABLE_LIB_NAME}-targets
    FILE ${CPPTABLE_LIB_NAME}-targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}/${CPPTABLE_LIB_NAME}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.ipp"
        PATTERN "*.h"
)
