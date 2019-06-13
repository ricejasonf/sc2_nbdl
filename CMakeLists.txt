cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH /usr/local/cmake) # ??

project(sc2_nbdl_project)
enable_testing()

include(quick_project/Test OPTIONAL RESULT_VARIABLE quick_project)
if (quick_project)
  quick_project_add_check_target()
endif(quick_project)

add_definitions(-std=c++2a)
add_definitions(-DBOOST_HANA_CONFIG_ENABLE_STRING_UDL)
add_definitions(-ftemplate-backtrace-limit=0)
add_definitions(-Wall)
add_definitions(-Wextra)
add_definitions(-DBOOST_ASIO_STANDALONE
                -DBOOST_ASIO_NO_TYPEID
                -DBOOST_ASIO_HAS_STD_STRING_VIEW)

include_directories(${sc2_nbdl_project_SOURCE_DIR}/include)

if (quick_project)
  #add_subdirectory(test)
endif(quick_project)

if (EMSCRIPTEN)
  add_subdirectory(web_app)
else()
  add_subdirectory(server)
endif ()

