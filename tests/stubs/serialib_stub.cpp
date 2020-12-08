/*
 * serialib_stub.cpp
 */

#include "serialib.h"

char serialib::writeBytes([[maybe_unused]] const void *Buffer, [[maybe_unused]] const unsigned int NbBytes)
{
    return 1;
}

int serialib::readBytes ([[maybe_unused]] void *buffer,
		[[maybe_unused]] unsigned int maxNbBytes,
		[[maybe_unused]] unsigned int timeOut_ms,
		[[maybe_unused]] unsigned int sleepDuration_us)
{
	// TODO
	return 10;
}

char serialib::flushReceiver()
{
    return true;
}
