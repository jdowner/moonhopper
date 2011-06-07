set(CMAKE_CXX_FLAGS -DTIXML_USE_STL)
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")
set(SRC
    tinystr.cpp
    tinystr.h
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxml.h
    tinyxmlparser.cpp
)

add_library(tinyxml SHARED ${SRC})

