/*
 * sigList.cpp
 *
 *  Created on: May 19, 2020
 *      Author: OS1
 */

#include "sigList.h"
#include "helper.h"
#include <iostream.h>


SigList::SigList(){
	cnt = 0;
	first = last = 0;
}

SigList::~SigList(){
	Elem *old;
	while (first!=0){
		old = first;
		first = first->next;
		delete old;
	}
	first = last = 0;
	cnt = 0;
}

int SigList::get(){
	int num = -1;
	if (count()==0)
		return num;
	Elem *ret = first;
	first = first->next;
	ret->next = 0;
	num = ret->info;
	delete ret;
	cnt--;
	return num;
}

void SigList::put(int i){
	lock
	Elem *newElem = new Elem(i);
	if (first == 0)
		first = newElem;
	else
		last->next = newElem;
	last = newElem;
	cnt++;
	unlock
}



int SigList::count(){
	return cnt;
}
