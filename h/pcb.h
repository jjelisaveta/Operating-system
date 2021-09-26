/*
 * pcb.h
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "list.h"
#include "sigList.h"
#include "handList.h"

enum Status { CREATED, READY, BLOCKED, WAITING, FINISHED, DELETED };

class PCB {
public:
	PCB(Thread* myT = 0, StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	void setStatus(Status s);
	void startPCB();
	ID getId();
	static ID getRunningId();
	static PCB* getById(ID id);
	void waitToComplete();
	~PCB();
	Status status;

	static void wrapper();
	static volatile PCB* running;
	static List allPCBs;
	static ID sid;

	static unsigned int allSigMask;

private:
	ID id;
	unsigned sp, ss;
	unsigned bp;
	Time timeSlice;
	Time waitTime;
	List* waitForCompletion;
	unsigned* stack;
	Thread* myThread;
	Thread* myParent;


	int timeExceeded;
	KernelSem *mySem;

	int signaled;
	unsigned int mySigMask;
	SigList* sigList;
	HandList sigHandlers[16];

	friend void interrupt timer(...);
	friend class Thread;
	friend class KernelSem;
	friend class KernelEv;
	friend void checkSems();
	friend void checkSignals();
	friend void sig0();
};

#endif /* PCB_H_ */
