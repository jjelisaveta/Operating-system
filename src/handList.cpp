/*
 * handList.cpp
 *
 *  Created on: May 19, 2020
 *      Author: OS1
 */

#include "handList.h"
#include "helper.h"
#include "pcb.h"


HandList::HandList(){
	cnt = 0;
	first = last = 0;
}

HandList::~HandList(){
	deleteList();
}

SignalHandler HandList::get(){
	if (count()==0)
		return 0;
	Elem *ret = first;
	first = first->next;
	ret->next = 0;
	cnt--;
	return ret->info;
}

HandList::Elem* HandList::get(SignalHandler sh) {
	Elem *cur;
	for(cur = first; cur!=0; cur=cur->next){
		if (cur->info == sh)
			return cur;
	}
	return 0;

}

void HandList::put(SignalHandler sh){
	lock
	Elem *newElem = new Elem(sh);
	unlock
	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	cnt++;
}


int HandList::count(){
	return cnt;
}

void HandList::deleteList(){
	lock
	Elem *old;
	while (first!=0) {
		old = first;
		first = first->next;
		delete old;
	}
	first = last = 0;
	cnt = 0;
	unlock
}



