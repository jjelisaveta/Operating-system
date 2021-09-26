/*
 * IVTEntry.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "kev.h"
#include "helper.h"

class IVTEntry {
public:
	IVTEntry(IVTNo n, pInterrupt newR);
	~IVTEntry();

	void signal();
	void oldR();

private:
	pInterrupt oldRout;
	pInterrupt newRout;
	IVTNo num;
	KernelEv* myKernelEv;

	static IVTEntry* IVTable[256];
	void initEntry();
	void restoreEntry();

	friend class KernelEv;
};





#endif /* IVTENTRY_H_ */
