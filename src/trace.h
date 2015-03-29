//-----------------------------------------------------------------------------
// File: trace.h
// Desc: Simple debug logging.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#pragma once

#ifdef _DEBUG

//--------------------------------------------------------------------------------------
// Debug logging functions
// Description: Contains debug logging functions.
//
//     Initialize: Opens a logging file with the specified file name.
//     Trace: Writes a sprintf-formatted string to the logging file.
//     Close: Closes the logging file and reports any memory leaks.
//
// The TRACE_INIT, TRACE, and TRACE_CLOSE macros are mapped to the logging functions
// in debug builds, and defined as nothing in retail builds.
//--------------------------------------------------------------------------------------

HRESULT DebugLog_Initialize(const WCHAR *sFileName);
void    DebugLog_Trace(const WCHAR *sFormatString, ...);
void    DebugLog_Close();


#define TRACE_INIT(x) DebugLog_Initialize(x)
#define TRACE(x) DebugLog_Trace x
#define TRACE_CLOSE() DebugLog_Close()

#else

#define TRACE_INIT(x) 
#define TRACE(x) 
#define TRACE_CLOSE() 

#endif

