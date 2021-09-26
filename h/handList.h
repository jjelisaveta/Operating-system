/*
 * handList.h
 *
 *  Created on: May 19, 2020
 *      Author: OS1
 */

#ifndef HANDLIST_H_
#define HANDLIST_H_

#include "thread.h"

class HandList{
public:
	struct Elem{
		SignalHandler info;
		Elem* next;
		Elem(SignalHandler i, Elem *n = 0){
			info = i;
			next = n;
		}
	};

	HandList();
	~HandList();
	SignalHandler get();
	Elem* get(SignalHandler sh);
	void put(SignalHandler sh);
	int count();
	void deleteList();
private:
	int cnt;
	Elem* first, *last;

	friend class Thread;
	friend void checkSignals();
};


#endif /* HANDLIST_H_ */
