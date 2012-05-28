#ifndef HAND_MOTOR_OUTPUTS_H
#define HAND_MOTOR_OUTPUTS_H


#include "../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.h"
#include "../../Library/MotorOutput/MotorOutput.h"

bool handle_motor_outputs(MotorOutputs* motor_outputs, TimeStamp current_time, NeuralNet2MovObjHandMsg	*msgs_neural_net_2_mov_obj_hand);







#endif
