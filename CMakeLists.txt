cmake_minimum_required(VERSION 2.6)
project(AlpinoCorpus-Python)

if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wno-long-long")
endif()

list(APPEND CMAKE_MODULE_PATH "${AlpinoCorpus-Python_SOURCE_DIR}/cmake")

find_package(PythonLibs REQUIRED)
if(PYTHONLIBS_FOUND)
  include_directories(${PYTHON_INCLUDE_DIRS})
endif()

find_package(AlpinoCorpus REQUIRED)
if(ALPINOCORPUS_FOUND)
        INCLUDE_DIRECTORIES(${ALPINOCORPUS_INCLUDE_DIR})
endif(ALPINOCORPUS_FOUND)

set(PYTHON_HEADERS
  src/alpinocorpus.h
)

set(PYTHON_SOURCES
  src/alpinocorpus.cpp
)

add_library(alpinocorpus SHARED
  ${PYTHON_SOURCES}
)

set_target_properties(alpinocorpus PROPERTIES LINKER_LANGUAGE C)
set_target_properties(alpinocorpus PROPERTIES PREFIX "")
set_target_properties(alpinocorpus PROPERTIES OUTPUT_NAME "alpinocorpus")

if(APPLE)
  set_target_properties(alpinocorpus PROPERTIES SUFFIX ".so")
endif()

target_link_libraries(alpinocorpus ${PYTHON_LIBRARIES})
target_link_libraries(alpinocorpus alpino_corpus)
