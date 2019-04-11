/* Raven 2 Control - Control software for the Raven II robot
 * Copyright (C) 2005-2018  Andrew Lewis, Yun-Hsuan Su, Blake Hannaford, 
 * and the University of Washington BioRobotics Laboratory
 *
 * This file is part of Raven 2 Control.
 *
 * Raven 2 Control is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Raven 2 Control is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Raven 2 Control.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * crtk_state.cpp
 *
 * \brief Class file for CRTK API state and status flags
 *
 *
 * \date Oct 18, 2018
 * \author Andrew Lewis
 * \author Melody Yun-Hsuan Su
 *
 */

#include "ros/ros.h"
#include "crtk_robot_state.h"

#include <sstream>

CRTK_robot_state::CRTK_robot_state(){
  is_disabled   = 0;
  is_enabled    = 0;
  is_paused     = 0;
  is_fault      = 0;
  is_homing     = 0;
  is_busy       = 0;
  is_ready      = 0;
  is_homed      = 0;

  has_connected = 0;

  ROS_INFO("def state cons called");
}

CRTK_robot_state::CRTK_robot_state(ros::NodeHandle n){
  is_disabled   = 0;
  is_enabled    = 0;
  is_paused     = 0;
  is_fault      = 0;
  is_homing     = 0;
  is_busy       = 0;
  is_ready      = 0;
  is_homed      = 0;

  has_connected = 0;
  init_ros(n);

  ROS_INFO("NH state cons called");
}


/**
 * @brief      initializes ros pubs and subs
 *
 * @param[in]  n     ros nodehandle
 *
 * @return     success
 */
bool CRTK_robot_state::init_ros(ros::NodeHandle n){

  pub = n.advertise<crtk_msgs::StringStamped>("crtk_command", 1);
  sub = n.subscribe("crtk_state", 1, &CRTK_robot_state::crtk_state_cb,this);

  return true;
}


/**
 * @brief      updates local copy of robot's state based on the messages from the robot
 *
 * @param[in]  msg   The message from ROS
 */
void CRTK_robot_state::crtk_state_cb(crtk_msgs::operating_state msg){

  std::string state = msg.state;

  if (state =="DISABLED"){
    set_disabled_state();
  }
  else if (state =="ENABLED"){
    set_enabled_state();
  }

  else if (state =="PAUSED"){
    set_paused_state();
  }
  else if (state =="FAULT"){
    set_fault_state();
  }
  else{
    set_fault_state();
  }


  set_homed(msg.is_homed);
  set_busy(msg.is_busy);
  set_homing();
  // set_ready(msg.is_ready);
  ready_logic();

  set_connected(1);

  static int count = 0;
  // ROS_INFO("sub count = %i: I heard that the robot is in [%i,%i,%i,%i] %i.", count, msg.is_disabled, msg.is_enabled, msg.is_paused, msg.is_fault, has_connected);
  ++count;



}


void CRTK_robot_state::crtk_command_pb(CRTK_robot_command command){
  
  static int count = 0;
  static crtk_msgs::StringStamped msg_command;

  //robot command supports ("ENABLE", "DISABLE", "PAUSE", "RESUME", "NULL")


  switch(command)
  {
    case CRTK_ENABLE:
      msg_command.data = "enable";
      ROS_INFO("Sent ENABLE: Please press silver button!"); 
      break;

    case CRTK_DISABLE:
      msg_command.data = "disable";
      ROS_INFO("Sent DISABLE."); 
      break;

    case CRTK_PAUSE:
      msg_command.data = "pause";
      ROS_INFO("Sent PAUSE."); 
      break;

    case CRTK_RESUME:
      msg_command.data = "resume";
      ROS_INFO("Sent RESUME."); 
      break;

    case CRTK_UNHOME:
      msg_command.data = "unhome";
      ROS_INFO("Sent UNHOME."); 
      break;

    case CRTK_HOME:
      msg_command.data = "home";
      ROS_INFO("Sent HOME."); 
      break;

    default:
      msg_command.data = "NULL";
      ROS_INFO("Sent NULL."); 
      break;
  }
  msg_command.header.stamp = msg_command.header.stamp.now();
  pub.publish(msg_command);
  
  // ROS_INFO("pub count = %i \t command: %s.", count, msg_command.data.c_str());

  ++count;

}

CRTK_robot_state_enum CRTK_robot_state::get_state(){
  if(is_disabled){
    return CRTK_DISABLED;
  }
  else if(is_enabled){
    return CRTK_ENABLED;
  }
  else if(is_paused){
    return CRTK_PAUSED;
  }
  else{
    return CRTK_FAULT;
  }
}

bool CRTK_robot_state::set_homing(){
  is_homing = is_busy && !is_homed;
  return is_homing;
}
bool CRTK_robot_state::set_busy(bool new_state){
  is_busy = new_state;
  return is_busy;
}
// bool CRTK_robot_state::set_ready(bool new_state){
//   is_ready = new_state;
//   return is_ready;
// }
bool CRTK_robot_state::set_homed(bool new_state){
  is_homed = new_state;
  return is_homed;
}

bool CRTK_robot_state::ready_logic(){
  is_ready = !is_busy && is_homed && is_enabled;
  return is_ready;
}


bool CRTK_robot_state::set_disabled_state(){
  is_disabled   = 1;
  is_enabled    = 0;
  is_paused     = 0;  
  is_fault      = 0;

  return 0;
}
bool CRTK_robot_state::set_enabled_state(){
  is_disabled   = 0;
  is_enabled    = 1;
  is_paused     = 0;  
  is_fault      = 0;

  return 0;
}
bool CRTK_robot_state::set_paused_state(){
  is_disabled   = 0;
  is_enabled    = 0;
  is_paused     = 1;  
  is_fault      = 0;

  return 0;
}
bool CRTK_robot_state::set_fault_state(){
  is_disabled   = 0;
  is_enabled    = 0;
  is_paused     = 0;  
  is_fault      = 1;

  return 0;
}

bool CRTK_robot_state::set_connected(bool val){
  if(val == 0 || val == 1)
    has_connected = val;
}

char CRTK_robot_state::state_char(){
  char out;

  if(is_disabled)       out = 'D';
  else if (is_enabled)  out = 'E';
  else if (is_paused)   out = 'P';
  else if (is_fault)    out = 'F';
  else out = 'N'; //no connection?

  return out;
}


bool CRTK_robot_state::get_disabled(){
  return is_disabled;
}
bool CRTK_robot_state::get_enabled(){
  return is_enabled;
}
bool CRTK_robot_state::get_paused(){
  return is_paused;
}
bool CRTK_robot_state::get_fault(){
  return is_fault;
}
bool CRTK_robot_state::get_homing(){
  return is_homing;
}
bool CRTK_robot_state::get_busy(){
  return is_busy;
}
bool CRTK_robot_state::get_ready(){
  return is_ready;
}
bool CRTK_robot_state::get_homed(){
  return is_homed;
}
bool CRTK_robot_state::get_connected(){
  return has_connected;
}
