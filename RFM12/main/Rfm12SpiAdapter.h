/* 
* Rfm12SpiAdapter.h
*
* Created: 11.06.2013 01:52:18
* Author: Markus
*/


#ifndef __RFM12SPIADAPTER_H__
#define __RFM12SPIADAPTER_H__

#include "rfm12/ISpi.hpp"

class Rfm12SpiAdapter : public rfm12::ISpi
{
public:
	Rfm12SpiAdapter();
	~Rfm12SpiAdapter();
private:
	Rfm12SpiAdapter( const Rfm12SpiAdapter &c );
	Rfm12SpiAdapter& operator=( const Rfm12SpiAdapter &c );

}; //Rfm12SpiAdapter

#endif //__RFM12SPIADAPTER_H__
