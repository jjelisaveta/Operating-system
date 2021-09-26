/*
 * ksem.h
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_

#include "semaphor.h"
#include "SCHEDULE.H"
#include "list.h"
#include "helper.h"
#include "pcb.h"

class KernelSem {
public:
	KernelSem(Semaphore *myS, int init = 1);
	~KernelSem();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n = 0);
	int val() const;

	static List allSems;
	List* semWait;
private:
	int value;
	Semaphore *mySem;

	friend class Semaphore;
	friend void checkSems();
	friend class Znak;
};

#endif /* KSEM_H_ */
