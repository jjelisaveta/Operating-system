/*
 * event.h
 *
 *  Created on: May 12, 2020
 *      Author: OS1
 */

#ifndef _event_h_
#define _event_h_
typedef unsigned char IVTNo;
class KernelEv;

class Event
{
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};


#define PREPAREENTRY(no, callOld)\
void interrupt inter##no(...); \
IVTEntry newEntry##no(no, inter##no); \
void interrupt inter##no(...) {\
	newEntry##no.signal();\
	if (callOld == 1)\
		newEntry##no.oldR();\
}

#endif
