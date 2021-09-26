/*
 * kev.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#include "kev.h"
#include "event.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "helper.h"
#include "ivtentry.h"


KernelEv::KernelEv(PCB* o, IVTNo ivtNo){
	lock
	owner = o;
	this->ivtNo = ivtNo;
	blocked = 0;
	value = 1;
	//if (IVTEntry::IVTable==0) cout << "nemaaaa" << endl;
	IVTEntry::IVTable[(int)ivtNo]->myKernelEv=this;
	unlock
}

KernelEv::~KernelEv(){
	owner = blocked = 0;
}

void KernelEv::wait(){
	lock
	if (PCB::running->getId() == owner->getId()){
		if (value==0){
			blocked = (PCB*)PCB::running;
			blocked->status = BLOCKED;
			dispatch();
		}
		else
			value = 0;
	}
	unlock
}


void KernelEv::signal(){
	lock
	//cout << "signal";
	if (blocked==0)
		value++;
	else {
		blocked->status = READY;
		Scheduler::put(blocked);
		blocked = 0;
	}
	unlock
}

