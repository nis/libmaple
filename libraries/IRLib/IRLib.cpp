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

#include "IRLib.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
	
volatile uint8 my_debug = 0;
uint8 ir_input_pin;
ir_sequence last_command;
byte new_command = 0;
		
/*****************************   Functions   *******************************/

void IRLib::ir_debug(void) {
	//Serial1.println(my_debug, DEC);
	if(new_command)
	{	
		Serial1.println("Sequence: ");
		for(uint8 i = 0; i < last_command.pointer; ++i)
		{
			Serial1.print("\t");
			Serial1.println(last_command.marks[i], DEC);
		}
		new_command = 0;
	}
}

IRLib::IRLib(uint8 input_pin) {
    pinMode(input_pin, INPUT);
	ir_input_pin = input_pin;
	
	HardwareTimer timer(1);
	
	timer.pause();
	timer.setPrescaleFactor(72);
	timer.setOverflow(U_SEC_PER_TICK);
	timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
	timer.setCompare(TIMER_CH1, 1);
	timer.attachCompare1Interrupt(irlib_interrupt_handler);
	timer.refresh();
	timer.resume();
}

void irlib_interrupt_handler(void) {
	static byte state = IRLIB_STATE_IDLE;
	static uint16 counter = 0;
	static ir_sequence seq;
	
	byte ir_signal = digitalRead(ir_input_pin);
	
	switch (state) {
		case IRLIB_STATE_IDLE:
			if(ir_signal == IR_MARK)
			{
				counter = 0;
				ir_sequence new_seq;
				seq = new_seq;
				state = IRLIB_STATE_COUNT_MARK;
			}
		break;
		
		case IRLIB_STATE_COUNT_MARK:
			counter++;
			if(ir_signal == IR_NOT_MARK)
			{
				seq.marks[seq.pointer] = counter;
				seq.pointer++;
				counter = 0;
				state = IRLIB_STATE_COUNT_NOT_MARK;
			}
		break;
		
		case IRLIB_STATE_COUNT_NOT_MARK:
			counter++;
			if(counter > 200)
			{
				state = IRLIB_STATE_COMMAND_END;
			} else if(ir_signal == IR_MARK) 
			{
				seq.marks[seq.pointer] = counter;
				seq.pointer++;
				counter = 0;
				state = IRLIB_STATE_COUNT_MARK;
			}
		break;
		
		case IRLIB_STATE_COMMAND_END:
			last_command = seq;
			new_command = 1;
			state = IRLIB_STATE_IDLE;
		break;
	}
}

/****************************** End Of Module *******************************/