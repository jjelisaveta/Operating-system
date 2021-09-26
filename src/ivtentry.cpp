/*
 * IVTEntry.cpp
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#include "ivtentry.h"
#include "kev.h"
#include "helper.h"
#include <dos.h>

IVTEntry* IVTEntry::IVTable[256];

IVTEntry::IVTEntry(IVTNo n, pInterrupt newR){
	lock
	num = n;
	newRout = newR;
	initEntry();
	IVTable[num]=this;
	myKernelEv = 0;
	unlock
}

IVTEntry::~IVTEntry(){
	restoreEntry();
}

void IVTEntry::signal(){
	lock
	if (myKernelEv==0) {
		unlock
		return;
	}
	myKernelEv->signal();
	unlock
}

void IVTEntry::oldR(){
	oldRout();
}

void IVTEntry::initEntry(){
	lock
	oldRout = getvect(num);
	setvect(num, newRout);
	unlock
}

void IVTEntry::restoreEntry(){
	lock
	setvect((int)num, oldRout);
	unlock
}





