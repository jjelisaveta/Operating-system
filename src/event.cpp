/*
 * event.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#include "event.h"
#include "pcb.h"
#include "kev.h"
#include "helper.h"

Event::Event (IVTNo ivtNo){
	lock
	myImpl = new KernelEv((PCB*)(PCB::running), ivtNo);
	unlock
}

Event::~Event (){
	lock
	delete myImpl;
	unlock
}

void Event::wait (){
	lock
	myImpl->wait();
	unlock
}

void Event::signal(){
	lock
	myImpl->signal();
	unlock
}
