/*
 * system.h
 *
 * Created: 14.12.2012 00:47:49
 *  Author: sunside
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "util.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#pragma message("F_CPU previously undefined, now set to " VALUE(F_CPU))
#endif

#endif /* SYSTEM_H_ */