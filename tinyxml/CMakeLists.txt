set(CMAKE_CXX_FLAGS -DTIXML_USE_STL)
set(LIBRARY_OUTPUT_PATH ${MOONHOPPER_LIB_PATH})
set(TINYXML_INC_PATH ${MOONHOPPER_INC_PATH}/tinyxml)

set(SRC
  tinystr.cpp
  tinyxml.cpp
  tinyxmlerror.cpp
  tinyxmlparser.cpp
  )

set(HDR
  tinyxml.h
  tinystr.h
  )

# Define a command that is responsible for the creation of a tinyxml directory
# where the headers can be placed.
add_custom_command(
  OUTPUT tinyxml-inc-dir PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${TINYXML_INC_PATH}
  )

# Define a target for copying the tinyxml headers
add_custom_target(copy-tinyxml-headers ALL DEPENDS tinyxml-inc-dir)

# Iterate over each of the headers and create a custom command to copy the
# header to the header directory
foreach(header ${HDR})
  add_custom_command(
    TARGET copy-tinyxml-headers PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${header} ${TINYXML_INC_PATH}
    COMMAND ${CMAKE_COMMAND} -E echo "copying ${header} to build area"
    DEPENDS tinyxml-inc-dir ${CMAKE_CURRENT_SOURCE_DIR}/${header}
    )
endforeach(header ${HDRS})

# Define the library creation
add_library(tinyxml SHARED ${SRC})
add_dependencies(tinyxml copy-tinyxml-headers)
