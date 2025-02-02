// ////////////////////////////////////////////////////////////////////////////
// Carestream Health RESTRICTED INFORMATION - for internal use only
// ////////////////////////////////////////////////////////////////////////////
// 
// File:    ProcessMessage.h
// Author:  Forrest Feng
// Date:    2016.02.18
// Remark:  Process message received from chromium 
//
// Copyright 2016, All Rights Reserved.
// 
// ////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

// The message has the size header trimed, only contains the message boday.
void ProcessMessage(char* jsonMsg, std::string &resp);