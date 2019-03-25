add_executable(sc2_nbdl.web_app main.cpp)

# TODO I think there is a way to NO_EXIT_RUNTIME w/o a link flag
set_target_properties(
  sc2_nbdl.web_app
  PROPERTIES LINK_FLAGS "--bind -s NO_EXIT_RUNTIME=1"
)

# TODO this could be moved to quick_project or something
function(add_www_copy_file target_ file_)
  add_custom_target(
    ${target_}.www.${file_}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/www/${file_}
                                     ${CMAKE_CURRENT_BINARY_DIR}/${file_}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/www/${file_}
  )
  add_dependencies(${target_} ${target_}.www.${file_})
endfunction()

target_include_directories(
  sc2_nbdl.web_app
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../include
)

add_www_copy_file(sc2_nbdl.web_app index.html)
