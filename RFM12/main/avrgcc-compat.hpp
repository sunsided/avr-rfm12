/*
 * avrgcc_compat.hpp
 *
 * Created: 14.06.2013 15:56:16
 *  Author: Markus
 */ 


#ifndef AVRGCCCOMPAT_H_
#define AVRGCCCOMPAT_H_

#include <stdlib.h>

void * operator new(size_t size);
void * operator new[](size_t size);

void operator delete(void * ptr);
void operator delete[](void * ptr);

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

extern "C" void __cxa_pure_virtual(void); 

#endif /* AVRGCCCOMPAT_H_ */