set(CMAKE_CXX_FLAGS -DTIXML_USE_STL)
set(LIBRARY_OUTPUT_PATH ${MOONHOPPER_LIB_PATH})
set(SRC
    tinystr.cpp
    tinystr.h
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxml.h
    tinyxmlparser.cpp
)

add_library(tinyxml SHARED ${SRC})

