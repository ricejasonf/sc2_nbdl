add_executable(sc2_nbdl.server main.cpp)

add_definitions(-DBOOST_ASIO_STANDALONE -DBOOST_ASIO_NO_TYPEID)

target_link_libraries(
  sc2_nbdl.server
  PRIVATE ${JsonCpp}
  PRIVATE ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(
  sc2_nbdl.server
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../include
)

add_custom_target(
  run.sc2_nbdl.server
  COMMAND "sc2_nbdl.server"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS sc2_nbdl.server
  COMMENT "Build and run sc2_nbdl.server"
)

install(
  TARGETS sc2_nbdl.server
  RUNTIME DESTINATION bin
)
