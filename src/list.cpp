/*
 * list.cpp
 *
 *  Created on: Apr 23, 2020
 *      Author: OS1
 */

#include "list.h"
#include "pcb.h"
#include <iostream.h>
#include "thread.h"
#include "helper.h"


List::List() {
	lock
	cnt = 0;
	first = last = 0;
	unlock
}

void List::insert(PCB* newPCB) {    //ubacuje na kraj liste, radi
	lock
	Elem *newElem = new Elem(newPCB);

	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	cnt++;
	unlock
}


PCB* List::removeById(ID id) {	//radi
	lock
	Elem *cur, *prev = 0;
	Elem* old = 0;
	PCB* oldPCB;
	for (cur = first; cur!=0;cur = cur->next){
		if (cur->info->getId()==id){
			old = cur;
			cnt--;
			if (prev==0)
				first = first->next;
			else
				prev->next = cur->next;
			if (last == cur)
				last = prev;
			oldPCB = old->info;
			old->next = 0;
			delete old;
			if (cnt==0) last = first = 0;
			unlock
			return oldPCB;
		}
		prev = cur;
	}
	unlock
	return 0;
}

PCB* List::removeFirst() {
	lock
	PCB* oldPCB = 0;
	Elem* old = first;
	first = first->next;
	if (old==last) last = 0;
	if (old){
		cnt--;
		oldPCB = old->info;
		old->next = 0;
		delete old;
		unlock
		return oldPCB;
	}
	else {

		unlock
		return 0;
	}
}

int List::empty(){
	if (cnt==0)
		return 1;
	else
		return 0;
}

PCB* List::getById(ID id) {    //radi
	if (id<0 || id>PCB::sid) return 0;
	Elem *cur = first;
	for(cur = first; cur!=0; cur = cur->next){
		if (cur->info->getId() == id){
			return cur->info;
		}
	}
	return 0;
}

void List::deleteList() {
	Elem *old;
	while (first != 0) {
		old = first;
		first = first->next;
		lock
		delete old;
		unlock
	}
	cnt = 0;
}


List::~List() {
	deleteList();
}



