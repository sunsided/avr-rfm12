/*
 * util.h
 *
 * Created: 12.12.2012 22:50:06
 *  Author: sunside
 */ 

#ifndef UTIL_H_
#define UTIL_H_

//!\def SET_BIT Sets a bit
#define SET_BIT(TARGET, BIT) TARGET |= (1 << BIT)

//!\def CLEAR_BIT Sets a bit
#define CLEAR_BIT(TARGET, BIT) TARGET &= ~(1 << BIT)

//! \def SUCCESS The return value for success
#define SUCCESS	0

//! \def VALUE_TO_STRING Helper macro to print the content of a define
#define VALUE_TO_STRING(x) #x

//! \def VALUE Macro to print the content of a define
#define VALUE(x) VALUE_TO_STRING(x)

//! \def VAR_NAME_VALUE macro to print the name and content of a define
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#endif /* UTIL_H_ */