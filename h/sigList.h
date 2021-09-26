/*
 * sigList.h
 *
 *  Created on: May 19, 2020
 *      Author: OS1
 */

#ifndef SIGLIST_H_
#define SIGLIST_H_

class SigList{
public:
	struct Elem{
			int info;
			Elem* next;
			Elem(int i, Elem *n = 0){
				info = i;
				next = n;
			}
		};

	SigList();
	~SigList();
	int get();
	void put(int i);
	int count();
private:
	int cnt;
	Elem* first, *last;
};



#endif /* SIGLIST_H_ */
