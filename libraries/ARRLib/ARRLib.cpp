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

/***************************** Include files *******************************/

#include "ARRLib.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
	
volatile uint8 my_debug = 0;
uint8 ir_input_pin;
ir_sequence last_command;
byte new_command = 0;
		
/*****************************   Functions   *******************************/

void ARRLib::ir_debug(void) {
	//Serial1.println(my_debug, DEC);
	if(new_command)
	{	
		Serial1.print("Sequence: [");
		Serial1.print(last_command.raw_pointer, DEC);
		Serial1.print("] ");
		Serial1.print(last_command.decoded, DEC);
		Serial1.print(" Data: ");
		Serial1.print(last_command.data[0], HEX);
		Serial1.print(" ");
		Serial1.print(last_command.data[1], HEX);
		Serial1.print(" ");
		Serial1.print(last_command.data[2], HEX);
		Serial1.print(" ");
		Serial1.println(last_command.data[3], HEX);
		// for(uint8 i = 0; i < last_command.raw_pointer; ++i)
		// {
		// 	Serial1.print("\t");
		// 	Serial1.println(last_command.raw[i], DEC);
		// }
		new_command = 0;
	}
}

ARRLib::ARRLib(uint8 input_pin) {
    pinMode(input_pin, INPUT);
	ir_input_pin = input_pin;
	
	HardwareTimer timer(1);
	
	timer.pause();
	timer.setPrescaleFactor(72);
	timer.setOverflow(ARRLIB_U_SEC_PER_TICK);
	timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
	timer.setCompare(TIMER_CH1, 1);
	timer.attachCompare1Interrupt(arrlib_interrupt_handler);
	timer.refresh();
	timer.resume();
}

void arrlib_interrupt_handler(void) {
	static byte state = ARRLIB_STATE_IDLE;
	static uint32 counter = 0;
	static ir_sequence seq;
	
	byte ir_signal = digitalRead(ir_input_pin);
	
	counter++;
	
	if(seq.raw_pointer >= ARRLIB_IR_RAW_BUFFER_LENGTH)
	{
		state = ARRLIB_STATE_IDLE;
	}
	
	switch (state) {
		case ARRLIB_STATE_IDLE:
			if(ir_signal == ARRLIB_IR_MARK)
			{
				if(counter < ARRLIB_GAP_TICKS)
				{
					counter = 0;
				} else {
					seq.raw_pointer = 0;
					seq.decoded = 0;
					//seq.marks[seq.pointer++] = counter;
					counter = 0;
					state = ARRLIB_STATE_COUNT_MARK;
				}
			}
		break;
		
		case ARRLIB_STATE_COUNT_MARK:
			if(ir_signal == ARRLIB_IR_NOT_MARK)
			{
				seq.raw[seq.raw_pointer++] = counter;
				counter = 0;
				state = ARRLIB_STATE_COUNT_NOT_MARK;
			}
		break;

 		case ARRLIB_STATE_COUNT_NOT_MARK:
 			if(ir_signal == ARRLIB_IR_MARK)
 			{
 				seq.raw[seq.raw_pointer++] = counter;
 				counter = 0;
 				state = ARRLIB_STATE_COUNT_MARK;
 			} else {
				if(counter > ARRLIB_GAP_TICKS)
				{
					state = ARRLIB_STATE_COMMAND_END;
				}
			}
 		break;

		case ARRLIB_STATE_COMMAND_END:
			if(seq.raw_pointer == 67)
			{
				if(
						(seq.raw[0] >= ARRLIB_START_H_MIN && seq.raw[0] <= ARRLIB_START_H_MAX) &&
						(seq.raw[1] >= ARRLIB_START_L_MIN && seq.raw[1] <= ARRLIB_START_L_MAX)
				  )
				{
					// First byte
					for(uint8 i = 17; i > 2; i = i - 2)
					{
						if(seq.raw[i] >= ARRLIB_ONE_L_MIN && seq.raw[i] <= ARRLIB_ONE_L_MAX)
						{
							seq.data[0] = (seq.data[0] << 1) | 1;
						} else if(seq.raw[i] >= ARRLIB_ZERO_L_MIN && seq.raw[i] <= ARRLIB_ZERO_L_MAX)
						{
							seq.data[0] <<= 1;
						}
					}
					
					// Second byte
					for(uint8 i = 33; i > 18; i = i - 2)
					{
						if(seq.raw[i] >= ARRLIB_ONE_L_MIN && seq.raw[i] <= ARRLIB_ONE_L_MAX)
						{
							seq.data[1] = (seq.data[1] << 1) | 1;
						} else if(seq.raw[i] >= ARRLIB_ZERO_L_MIN && seq.raw[i] <= ARRLIB_ZERO_L_MAX)
						{
							seq.data[1] <<= 1;
						}
					}
					
					// Third byte
					for(uint8 i = 49; i > 34; i = i - 2)
					{
						if(seq.raw[i] >= ARRLIB_ONE_L_MIN && seq.raw[i] <= ARRLIB_ONE_L_MAX)
						{
							seq.data[2] = (seq.data[2] << 1) | 1;
						} else if(seq.raw[i] >= ARRLIB_ZERO_L_MIN && seq.raw[i] <= ARRLIB_ZERO_L_MAX)
						{
							seq.data[2] <<= 1;
						}
					}
					
					// Fourth byte
					for(uint8 i = 65; i > 50; i = i - 2)
					{
						if(seq.raw[i] >= ARRLIB_ONE_L_MIN && seq.raw[i] <= ARRLIB_ONE_L_MAX)
						{
							seq.data[3] = (seq.data[3] << 1) | 1;
						} else if(seq.raw[i] >= ARRLIB_ZERO_L_MIN && seq.raw[i] <= ARRLIB_ZERO_L_MAX)
						{
							seq.data[3] <<= 1;
						}
					}
				} else {
					seq.decoded = 0;
				}
				
				
				
				last_command = seq;
				new_command = 1;
			}
			state = ARRLIB_STATE_IDLE;
		break;
	}
}

/****************************** End Of Module *******************************/