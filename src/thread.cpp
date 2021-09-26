/*
 * thread.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */
#include "thread.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "list.h"
#include "helper.h"
#include "sigList.h"
#include "handList.h"


Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(this, stackSize, timeSlice);
	unlock
}

void Thread::start() {
	lock
	myPCB->startPCB();
	unlock
}


void Thread::waitToComplete() {
	lock
	myPCB->waitToComplete();
	unlock
}

Thread::~Thread() {
	waitToComplete();
	lock
	delete myPCB;
	unlock
}

ID Thread::getId() {
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	PCB* found = PCB::getById(id);
	if (found)
		return found->myThread;
	else
		return 0;
}

void Thread::signal(SignalId signal){
	sLock
	if (signal < 0 || signal>15 || (signal==1 && (per!=1 || per!=3)) || (signal==2 && (per!=2 || per!=3))) {
		per = 0;
		sUnlock
		return;
	}
	myPCB->signaled = 1;
	myPCB->sigList->put(signal);
	//cout << "signalizirano " << (int)signal<< endl;
	sUnlock
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	if (signal>=0 && signal<16 && handler!=0){
		myPCB->sigHandlers[signal].put(handler);
	}
}

void Thread::unregisterAllHandlers(SignalId id){
	if (id>=0 && id<16){
		myPCB->sigHandlers[id].deleteList();
	}
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	HandList::Elem *h1 = 0, *h2 = 0;
	SignalHandler tmp;
	h1 = myPCB->sigHandlers[id].get(hand1);

	h2 =  myPCB->sigHandlers[id].get(hand2);
	if (h1!=0 && h2!=0){
		tmp = h1->info;
		h1->info = h2->info;
		h2->info = tmp;
	}
}

void Thread::blockSignal(SignalId signal){
	myPCB->mySigMask = myPCB->mySigMask | 1<<signal;
}

void Thread::blockSignalGlobally(SignalId signal){
	PCB::allSigMask = PCB::allSigMask | 1<<signal;
}

void Thread::unblockSignal(SignalId signal){
	myPCB->mySigMask = myPCB->mySigMask & ~(1<<signal);
}

void Thread::unblockSignalGlobally(SignalId signal){
	PCB::allSigMask = PCB::allSigMask & ~(1<<signal);
}



