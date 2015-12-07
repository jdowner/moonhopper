set(CONFIGS
  default.cfg
  )

set(IMAGES
  avatar.png
  dialog.png
  hook.png
  moon.png
  running.png
  )

set(RESOURCES_CFG ${MOONHOPPER_RES_PATH}/config)
set(RESOURCES_IMG ${MOONHOPPER_RES_PATH}/images)

add_custom_command(
  OUTPUT resource-dir POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MOONHOPPER_RES_PATH}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MOONHOPPER_RES_PATH}/config
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MOONHOPPER_RES_PATH}/images
  )

add_custom_target(resources ALL DEPENDS resource-dir)

foreach(cfg ${CONFIGS})
  add_custom_command(
    TARGET resources POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${cfg} ${RESOURCES_CFG}
    COMMAND ${CMAKE_COMMAND} -E echo "copying ${cfg} to ${RESOURCES_CFG}"
    DEPENDS resource-dir ${CMAKE_CURRENT_SOURCE_DIR}/${cfg}
    )
endforeach(cfg ${CONFIGS})

foreach(image ${IMAGES})
  add_custom_command(
    TARGET resources POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${image} ${RESOURCES_IMG}
    COMMAND ${CMAKE_COMMAND} -E echo "copying ${image} to ${RESOURCES_IMG}"
    DEPENDS resource-dir ${CMAKE_CURRENT_SOURCE_DIR}/${image}
    )
endforeach(image ${IMAGES})
