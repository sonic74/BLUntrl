//-----------------------------------------------------------------------------
// File: trace.cpp
// Desc: Simple debug logging.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
//  Copyright (C) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include <windows.h>
#include "trace.h"

#include <strsafe.h>
#include <crtdbg.h>


//////////////////

#ifdef _DEBUG

// Debug logging

// Log file
HANDLE g_hFile = INVALID_HANDLE_VALUE;

//--------------------------------------------------------------------------------------
// Name: Initialize
// Description: Opens a logging file with the specified file name.
//--------------------------------------------------------------------------------------

HRESULT DebugLog_Initialize(const WCHAR *sFileName)
{
    // Close any existing file.
    if (g_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(g_hFile);
        g_hFile = INVALID_HANDLE_VALUE;
    }

    // Open the new logging file.

	HRESULT hr = S_OK;
	if (sFileName)
	{
		g_hFile = CreateFile(
			sFileName, 
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			0, 
			NULL);

		if (g_hFile == INVALID_HANDLE_VALUE)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}
		else
		{
			// Redirect debug messages to the logging file.
			_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
			_CrtSetReportFile(_CRT_WARN, g_hFile);
		}
	}
	else
	{
		// No file name, send debug messages to the debug window.
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// Name: Trace
// Description: Writes a sprintf-formatted string to the logging file.
//--------------------------------------------------------------------------------------

void DebugLog_Trace(const WCHAR *sFormatString, ...)
{
    HRESULT hr = S_OK;
    va_list va;

    const DWORD TRACE_STRING_LEN = 512;

    WCHAR message[TRACE_STRING_LEN];

    va_start(va, sFormatString);
    hr = StringCchVPrintf(message, TRACE_STRING_LEN, sFormatString, va);
    va_end(va);

    if (SUCCEEDED(hr))
    {
        _CrtDbgReport(_CRT_WARN, NULL, NULL, NULL, "%S\r\n", message);

    }
}

//--------------------------------------------------------------------------------------
// Name: Close
// Description: Closes the logging file and reports any memory leaks.
//--------------------------------------------------------------------------------------

void DebugLog_Close()
{
    _CrtDumpMemoryLeaks();
	CloseHandle(g_hFile);
}

#endif // _DEBUG