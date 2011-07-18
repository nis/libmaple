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

#ifndef _IRLIB_H_
#define _IRLIB_H_

/***************************** Include files *******************************/

#include "wirish.h"

/*****************************    Defines    *******************************/

// IR commands
#define IRLIB_NO_COMMAND		0 	// No valid command received
// Apple remote commands
#define IRLIB_APPLE_UP			1
#define IRLIB_APPLE_DOWN		2
#define IRLIB_APPLE_LEFT		3
#define IRLIB_APPLE_RIGHT		4
#define IRLIB_APPLE_CENTER		5
#define IRLIB_APPLE_MENU		6
#define IRLIB_APPLE_PLAY		7

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************     Class     *******************************/

class IRLib {
	public:
		void irlib_init(uint8 ir_input_pin); 		// Setup.
		void irlib_interrupt_handler(void); 		// Statemachine for handling incomming data.
		
		uint8 irlib_decode_apple(void);				// Decodes and returns the last received command.
													// Does not delete command if the command is being repeated.
													
		
	private:
		uint8 raw_data_ready(void);					// Have we finished receiving a command?
		uint16 command_repeating(void);				// Returns the number if times the current command has been repeated?
		
};

/****************************** End Of Module *******************************/

#endif