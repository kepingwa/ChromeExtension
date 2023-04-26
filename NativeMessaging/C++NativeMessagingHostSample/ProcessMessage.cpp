// ////////////////////////////////////////////////////////////////////////////
// Carestream Health RESTRICTED INFORMATION - for internal use only
// ////////////////////////////////////////////////////////////////////////////
// 
// File:    ProcessMessage.cpp
// Author:  Forrest Feng
// Date:    2016.02.18
// Remark:  Process message received from chromium with JsonCpp librar.
//
// Copyright 2016, All Rights Reserved.
// 
// ////////////////////////////////////////////////////////////////////////////

#include "Windows.h" //ExitWindowsEx
#include <string>
#include "ProcessMessage.h"
#include "json\json.h"
// Edit this file to handle more request message from chromium browser

void ProcessMessage(char* jsonMsg, std::string& resp)
{
	//Todo: parse the request and send the corresponding response
	/*
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(jsonMsg+4, root);
	if (!parsingSuccessful)
	{
		return;
	}*/

	const char* data = "{"
		"\"cardNumber\":\"4012002000060016\","
		"\"cardExpiryMonth\":\"12\","
		"\"cardExpiryYear\":\"2025\","
		"\"cardHolderName\":\"Jane Yang\","
		"\"cardHolderGivenName\":\"Jane\","
		"\"cardHolderFamilyName\":\"Yang\""
		"}\n";

	resp = data;
}

