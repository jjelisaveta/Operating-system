/*
 * helper.h
 *
 *  Created on: Apr 30, 2020
 *      Author: OS1
 */

#ifndef HELPER_H_
#define HELPER_H_


typedef void interrupt (*pInterrupt)(...);
void saveTimer();
void restoreTimer();
void interrupt timer(...);
void checkSems();
void checkSignals();
void tick();

void sig0();
extern int sLockFlag;
extern int per;


#define sLock sLockFlag = 1;

#define sUnlock sLockFlag = 0;



#define lock asm {\
pushf;\
cli;\
}

#define unlock asm popf



#endif /* HELPER_H_ */


