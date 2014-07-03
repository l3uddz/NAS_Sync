#include "core.h"

// This function will connect to the specified FTP server
BOOL FTP::Connect(LPSTR lpszServer, DWORD dwPort, LPSTR lpszUsername, LPSTR lpszPassword)
{
	BOOL mRet = FALSE;
	DWORD dwError = 0, dwErrorSize = 0;
	CHAR szErrorMsg[512] = { 0 };
	DWORD dwContext = 1;

	dwErrorSize = sizeof(szErrorMsg) - 1;

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		dwError = GetLastError();
		InternetGetLastResponseInfoA(&dwError, szErrorMsg, &dwErrorSize);
		printf("Error InternetOpen, Code: %d Info:\n%s\n", dwError, szErrorMsg);
		goto finish;
	}

	hSession = InternetConnectA(hInternet,lpszServer, (INTERNET_PORT)dwPort, lpszUsername, lpszPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, dwContext);
	if (hSession == NULL) {
		dwError = GetLastError();
		InternetGetLastResponseInfoA(&dwError, szErrorMsg, &dwErrorSize);
		printf("Error InternetConnect, Code: %d Info:\n%s\n", dwError, szErrorMsg);
		goto finish;
	}
	
	printf("Connected to %s:%d as %s\n", lpszServer, dwPort, lpszUsername);

	mRet = TRUE;

finish:
	if (mRet == FALSE && hInternet != NULL)
		InternetCloseHandle(hInternet);
	if (mRet == FALSE && hSession != NULL)
		InternetCloseHandle(hSession);

	return mRet;
}

// This function will UPLOAD the specified file, first it will attempt to create the directory pointed to
// by lpszRemoteFolder - it will then attempt to upload the file (lpszLocalFile -> lpszRemoteFile)
BOOL FTP::TransferFile(LPSTR lpszLocalFile, LPSTR lpszRemoteFile, LPSTR lpszRemoteFolder)
{
	BOOL mRet = FALSE;
	DWORD dwError = 0, dwErrorSize = 0;
	CHAR szErrorMsg[512] = { 0 };

	dwErrorSize = sizeof(szErrorMsg) - 1;

	mRet = this->CreateDirectory(lpszRemoteFolder);
	if (mRet == FALSE) goto finish;

	mRet = FtpPutFileA(hSession, lpszLocalFile, lpszRemoteFile, FTP_TRANSFER_TYPE_BINARY, 0);
	if (mRet == FALSE) {
		dwError = GetLastError();
		InternetGetLastResponseInfoA(&dwError, szErrorMsg, &dwErrorSize);
		printf("ERROR\nFtpPutFile, Code: %d Info:\n%s\n", dwError, szErrorMsg);
		goto finish;
	}
		

	mRet = TRUE;
	
finish:
	return mRet;
}

// This function will create a directory(s) specified in the lpszRemoteFolder variable
// It will create multiple directories if required (to create the lpszRemoteFolder)
BOOL FTP::CreateDirectory(LPSTR lpszRemoteFolder)
{
	BOOL mRet = FALSE;
	LPSTR pch;
	CHAR szTmp[512] = { 0 };
	DWORD dwError = 0, dwErrorSize = 0;
	CHAR szErrorMsg[512] = { 0 };

	dwErrorSize = sizeof(szErrorMsg) - 1;
	lstrcpyA(szTmp, lpszRemoteFolder);

	FtpSetCurrentDirectoryA(hSession, "/");

	pch = strtok(szTmp, "/");
	while (pch != NULL)
	{
		mRet = FtpSetCurrentDirectoryA(hSession, pch);
		if (mRet == FALSE)
		{
			// Lets try to create the directory...
			if (FtpCreateDirectoryA(hSession, pch) == FALSE) {
				dwError = GetLastError();
				InternetGetLastResponseInfoA(&dwError, szErrorMsg, &dwErrorSize);
				printf("ERROR\nFtpCreateDirectory, Code: %d Info:\n%s\n", dwError, szErrorMsg);
				goto finish;

			}
			else {
				if ((mRet = FtpSetCurrentDirectoryA(hSession, pch)) == FALSE) {
					dwError = GetLastError();
					InternetGetLastResponseInfoA(&dwError, szErrorMsg, &dwErrorSize);
					printf("ERROR\nFtpSetCurrentDirectory, Code: %d Info:\n%s\n", dwError, szErrorMsg);
					goto finish;
				}
			}

		}

		pch = strtok(NULL, "/");
	}

	mRet = TRUE;

finish:
	return mRet;
}

// Function will return the file size of the remote file
// this function often fails with non standard ASCII characters in the filepath..
DWORD FTP::FileSize(LPSTR lpszRemoteFilePath)
{
	DWORD dwRet = 0;
	HINTERNET hFile;
	
	if ((hFile = FtpOpenFileA(hSession, lpszRemoteFilePath, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY, 0)) == NULL)
		return dwRet;

	dwRet = FtpGetFileSize(hFile, NULL);

	if (hFile) 
		InternetCloseHandle(hFile);

	return dwRet;
}

// Function will check a mediaFile passed to it from our vector of media files, 
// It will check as to whether the file exists on the FTP server already, if it does it will compare the file sizes...
BOOL FTP::ShouldUploadFile(_mediaFile media)
{
	BOOL mRet = FALSE;
	HINTERNET hDirectory = NULL;
	WIN32_FIND_DATA findFileData;
	DWORD dwFileSize = 0;

	// If the remote folder does not exist, then the file should be uploaded
	if (FtpSetCurrentDirectoryA(hSession, media.lpszRemoteFolder) == FALSE)
		return TRUE;

	// Lets process the folder to locate our mediaFile if possible, otherwise it has to be uploaded
	if ((hDirectory = FtpFindFirstFileA(hSession, NULL, &findFileData, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0)) == NULL)
		return TRUE;	// Folder is empty, obviously our file needs to be uploaded.

	// File check code
	do 
	{
		if (!StrNCmpI(findFileData.cFileName, media.lpszFileName, lstrlenA(media.lpszFileName)))
		{
			dwFileSize = findFileData.nFileSizeHigh > 0 ? findFileData.nFileSizeHigh : findFileData.nFileSizeLow;

			if (dwFileSize == media.dwFileSize)
			{
				mRet = FALSE;
				goto finish;
			} else 
			{
				mRet = TRUE;
				goto finish;
			}

		}

	} while (InternetFindNextFileA(hDirectory, &findFileData));

	mRet = TRUE;

finish:

	if (hDirectory) 
		InternetCloseHandle(hDirectory);

	return mRet;
}
