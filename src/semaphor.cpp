/*
 * semaphor.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#include "semaphor.h"
#include "ksem.h"
#include "helper.h"

Semaphore::Semaphore(int init) {
	lock
	myImpl = new KernelSem(this, init);
	unlock
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->val();
}


Semaphore::~Semaphore() {
	delete myImpl;
}


