add_executable(sc2_nbdl.server main.cpp)

target_link_libraries(
  sc2_nbdl.server
  PRIVATE ${JsonCpp}
  PRIVATE ${CMAKE_THREADS_LIB_INIT}
)

target_include_directories(
  sc2_nbdl.server
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../include
)

install(
  TARGETS sc2_nbdl.server
  RUNTIME DESTINATION bin
)
