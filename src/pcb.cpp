/*
 * pcb.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */

#include "pcb.h"
#include <dos.h>
#include <stdio.h>
#include "SCHEDULE.H"
#include "thread.h"
#include "list.h"
#include "helper.h"

ID PCB::sid = 0;
volatile PCB* PCB::running = 0;
List PCB::allPCBs = List();
unsigned int PCB::allSigMask = 0;

extern Thread* useless;
extern int contextSwitchDemand;


PCB::PCB(Thread* myT, StackSize stackSize, Time timeSlice) {
	lock
	id = sid++;
	myThread = myT;

	if (stackSize < 16)
		stackSize = defaultStackSize;
	stack = new unsigned[stackSize];

	if (timeSlice == 0)
		this->timeSlice = -2;    //za beskonacno izvrsavanje
	else
		this->timeSlice = timeSlice;

	status = CREATED;
	timeExceeded = 1;
	PCB::allPCBs.insert(this);
	waitForCompletion = new List();

	sigList = new SigList;
	signaled = 0;
	mySigMask = 0;
	/*if (id!=0)*/ myParent = PCB::running->myThread;
	sigHandlers[0].put(sig0);

#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 1] = FP_SEG(myThread);
	stack[stackSize - 2] = FP_OFF(myThread);
	stack[stackSize - 5] = 0x200; 				//PSW sa I=1
	stack[stackSize - 6] = FP_SEG(wrapper);
	stack[stackSize - 7] = FP_OFF(wrapper);
	ss = FP_SEG(stack + stackSize - 16);
	sp = FP_OFF(stack + stackSize - 16);
	bp = sp;
#endif
	unlock
}

void PCB::setStatus(Status s) {
	status = s;
}

void PCB::startPCB() {
	lock
	if (status == CREATED) {
		status = READY;
		Scheduler::put(this);
	}
	unlock
}

ID PCB::getId() {
	return id;
}

ID PCB::getRunningId() {
	return ((PCB*) running)->getId();
}

PCB* PCB::getById(ID id) {
	return PCB::allPCBs.getById(id);
}

void PCB::waitToComplete() {
	lock
	if (this->status==FINISHED || this->id == PCB::running->id || this->id == useless->myPCB->id) {
		unlock
		return;
	}

	PCB::running->status = BLOCKED;
	waitForCompletion->insert((PCB*)PCB::running);
	dispatch();

	unlock
}

void PCB::wrapper() {
	((Thread*) PCB::running->myThread)->run();
	//nit je zavrsena

	lock
	per = 3;
	if (PCB::running->myParent) PCB::running->myParent->signal(1);
	PCB::running->myThread->signal(2);

	PCB* cur = PCB::running->waitForCompletion->removeFirst();
	while (cur) {
		cur->status =READY;
		Scheduler::put(cur);
		cur = PCB::running->waitForCompletion->removeFirst();
	}
	PCB::running->status = FINISHED;
	unlock

	dispatch();
}

PCB::~PCB() {
	allPCBs.removeById(this->getId());
	lock
	delete[] stack;
	unlock
}

void dispatch() {
	contextSwitchDemand = 1;
	asm int 8;
	contextSwitchDemand = 0;
}

