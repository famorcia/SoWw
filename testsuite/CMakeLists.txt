enable_testing()

include(${wxWidgets_USE_FILE})
add_definitions(-DWXINTL_NO_GETTEXT_MACRO -DSOWW_INTERNAL=1)

include_directories(
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${wxWidgets_INCLUDE_DIRS}
)

add_subdirectory(devices)
add_subdirectory(widgets)
