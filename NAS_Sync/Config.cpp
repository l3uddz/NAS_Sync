#include "core.h"

LPSTR lpszConfig_Path = NULL;

LPSTR Config::GetConfigPath()
{
	LPSTR lpszConfig = NULL;
	CHAR  szModulePath[MAX_PATH] = { 0 };
	DWORD dwConfigSize = 0;

	if ((dwConfigSize = GetCurrentDirectoryA(sizeof(szModulePath) - 1, szModulePath)) <= 0)
		return NULL;

	dwConfigSize += 13;
	if ((lpszConfig = (LPSTR)Mem::Alloc(dwConfigSize)) == NULL)
		return NULL;

	lstrcpyA(lpszConfig, szModulePath);
	lstrcatA(lpszConfig, "\\config.ini");

	return lpszConfig;
}

LPSTR Config::ReadString(LPSTR lpszSection, LPSTR lpszSetting)
{
	LPSTR lpszRet = NULL;
	CHAR szTmp[512] = { 0 };
	DWORD dwTmp = 0;

	if (lpszConfig_Path == NULL && (lpszConfig_Path = GetConfigPath()) == NULL) {
		printf("Error locating configuration file...\n");
		return NULL;
	}

	if ((dwTmp = GetPrivateProfileStringA(lpszSection, lpszSetting, NULL, szTmp, sizeof(szTmp) - 1, lpszConfig_Path)) <= 0) {
//		printf("Error reading setting from configuration file... (%s\\%s)\n", lpszSection, lpszSetting);
		return NULL;
	}

	if ((lpszRet = (LPSTR)Mem::Alloc(dwTmp)) == NULL)
		return NULL;

	lstrcpyA(lpszRet, szTmp);
	
	return lpszRet;
}

DWORD Config::ReadNumber(LPSTR lpszSection, LPSTR lpszSetting)
{
	DWORD dwRet = 0;

	if (lpszConfig_Path == NULL && (lpszConfig_Path = GetConfigPath()) == NULL) {
		printf("Error locating configuration file...\n");
		return NULL;
	}

	if ((dwRet = (DWORD)GetPrivateProfileIntA(lpszSection, lpszSetting, 0, lpszConfig_Path)) <= 0) {
		printf("Error reading setting from configuration file... (%s\\%s)\n", lpszSection, lpszSetting);
	}

	return dwRet;
}