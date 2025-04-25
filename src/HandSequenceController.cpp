#include "HandSequenceController.h"

HandSequenceController::HandSequenceController(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config)
: mc_control::MCController(rm, dt)
{
  solver().addConstraintSet(contactConstraint);
  solver().addConstraintSet(kinematicsConstraint);
  solver().addTask(postureTask);
  solver().setContacts({{}});

  mc_rtc::log::success("HandSequenceController init done ");
}

bool HandSequenceController::run()
{
  return mc_control::MCController::run();
}

void HandSequenceController::reset(const mc_control::ControllerResetData & reset_data)
{
  mc_control::MCController::reset(reset_data);
}

CONTROLLER_CONSTRUCTOR("HandSequenceController", HandSequenceController)
