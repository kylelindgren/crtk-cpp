#include <crtk_lib_cpp/crtk_robot_state.h>

#ifndef _STATE_TESTS_
#define _STATE_TESTS_

// main testing loop for all test units
int state_testing(CRTK_robot_state, time_t);

// I.    {disabled, ~homed} + enable [prompt for button press] → {enabled / init}
int test_1(CRTK_robot_state, time_t);

// II.    {disabled, homed} + enable [prompt for button press] → {enabled / p_dn}
// IV-2.  {enabled, busy} + pause → {paused / p_up}
int test_2(CRTK_robot_state, time_t);

// VI-2.    {paused, p_up} + disable → {disabled / e-stop}
// VIII-2.    {disabled, homed} + unhome → {disabled, ~homed / e-stop} 
int test_3(CRTK_robot_state, time_t);

// IV-1.    {enabled, homing} + pause → {disabled / e-stop}
// VIII-1.  {disabled, ~homed} + unhome → {disabled, ~homed / e-stop}
int test_4(CRTK_robot_state, time_t);

// III-1.    {enabled, homing} + disable → {disabled / e-stop}
// III-2.    {enabled, busy} + disable → {disabled / e-stop}
int test_5(CRTK_robot_state, time_t);

// VIII-3.    {enabled, homing} + unhome → {disabled, ~homed / e-stop}
// VIII-4.    {enabled, busy} + unhome → {disabled, ~homed / e-stop}
int test_6(CRTK_robot_state, time_t);

// VIII-6.    {paused, homed} + unhome → {disabled, ~homed / e-stop}
int test_7(CRTK_robot_state, time_t);

// V-3.    {disabled, ~homed} + home [prompt for button press] → {enabled, homing / init}
// V-1.    {enabled, homed} + home [prompt for button press] → {enabled, homing / init}
// V-2.    {paused, homed} + home [prompt for button press] → {enabled, homing / init}
int test_8(CRTK_robot_state, time_t);     
#endif