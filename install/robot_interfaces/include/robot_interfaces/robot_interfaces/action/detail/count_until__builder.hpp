// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_interfaces:action/CountUntil.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__ACTION__DETAIL__COUNT_UNTIL__BUILDER_HPP_
#define ROBOT_INTERFACES__ACTION__DETAIL__COUNT_UNTIL__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_interfaces/action/detail/count_until__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_Goal_period
{
public:
  explicit Init_CountUntil_Goal_period(::robot_interfaces::action::CountUntil_Goal & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_Goal period(::robot_interfaces::action::CountUntil_Goal::_period_type arg)
  {
    msg_.period = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Goal msg_;
};

class Init_CountUntil_Goal_target_number
{
public:
  explicit Init_CountUntil_Goal_target_number(::robot_interfaces::action::CountUntil_Goal & msg)
  : msg_(msg)
  {}
  Init_CountUntil_Goal_period target_number(::robot_interfaces::action::CountUntil_Goal::_target_number_type arg)
  {
    msg_.target_number = std::move(arg);
    return Init_CountUntil_Goal_period(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Goal msg_;
};

class Init_CountUntil_Goal_goal_number
{
public:
  Init_CountUntil_Goal_goal_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_Goal_target_number goal_number(::robot_interfaces::action::CountUntil_Goal::_goal_number_type arg)
  {
    msg_.goal_number = std::move(arg);
    return Init_CountUntil_Goal_target_number(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Goal msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_Goal>()
{
  return robot_interfaces::action::builder::Init_CountUntil_Goal_goal_number();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_Result_reached_number
{
public:
  explicit Init_CountUntil_Result_reached_number(::robot_interfaces::action::CountUntil_Result & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_Result reached_number(::robot_interfaces::action::CountUntil_Result::_reached_number_type arg)
  {
    msg_.reached_number = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Result msg_;
};

class Init_CountUntil_Result_goal_number
{
public:
  Init_CountUntil_Result_goal_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_Result_reached_number goal_number(::robot_interfaces::action::CountUntil_Result::_goal_number_type arg)
  {
    msg_.goal_number = std::move(arg);
    return Init_CountUntil_Result_reached_number(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Result msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_Result>()
{
  return robot_interfaces::action::builder::Init_CountUntil_Result_goal_number();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_Feedback_current_number
{
public:
  explicit Init_CountUntil_Feedback_current_number(::robot_interfaces::action::CountUntil_Feedback & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_Feedback current_number(::robot_interfaces::action::CountUntil_Feedback::_current_number_type arg)
  {
    msg_.current_number = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Feedback msg_;
};

class Init_CountUntil_Feedback_goal_number
{
public:
  Init_CountUntil_Feedback_goal_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_Feedback_current_number goal_number(::robot_interfaces::action::CountUntil_Feedback::_goal_number_type arg)
  {
    msg_.goal_number = std::move(arg);
    return Init_CountUntil_Feedback_current_number(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_Feedback msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_Feedback>()
{
  return robot_interfaces::action::builder::Init_CountUntil_Feedback_goal_number();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_SendGoal_Request_goal
{
public:
  explicit Init_CountUntil_SendGoal_Request_goal(::robot_interfaces::action::CountUntil_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_SendGoal_Request goal(::robot_interfaces::action::CountUntil_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_SendGoal_Request msg_;
};

class Init_CountUntil_SendGoal_Request_goal_id
{
public:
  Init_CountUntil_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_SendGoal_Request_goal goal_id(::robot_interfaces::action::CountUntil_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_CountUntil_SendGoal_Request_goal(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_SendGoal_Request>()
{
  return robot_interfaces::action::builder::Init_CountUntil_SendGoal_Request_goal_id();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_SendGoal_Response_stamp
{
public:
  explicit Init_CountUntil_SendGoal_Response_stamp(::robot_interfaces::action::CountUntil_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_SendGoal_Response stamp(::robot_interfaces::action::CountUntil_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_SendGoal_Response msg_;
};

class Init_CountUntil_SendGoal_Response_accepted
{
public:
  Init_CountUntil_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_SendGoal_Response_stamp accepted(::robot_interfaces::action::CountUntil_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_CountUntil_SendGoal_Response_stamp(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_SendGoal_Response>()
{
  return robot_interfaces::action::builder::Init_CountUntil_SendGoal_Response_accepted();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_GetResult_Request_goal_id
{
public:
  Init_CountUntil_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::robot_interfaces::action::CountUntil_GetResult_Request goal_id(::robot_interfaces::action::CountUntil_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_GetResult_Request>()
{
  return robot_interfaces::action::builder::Init_CountUntil_GetResult_Request_goal_id();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_GetResult_Response_result
{
public:
  explicit Init_CountUntil_GetResult_Response_result(::robot_interfaces::action::CountUntil_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_GetResult_Response result(::robot_interfaces::action::CountUntil_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_GetResult_Response msg_;
};

class Init_CountUntil_GetResult_Response_status
{
public:
  Init_CountUntil_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_GetResult_Response_result status(::robot_interfaces::action::CountUntil_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_CountUntil_GetResult_Response_result(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_GetResult_Response>()
{
  return robot_interfaces::action::builder::Init_CountUntil_GetResult_Response_status();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_CountUntil_FeedbackMessage_feedback
{
public:
  explicit Init_CountUntil_FeedbackMessage_feedback(::robot_interfaces::action::CountUntil_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::CountUntil_FeedbackMessage feedback(::robot_interfaces::action::CountUntil_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_FeedbackMessage msg_;
};

class Init_CountUntil_FeedbackMessage_goal_id
{
public:
  Init_CountUntil_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CountUntil_FeedbackMessage_feedback goal_id(::robot_interfaces::action::CountUntil_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_CountUntil_FeedbackMessage_feedback(msg_);
  }

private:
  ::robot_interfaces::action::CountUntil_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::CountUntil_FeedbackMessage>()
{
  return robot_interfaces::action::builder::Init_CountUntil_FeedbackMessage_goal_id();
}

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__ACTION__DETAIL__COUNT_UNTIL__BUILDER_HPP_
