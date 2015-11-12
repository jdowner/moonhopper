set(IMAGES
  avatar.png
  dialog.png
  hook.png
  moon.png
  running.png
  )

add_custom_command(
  OUTPUT resources-cfg POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MOONHOPPER_RES_PATH}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/default.cfg ${MOONHOPPER_RES_PATH}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/default.cfg
  )

foreach(image ${IMAGES})
  set(resource_deps "${resource_deps} resources_${image}")
  add_custom_command(
    OUTPUT "resources_${image}" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${MOONHOPPER_RES_PATH}
    COMMAND ${CMAKE_COMMAND} -E copy ${image} ${MOONHOPPER_RES_PATH}
    COMMAND echo ${image} > ${MOONHOPPER_RES_PATH}
    )
  message(STATUS "copying ${image}")
endforeach()

add_custom_target(resources ALL DEPENDS resources-cfg ${resources_deps})
