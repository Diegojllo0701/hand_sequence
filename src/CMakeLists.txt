set(controller_SRC
  HandSequenceController.cpp
)

set(controller_HDR
  HandSequenceController.h
)

add_controller(${PROJECT_NAME} ${controller_SRC} ${controller_HDR})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DHandSequenceController_EXPORTS")