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
    MOCK_METHOD(void, flushReceiver, ());
    MOCK_METHOD(char, writeBytes, (const void *Buffer, const unsigned int NbBytes));
    MOCK_METHOD(int, readBytes,   (void *buffer, unsigned int maxNbBytes, const unsigned int timeOut_ms, unsigned int sleepDuration_us));
};

#endif
