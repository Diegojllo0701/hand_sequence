#include "HandSequenceController.h"
#include <mc_rtc/logging.h>

namespace mc_control {

HandSequenceController::HandSequenceController(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config)
: MCController(rm, dt, config)
{
    // Load configuration
    left_target_ = config("left_target", sva::PTransformd{
        Eigen::Quaterniond(0.7, 0, 0.7, 0).normalized(),
        Eigen::Vector3d(0.5, 0.25, 1.1)
    });

    right_target_ = config("right_target", sva::PTransformd{
        Eigen::Quaterniond(0.7, 0, 0.7, 0).normalized(),
        Eigen::Vector3d(0.5, -0.25, 1.1)
    });

    movement_duration_ = config("duration", 2.0);

    // Setup default constraints
    solver().addConstraintSet(contactConstraint);
    solver().addConstraintSet(selfCollisionConstraint);
    solver().addConstraintSet(*compoundJointConstraint);
    solver().addTask(postureTask);
}

void HandSequenceController::reset(const ControllerResetData & reset_data)
{
    MCController::reset(reset_data);

    // Initialize time tracking
    controller_time_ = 0.0;
    state_start_time_ = 0.0;

    // Setup contacts
    mc_rtc::Configuration contact_config;
    contact_config.add("r1", robot().name());
    contact_config.add("r1Surface", "LeftFoot");
    contact_config.add("r2", "ground");
    contact_config.add("r2Surface", "AllGround");
    addContact(contact_config);

    contact_config.add("r1Surface", "RightFoot");
    addContact(contact_config);

    // Create CoM task
    com_task_ = std::make_shared<mc_tasks::CoMTask>(robots(), robots().robotIndex());
    com_task_->weight(1000);
    solver().addTask(com_task_);

    // Create hand tasks
    left_hand_task_ = std::make_shared<mc_tasks::TransformTask>(
        robot().frame("L_WRIST_Y_S"), 5.0, 100.0);
    right_hand_task_ = std::make_shared<mc_tasks::TransformTask>(
        robot().frame("R_WRIST_Y_S"), 5.0, 100.0);

    // Store initial hand poses
    left_hand_start_pose_ = robot().frame("L_WRIST_Y_S").position();
    right_hand_start_pose_ = robot().frame("R_WRIST_Y_S").position();

    // Create gaze task
    gaze_task_ = std::make_shared<mc_tasks::LookAtTask>(
        "NECK_P_S",
        Eigen::Vector3d::UnitX(),
        solver().robots(), 0,
        5.0, 100.0
    );
    solver().addTask(gaze_task_);

    // Start with left hand movement
    current_state_ = MoveLeft;
    startMovement(MoveLeft);
}

bool HandSequenceController::run()
{
    controller_time_ += timeStep;

    if(controller_time_ - state_start_time_ > movement_duration_) {
        switch(current_state_) {
            case MoveLeft:    startMovement(ReturnLeft); break;
            case ReturnLeft:  startMovement(MoveRight); break;
            case MoveRight:   startMovement(ReturnRight); break;
            case ReturnRight: startMovement(MoveBoth); break;
            case MoveBoth:    startMovement(ReturnBoth); break;
            case ReturnBoth:  startMovement(MoveLeft); break;
        }
    }

    return MCController::run();
}

void HandSequenceController::startMovement(State new_state)
{
    current_state_ = new_state;
    state_start_time_ = controller_time_;

    switch(current_state_) {
        case MoveLeft:
            left_hand_task_->target(left_target_);
            solver().addTask(left_hand_task_);
            solver().removeTask(right_hand_task_);
            break;
        case ReturnLeft:
            left_hand_task_->target(left_hand_start_pose_);
            break;
        case MoveRight:
            right_hand_task_->target(right_target_);
            solver().addTask(right_hand_task_);
            solver().removeTask(left_hand_task_);
            break;
        case ReturnRight:
            right_hand_task_->target(right_hand_start_pose_);
            break;
        case MoveBoth:
            left_hand_task_->target(left_target_);
            right_hand_task_->target(right_target_);
            solver().addTask(left_hand_task_);
            solver().addTask(right_hand_task_);
            break;
        case ReturnBoth:
            // Set targets to the stored initial poses of both hands
            left_hand_task_->target(left_hand_start_pose_);
            right_hand_task_->target(right_hand_start_pose_);
            break;
    }
    updateGaze();
}

void HandSequenceController::updateGaze()
{
    switch(current_state_) {
        case MoveLeft:
        case ReturnLeft:
            gaze_task_->target(robot().frame("L_WRIST_Y_S").position().translation());
            break;
        case MoveRight:
        case ReturnRight:
            gaze_task_->target(robot().frame("R_WRIST_Y_S").position().translation());
            break;
        default:
            gaze_task_->target(robot().posW().translation() + Eigen::Vector3d(1.0, 0, 0));
    }
}

} // namespace mc_control

CONTROLLER_CONSTRUCTOR("HandSequenceController", mc_control::HandSequenceController)