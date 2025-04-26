#pragma once

#include <mc_control/mc_controller.h>
#include <mc_tasks/TransformTask.h>
#include <mc_tasks/LookAtTask.h>
#include <mc_tasks/CoMTask.h>

namespace mc_control {

struct HandSequenceController : public MCController
{
    HandSequenceController(mc_rbdyn::RobotModulePtr rm, double dt, const Configuration & config);

    bool run() override;
    void reset(const ControllerResetData & reset_data) override;

private:
    enum State {
        MoveLeft,
        ReturnLeft,
        MoveRight,
        ReturnRight,
        MoveBoth,
        ReturnBoth
    };

    // Tasks
    std::shared_ptr<mc_tasks::TransformTask> left_hand_task_;
    std::shared_ptr<mc_tasks::TransformTask> right_hand_task_;
    std::shared_ptr<mc_tasks::LookAtTask> gaze_task_;
    std::shared_ptr<mc_tasks::CoMTask> com_task_;

    // Targets
    sva::PTransformd left_target_;
    sva::PTransformd right_target_;

    // Start Poses
    sva::PTransformd left_hand_start_pose_;
    sva::PTransformd right_hand_start_pose_;

    // State
    State current_state_ = MoveLeft;
    double controller_time_ = 0.0;
    double state_start_time_ = 0.0;
    double movement_duration_ = 2.0;

    void startMovement(State new_state);
    void updateGaze();
};

} // namespace mc_control