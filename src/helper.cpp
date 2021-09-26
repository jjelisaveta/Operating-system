/*
 * helper.cpp
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */


#include "helper.h"
#include <stdio.h>
#include "list.h"
#include "useless.h"
#include "ksem.h"
#include "sigList.h"
#include "handList.h"
#include <dos.h>

void dispatch();
unsigned volatile tss, tsp, tbp;
extern int contextSwitchDemand = 0;
extern volatile unsigned cnt;
extern pInterrupt oldTimer;
extern Thread* useless;

List KernelSem::allSems;

int per = 0;

void saveTimer() {
	lock
	#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(8);
	setvect(0x60, oldTimer);
	setvect(8, timer);
	#endif
	unlock
}

void restoreTimer() {
	lock
	#ifndef BCC_BLOCK_IGNORE
	setvect(8, oldTimer);
	#endif
	unlock
}

void interrupt timer(...) {

	if (!contextSwitchDemand && sLockFlag==0){
		tick();
		cnt--;
		checkSems();
	}

		if (((cnt == 0 && PCB::running->timeSlice!=-2) || contextSwitchDemand) && sLockFlag == 0 ) {
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;


			if (PCB::running->status == READY && PCB::getRunningId()!=((Useless*)useless)->getId())
				Scheduler::put((PCB*)PCB::running);;


			PCB::running = Scheduler::get();
			if (useless!=0 && PCB::running == 0)
				PCB::running = ((Useless*)useless)->getPCB();

			cnt = PCB::running->timeSlice;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			checkSignals();
			if (PCB::running->status==DELETED) {
				PCB::running->status = FINISHED;
				//delete PCB::running->myThread;
				per = 1;
				if (PCB::running->myParent) PCB::running->myParent->signal(1);
				PCB* cur = PCB::running->waitForCompletion->removeFirst();
				while (cur) {
					cur->status =READY;
					Scheduler::put(cur);
					cur = PCB::running->waitForCompletion->removeFirst();
				}

				PCB::running = Scheduler::get();
				if (useless!=0 && PCB::running == 0)
					PCB::running = ((Useless*)useless)->getPCB();

				cnt = PCB::running->timeSlice;
				tsp = PCB::running->sp;
				tss = PCB::running->ss;
				tbp = PCB::running->bp;
				asm {
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
				}
			}
			contextSwitchDemand = 0;
		}

	if (!contextSwitchDemand)
		asm int 0x60;
}


void checkSems(){
	lock
	List::Elem* cur =KernelSem::allSems.first;
	List::Elem* prev = 0;
	while (cur){
		PCB* curPCB = cur->info;
		if (curPCB->waitTime!=900){
			if (curPCB->waitTime-- == 0) {

				if (prev == 0)
					KernelSem::allSems.first = KernelSem::allSems.first->next;
				else
					prev->next = cur->next;
				if (KernelSem::allSems.last == cur)
					KernelSem::allSems.last = prev;
				KernelSem::allSems.cnt--;
				List::Elem *old = cur;
				cur = cur->next;
				old->next = 0;

				curPCB->mySem->semWait->removeById(curPCB->id);
				curPCB->mySem->value++;
				curPCB->mySem = 0;
				curPCB->timeExceeded = 0;   //isteklo vreme
				curPCB->status = READY;
				Scheduler::put(curPCB);

			} else{
				prev = cur;
				cur = cur->next;
			}
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
	unlock
}

void checkSignals(){
	sLock

	if (PCB::running->signaled == 1) {
		HandList* hList;
		int sig = PCB::running->sigList->get();
		int blocked = 0;
		SigList * newSigList = new SigList();

		while (PCB::running->sigList->count()>=0 && sig!=-1) {
			if (((PCB::allSigMask & (1<<sig)) != 0) || ((PCB::running->mySigMask & (1<<sig))!=0) ) //blokiran
				newSigList->put(sig);
			else {
				hList = (HandList*)(&PCB::running->sigHandlers[sig]);
				HandList::Elem* hCur = hList->first;
				while (hCur){
					(*(hCur->info))();
					hCur = hCur->next;
				}
				if (sig == 0) break;   //posle brisanja se nista ne radi
			}
			sig = PCB::running->sigList->get();
		}
		delete PCB::running->sigList;
		PCB::running->sigList = newSigList;
		PCB::running->signaled=0;
	}

	sUnlock
}


void sig0(){
	PCB::running->status = DELETED;
}





