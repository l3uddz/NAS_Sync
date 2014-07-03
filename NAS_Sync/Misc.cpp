#include "core.h"

int ANSIToUTF8(char *pszCode, char *UTF8code)
{
	WCHAR Unicode[MAX_PATH * sizeof(WCHAR)] = { 0 };
	char utf8[MAX_PATH] = { 0 };

	int nUnicodeSize = MultiByteToWideChar(CP_ACP, 0, pszCode, lstrlenA(pszCode), Unicode, sizeof(Unicode));
	int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, Unicode, -1, UTF8code, sizeof(Unicode), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, utf8, nUTF8codeSize, Unicode, sizeof(Unicode));

	return nUTF8codeSize;
}
