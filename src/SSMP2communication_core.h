/*
 * SSMP2communication_core.h - Core functions (services) of the new SSM-protocol
 *
 * Copyright (C) 2008-2009 Comer352l
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SSMP2COMMUNICATIONCORE_H
#define SSMP2COMMUNICATIONCORE_H


#ifdef __WIN32__
    #include "windows\serialCOM.h"
    #define waitms(x) Sleep(x);
#elif defined __linux__
    #include <unistd.h>
    #include "linux/serialCOM.h"
    #define waitms(x) usleep(1000*x);
#else
    #error "Operating system not supported !"
#endif
#ifdef __FSSM_DEBUG__
    #include <iostream>
    #include "libFSSM.h"
#endif



class SSMP2communication_core
{

public:
	SSMP2communication_core(serialCOM *port);

	bool ReadDataBlock(char ecuaddr, char padaddr, unsigned int dataaddr, unsigned char nrofbytes, char *data);
	bool ReadMultipleDatabytes(char ecuaddr, char padaddr, unsigned int dataaddr[256], unsigned char datalen, char *data);
	bool WriteDataBlock(char ecuaddr, unsigned int dataaddr, char *data, unsigned char datalen, char *datawritten = NULL);
	bool WriteDatabyte(char ecuaddr, unsigned int dataaddr, char databyte, char *databytewritten = NULL);
	bool GetCUdata(char ecuaddr, char *SYS_ID, char *ROM_ID, char *flagbytes, unsigned char *nrofflagbytes);

private:
	bool SndRcvMessage(char ecuaddr, char *outdata, unsigned char outdatalen, char *indata, unsigned char *indatalen);
	char calcchecksum(char *message, unsigned int nrofbytes);
	void charcat(char *chararray_a, char *chararray_b, unsigned int len_a, unsigned int len_b);
	bool charcmp(char *chararray_a, char *chararray_b, unsigned int len);

protected:
	serialCOM *_port;

};



#endif
