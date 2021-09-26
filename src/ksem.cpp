/*
 * ksem.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#include "ksem.h"

List allSems = List();
extern int lockFlag, lockDemand, in;

KernelSem::KernelSem(Semaphore *myS, int init) {
	lock
	mySem = myS;
	value = init;
	semWait = new List();
	unlock
}

int KernelSem::wait(Time maxTimeToWait) {
	lock
	if(--value<0){
		if (maxTimeToWait==0)
			PCB::running->waitTime = 900;
		else
			PCB::running->waitTime = maxTimeToWait;
		PCB::running->mySem = this;
		semWait->insert((PCB*)PCB::running);
		allSems.insert((PCB*)PCB::running);
		PCB::running->status = BLOCKED;

		dispatch();
		unlock
		return PCB::running->timeExceeded;
	}
	else{
		unlock
		return 1;
	}
}

int KernelSem::signal(int n) {
	lock
	int ret = n;
	if (ret<0) {
		unlock
		return ret;
		}
	else if (ret==0) {
		value++;
		if (value <= 0) {
				PCB* deblock = semWait->removeFirst();
				KernelSem::allSems.removeById(deblock->getId());
				deblock->mySem = 0;
				deblock->status=READY;
				Scheduler::put(deblock);
			}
		unlock
		return ret;
	}
	else {
		value++;
		if (value <= 0) {
			value--;
				while (n>0 && ++value<=0) { 				  //PROVERI
					PCB* deblock = semWait->removeFirst();
					KernelSem::allSems.removeById(deblock->id);
					deblock->status = READY;
					Scheduler::put(deblock);
					n--;
				}
				ret -= n;
		}
		else
			ret = 0;
		unlock
		return ret;
	}
}

int KernelSem::val() const {
	return value;
}

KernelSem::~KernelSem(){
	lock
	delete semWait;
	unlock
}
