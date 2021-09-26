#include <stdio.h>
#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "thread.h"
#include "pcb.h"
#include "list.h"
#include "useless.h"
#include "helper.h"
#include "semaphor.h"
#include "event.h"
#include "kev.h"
#include "ivtentry.h"
#include <stdlib.h>
#include "bounded.h"
#include "intLock.h"
#include "keyevent.h"
#include "user.h"

volatile unsigned cnt = defaultTimeSlice;
int sLockFlag = 0;
pInterrupt oldTimer;
Thread* useless = 0;


extern int userMain (int argc, char* argv[]);

class UserThread: public Thread {
public:
	UserThread(int a, char** ar, StackSize stackSize = defaultStackSize,
		Time timeSlice = defaultTimeSlice) :	Thread(stackSize, timeSlice) {
			argc = a;
			argv = ar;
	}
	void run() {
		userMain(argc, argv);
	}

private:
	int argc;
	char** argv;

};

void main(int argc, char *argv[]) {

	saveTimer();

	lock
	PCB::running = new PCB(0);
	((PCB*)PCB::running)->setStatus(READY);
	Thread *u = new UserThread(argc, argv);
	unlock


	u->start();


	lock
	useless= new Useless();
	((Useless*)useless)->getPCB()->setStatus(READY);
	unlock


	u->waitToComplete();


	restoreTimer();

}



