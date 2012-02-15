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

find_package(Boost 1.46.1 COMPONENTS thread REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()


find_package(LibXml2 REQUIRED)
if(LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIR})
endif()

find_package(LibXslt REQUIRED)
if(LIBXSLT_FOUND)
  include_directories(${LIBXSLT_INCLUDE_DIR})
endif()

find_package(LibExslt REQUIRED)
if(LIBEXSLT_FOUND)
  include_directories(${LIBEXSLT_INCLUDE_DIR})
endif()

find_package(AlpinoCorpus REQUIRED)
if(ALPINOCORPUS_FOUND)
        INCLUDE_DIRECTORIES(${ALPINOCORPUS_INCLUDE_DIR})
endif(ALPINOCORPUS_FOUND)

set(PYTHON_HEADERS
  src/CorpusReader.hh
  src/Entry.hh
  src/EntryIterator.hh
  src/MarkerQuery.hh
  src/alpinocorpus.h
  src/boost_wrap.hh
)

set(PYTHON_SOURCES
  src/CorpusReader.cpp
  src/Entry.cpp
  src/EntryIterator.cpp
  src/MarkerQuery.cpp
  src/alpinocorpus.cpp
  src/boost_wrap.cpp
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
target_link_libraries(alpinocorpus ${Boost_LIBRARIES})
target_link_libraries(alpinocorpus ${ALPINOCORPUS_LIBRARIES})
target_link_libraries(alpinocorpus ${LIBXML2_LIBRARIES})
target_link_libraries(alpinocorpus ${LIBXSLT_LIBRARIES})
target_link_libraries(alpinocorpus ${LIBEXSLT_LIBRARIES})
