#pragma once

static void LogTrace(LPCTSTR pszFormat, ...)
{
#ifdef _UNICODE
	va_list pArgs;
	wchar_t szMessageBuffer[1024] = { 0 };
	va_start(pArgs, pszFormat);
	_vsntprintf(szMessageBuffer, 1024, pszFormat, pArgs);
	va_end(pArgs);
	OutputDebugString(szMessageBuffer);
#else
	va_list pArgs;
	char szMessageBuffer[1024] = { 0 };
	va_start(pArgs, pszFormat);
	_vsntprintf(szMessageBuffer, 1024, pszFormat, pArgs);
	va_end(pArgs);
	OutputDebugString(szMessageBuffer);
#endif
}