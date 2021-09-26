/*
 * useless.h
 *
 *  Created on: Apr 27, 2020
 *      Author: OS1
 */

#ifndef USELESS_H_
#define USELESS_H_

#include "thread.h"
#include "pcb.h"

class Useless: public Thread{
public:
	Useless(StackSize stackSize = defaultStackSize, Time timeSlice = 1);
	virtual void run();
	PCB* Useless::getPCB();

	friend void interrupt timer(...);
	friend class PCB;
};


#endif /* USELESS_H_ */
