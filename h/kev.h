/*
 * kev.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef KEV_H_
#define KEV_H_

#include "event.h"
#include "pcb.h"

class KernelEv {
public:
	KernelEv(PCB* o, IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

private:
	int value;
	PCB* owner;
	PCB* blocked;
	IVTNo ivtNo;
};



#endif /* KEV_H_ */
