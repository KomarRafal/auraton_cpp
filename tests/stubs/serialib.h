/*
 * serialib.hpp
 */

#ifndef SERIALIB_H
#define SERIALIB_H

#include "gmock/gmock.h"

class serialib
{
public:
	serialib() = default;
	~serialib() = default;
    MOCK_METHOD(char, openDevice, (const char *Device, const unsigned int Bauds));
    MOCK_METHOD(void, closeDevice, ());
    MOCK_METHOD(int, available, ());

    char    writeBytes  (const void *Buffer, const unsigned int NbBytes);
    int     readBytes   (void *buffer,unsigned int maxNbBytes,const unsigned int timeOut_ms=0, unsigned int sleepDuration_us=100);
    char    flushReceiver();
};

#endif
