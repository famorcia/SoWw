function(executable executable_name)
  set(multiValueArgs SOURCES LIBS)
  cmake_parse_arguments(executable "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  add_executable(${executable_name} ${executable_SOURCES})
  target_link_libraries(${executable_name} ${executable_LIBS})
  if (HAVE_ASAN)
    target_compile_options(${executable_name} PUBLIC "-fsanitize=address")
    set_target_properties(${executable_name} PROPERTIES LINK_FLAGS "-fsanitize=address")
  endif()
endfunction(executable)

dump_variable(
Coin_LIBRARY
WXWIDGETS_LIBRARIES
)

include(${wxWidgets_USE_FILE})
add_definitions(-DWXINTL_NO_GETTEXT_MACRO)

include_directories(
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
  ${wxWidgets_INCLUDE_DIRS}
)

add_executable(crash assertdbg/crash.cpp)
target_link_libraries(crash SoWw)

add_executable(coloreditor components/coloreditor.cpp)
target_link_libraries(coloreditor SoWw)

# add_executable(embeddedexaminer components/embeddedexaminer.cpp)
# target_link_libraries(embeddedexaminer SoWw)

add_executable(renderarea components/renderarea.cpp)
target_link_libraries(renderarea SoWw)

add_executable(examinerviewer components/examinerviewer.cpp)
target_link_libraries(examinerviewer SoWw)

#add_executable(scrollview components/scrollview.cpp)
#target_link_libraries(scrollview SoWw)

#add_executable(tripleview components/tripleview.cpp)
#target_link_libraries(tripleview SoWw)

add_executable(withoutdecor0 components/withoutdecor0.cpp)
target_link_libraries(withoutdecor0 SoWw)

add_executable(withoutdecor1 components/withoutdecor1.cpp)
target_link_libraries(withoutdecor1 SoWw)

add_executable(thumbwheel widgets/thumbwheel.cpp)
target_link_libraries(thumbwheel SoWw)

add_subdirectory(Mentor)