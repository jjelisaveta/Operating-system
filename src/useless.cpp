/*
 * useless.cpp
 *
 *  Created on: Apr 27, 2020
 *      Author: OS1
 */


#include "useless.h"
#include "helper.h"
#include <iostream.h>


Useless::Useless(StackSize stackSize, Time timeSlice) : Thread(stackSize, timeSlice){}

void Useless::run(){
	while (1) {

	}
}

PCB* Useless::getPCB() {
		return PCB::getById(this->getId());
}
