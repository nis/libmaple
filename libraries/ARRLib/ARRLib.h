/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 Nis Sarup
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#ifndef _ARRLIB_H_
#define _ARRLIB_H_

/***************************** Include files *******************************/

#include "wirish.h"

/*****************************    Defines    *******************************/

// IR commands
#define ARRLIB_NO_COMMAND					0 	// No valid command received
// Apple remote commands        			
#define ARRLIB_APPLE_UP						1
#define ARRLIB_APPLE_DOWN					2
#define ARRLIB_APPLE_LEFT					3
#define ARRLIB_APPLE_RIGHT					4
#define ARRLIB_APPLE_CENTER					5
#define ARRLIB_APPLE_MENU					6
#define ARRLIB_APPLE_PLAY					7
                                	
#define ARRLIB_U_SEC_PER_TICK 				100	// us per interrupt tick.
#define ARRLIB_MAX_COUNTER					10000/ARRLIB_U_SEC_PER_TICK // The maximum value for the timer
#define ARRLIB_GAP_US						5000
#define ARRLIB_GAP_TICKS					ARRLIB_GAP_US/ARRLIB_U_SEC_PER_TICK

// IR ISR States
#define ARRLIB_STATE_IDLE					0
#define ARRLIB_STATE_COUNT_MARK				1
#define ARRLIB_STATE_COUNT_NOT_MARK			2
#define ARRLIB_STATE_COMMAND_END			3

// IR Sensor
#define ARRLIB_IR_MARK						0
#define ARRLIB_IR_NOT_MARK					1
#define ARRLIB_IR_RAW_BUFFER_LENGTH			80

// us times for the NEC IR protocol
#define ARRLIB_START_H						9000
#define ARRLIB_START_L						4500
#define ARRLIB_BIT_H						560
#define ARRLIB_ONE_L						1687
#define ARRLIB_ZERO_L						560
#define ARRLIB_REPEAT_L						2250

// Pulse length converted to ticks
#define ARRLIB_START_H_MIN 		(uint8)(ARRLIB_START_H / ARRLIB_U_SEC_PER_TICK) - 3
#define ARRLIB_START_H_MAX 		(uint8)(ARRLIB_START_H / ARRLIB_U_SEC_PER_TICK) + 3
#define ARRLIB_START_L_MIN 		(uint8)(ARRLIB_START_L / ARRLIB_U_SEC_PER_TICK) - 3
#define ARRLIB_START_L_MAX 		(uint8)(ARRLIB_START_L / ARRLIB_U_SEC_PER_TICK) + 3
#define ARRLIB_BIT_H_MIN 		(uint8)(ARRLIB_BIT_H / ARRLIB_U_SEC_PER_TICK) - 2
#define ARRLIB_BIT_H_MAX 		(uint8)(ARRLIB_BIT_H / ARRLIB_U_SEC_PER_TICK) + 2
#define ARRLIB_ONE_L_MIN 		(uint8)(ARRLIB_ONE_L / ARRLIB_U_SEC_PER_TICK) - 3
#define ARRLIB_ONE_L_MAX 		(uint8)(ARRLIB_ONE_L / ARRLIB_U_SEC_PER_TICK) + 3
#define ARRLIB_ZERO_L_MIN		(uint8)(ARRLIB_ZERO_L / ARRLIB_U_SEC_PER_TICK) - 2
#define ARRLIB_ZERO_L_MAX 		(uint8)(ARRLIB_ZERO_L / ARRLIB_U_SEC_PER_TICK) + 2
#define ARRLIB_REPEAT_L_MIN 	(uint8)(ARRLIB_REPEAT_L / ARRLIB_U_SEC_PER_TICK) - 3
#define ARRLIB_REPEAT_L_MAX 	(uint8)(ARRLIB_REPEAT_L / ARRLIB_U_SEC_PER_TICK) + 3



/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

struct ir_sequence {
	uint16 raw[80];
	uint8 raw_pointer;
};

/*****************************     Class     *******************************/

void arrlib_interrupt_handler(void);

class ARRLib {
	public:
		
		ARRLib(uint8 input_pin); 		// Setup.
		void ir_debug(void);
		//uint8 irlib_decode_apple(void);				// Decodes and returns the last received command.
													// Does not delete command if the command is being repeated.
													
		
	private:
		uint8 raw_data_ready(void);					// Have we finished receiving a command?
		uint16 command_repeating(void);				// Returns the number if times the current command has been repeated?
		
};

/****************************** End Of Module *******************************/

#endif