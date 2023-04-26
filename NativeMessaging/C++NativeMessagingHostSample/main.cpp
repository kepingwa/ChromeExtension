// ////////////////////////////////////////////////////////////////////////////
// Carestream Health RESTRICTED INFORMATION - for internal use only
// ////////////////////////////////////////////////////////////////////////////
// 
// File:    native.cpp
// Author:  Forrest Feng
// Date:    2016.02.18
// Remark:  This console program is used to process message sent from Chromium 
//          plugin. 
//          Example of useage:
//          1. User click logoff button or a menu on the web page.
//          2. The click handler function send a logoff request message to a 
//             native executable via chrome.runtime.sendNativeMessage
//             NOTE : If the executable is not running chromium will start it 
//             automatically.
//          3. The client receives this message and call windows API to logoff current user.
//
// Copyright 2016 All Rights Reserved.
// 
// ////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <io.h>
#include "ProcessMessage.h"

// Define union to read the message size easily
typedef union {
	unsigned long u32;
	unsigned char u8[4];
} U32_U8;

// On Windows, the default I / O mode is O_TEXT.Set this to O_BINARY
// to avoid unwanted modifications of the input / output streams.
int SetBinaryMode(FILE* file)
{
	return 0;
	int result;

	result = _setmode(_fileno(file), _O_BINARY);
	if (result == -1)
	{
		perror("Cannot set mode");
		return result;
	}
	// set do not use buffer
	result = setvbuf(file, NULL, _IONBF, 0);	
	if (result != 0)
	{
		perror("Cannot set zero buffer");
		return result;
	}

	return 0;
}

#define DO_LOG(_MESSAGE) fwrite(_MESSAGE, 1, strlen(_MESSAGE), log);
// main logic
int _tmain(int argc, _TCHAR* argv[])
{
	const char* started = "Started\n";
	FILE* log = fopen("c:\\tmp\\native.txt", "w");
	fwrite(started, 1, strlen(started), log);

#if 0
	if (SetBinaryMode(stdin) != 0)
	{
		const char* started = "Bad1\n";
		DO_LOG("Bad1\n")
		fclose(log);
		return -1;
	}
	if (SetBinaryMode(stdout) != 0)
	{
		DO_LOG("Bad2\n")
		fclose(log);
		return -1;
	}
#endif
	fclose(log);

	// The format of raw message received from the stdin
	// IIIISSSSS...SS
	// IIII is a 4bytes integer in binary format. It is the lenth of the following json message.
	// SSSSS...SS is the json message immidiatlly follows the 4 bytes header. 
	// Each message is serialized using JSON, UTF-8 encoded and is preceded with 32-bit message length in native byte order.
	// You can send message back to chromium plugin by writing the same formated IIIISSSSS...SS to stdout.

	while (1) {
		log = fopen("c:\\tmp\\native.txt", "a+");

		size_t iSize = 0;
		U32_U8 lenBuf;
		lenBuf.u32 = 0;
		char* jsonMsg = NULL; // a json message encoded in utf-8 
		iSize = fread(lenBuf.u8, 1, 4, stdin);
		if (iSize == 4)
		{
			DO_LOG("Read size completed\n")
				int iLen = (int)lenBuf.u32;
			// now read the message
			if (iLen > 0)
			{
				DO_LOG("Length is valid\n")
					jsonMsg = (char*)malloc(8 * iLen);
				iSize = fread(jsonMsg, 1, iLen, stdin);
				char buf[256];
				sprintf(buf, "Len: %d\n", iSize);
				DO_LOG(buf)
					jsonMsg[iSize] = '\0';
				DO_LOG(jsonMsg)
					DO_LOG("\n")

				// process message
				std::string resp;
				ProcessMessage(jsonMsg, resp);

				// echo the message
				lenBuf.u32 = resp.length();
				fwrite(lenBuf.u8, 1, 4, stdout);
				fwrite(resp.c_str(), 1, resp.length(), stdout);
				fflush(stdout);
				Sleep(100);
				sprintf(buf, "Response (%d): %s\n", lenBuf.u32, resp);
				DO_LOG(buf)

				/*
				const char* data = "{"
					"\"cardNumber\":\"4012002000060016\","
					"\"cardExpiryMonth\":\"12\","
					"\"cardExpiryYear\":\"2025\","
					"\"cardHolderName\":\"Jane Yang\","
					"\"cardHolderGivenName\":\"Jane\","
					"\"cardHolderFamilyName\":\"Yang\""
					"}\n";
				lenBuf.u32 = strlen(data);
				fwrite(lenBuf.u8, 1, 4, stdout);
				fwrite(data, 1, lenBuf.u32, stdout);
				fflush(stdout);
				Sleep(1000);
				fflush(stdout);
				Sleep(1000);
				fflush(stdout);
				*/
			}

			//uncomment it to debug the messaging 
			/*FILE* log = fopen("D:\\native.txt", "w"); */

			// free resource
			if (jsonMsg != NULL)
				free(jsonMsg);

			fclose(log);
		}
		break;
	}	

	return 0;
}

