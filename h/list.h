/*
 * pcb_list.h
 *
 *  Created on: Apr 23, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "thread.h"

class PCB;

class List{
public:
	struct Elem{
		PCB* info;
		Elem* next;
		Elem(PCB *i, Elem *n = 0){
			info = i;
			next = n;
		}
	};

	List();
	void insert(PCB* newPCB);
	PCB* removeById(ID id);
	PCB* removeFirst();
	PCB* getById(ID id);
	int empty();
	~List();

	Elem *first;
	Elem *last;
	int cnt;

private:
	void deleteList();

	friend class PCB;
	friend class KernelSem;
};


#endif /* LIST_H_ */
