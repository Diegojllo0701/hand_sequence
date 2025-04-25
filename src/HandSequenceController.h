#pragma once

#include <mc_control/mc_controller.h>


#include "api.h"

struct HandSequenceController_DLLAPI HandSequenceController : public mc_control::MCController
{
  HandSequenceController(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config);

  bool run() override;

  void reset(const mc_control::ControllerResetData & reset_data) override;
};