#include "core.h"

std::vector<LPSTR> fileAccepts;
std::vector<LPSTR> fileIgnores;
std::vector<_mediaFile> mediaFiles;

// These are important
LPSTR lpszServer_RemotePath = NULL;
LPSTR lpszConfig_LocalPath = NULL;


// Function will initialize the vectors required based on the config entries.
BOOL Media::Init(LPSTR lpszCategory)
{
	INT nRet = 0;

	// Load path variables (IMPORTANT)
	if ((lpszConfig_LocalPath = (LPSTR)Config::ReadString(lpszCategory, "local")) == NULL)
		return FALSE;
	if ((lpszServer_RemotePath = (LPSTR)Config::ReadString(lpszCategory, "remote")) == NULL)
		return FALSE;

	// Ignores are not REQUIRED so this is ALLOWED to fail in the event there are no ignores
	nRet = LoadIgnores(lpszCategory);

	// Accepts are required..
	nRet = LoadAccepts(lpszCategory);
	if (nRet < 0)
		return FALSE;

	return TRUE;
}

// Function will load the accepted file extensions from configuration file
INT Media::LoadAccepts(LPSTR lpszCategory)
{
	LPSTR pch;
	LPSTR lpszConfigAccepts = NULL;	// We wont free this to avoid issues with our vector.

	if ((lpszConfigAccepts = Config::ReadString(lpszCategory, "accept")) == NULL)
		return -1;
	
	pch = strtok(lpszConfigAccepts, ",");
	while (pch != NULL)
	{
		fileAccepts.push_back(pch);
		pch = strtok(NULL, ",");
	}

	return fileAccepts.size();
}

// Function will load ignored shows/movies from the configuration file
INT Media::LoadIgnores(LPSTR lpszCategory)
{
	LPSTR pch;
	LPSTR lpszConfigIgnores = NULL;	// We wont free this either to avoid issues with our vector..

	if ((lpszConfigIgnores = Config::ReadString(lpszCategory, "ignore")) == NULL)
		return -1;

	pch = strtok(lpszConfigIgnores, ",");
	while (pch != NULL)
	{
		fileIgnores.push_back(pch);
		pch = strtok(NULL, ",");
	}

	return fileIgnores.size();
}



// Function will check whether the file should be accepted (based on fileAccepts and fileIgnores)
BOOL Media::AcceptFile(LPSTR lpszFileName)
{
	BOOL mRet = FALSE;
	DWORD dwPos = 0;

	// Check whether file should be accepted based on extension (.avi/.mp4 etc..)
	for (dwPos = 0; dwPos < fileAccepts.size(); dwPos++) 
	{
		if (StrStrIA(lpszFileName, fileAccepts.at(dwPos))) {
			mRet = TRUE;
			break;
		}
	}
	if (mRet == FALSE)
		goto finish;

	// Check whether file should be accepted based on whether it is in fileIgnores
	for (dwPos = 0; dwPos < fileIgnores.size(); dwPos++)
	{
		if (StrStrIA(lpszFileName, fileIgnores.at(dwPos))) {
			mRet = FALSE;
			break;
		}
	}

finish:
	return mRet;
}

// Function will scan lpszFolder recursively for files that are accepted!
// Init must have been called prior to calling this function....
INT Media::Search(LPSTR lpszFolder)
{
	WIN32_FIND_DATA findFileData;
	HANDLE hFindFile;
	CHAR szSearchPattern[MAX_PATH] = { 0 };
	CHAR szAltSearchPattern[MAX_PATH] = { 0 };
	CHAR szUTF8_FilePath[MAX_PATH] = { 0 };
	INT nTotalFiles = 0;
	_mediaFile tmpMedia;
	LPSTR lpszBuf = NULL;
	
	PathCombineA(szSearchPattern, lpszFolder, "*.*");
	hFindFile = FindFirstFile(szSearchPattern, &findFileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
	{

		while (FindNextFile(hFindFile, &findFileData))
		{
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Found a subdirectory, lets process it..
				// Ignore ".."
				if (!lstrcmpA(findFileData.cFileName, ".."))
					continue;

				PathCombineA(szSearchPattern, lpszFolder, findFileData.cFileName);

				nTotalFiles += Search(szSearchPattern);

			}
			else {
				// Found a file, lets process it...
				PathCombineA(szSearchPattern, lpszFolder, findFileData.cFileName);	
				// Quick hack below for the PathCombine call, to ignore any "alternate" filename that isnt atleast 3 characters long (.ext)
				// This is required so our vector isnt filled with broken .lpszAltLocalPath variables (e.g. .jpg files dont have alternative filenames)
				lpszBuf = lstrlenA(findFileData.cAlternateFileName) > 3 ? findFileData.cAlternateFileName : findFileData.cFileName;
				PathCombineA(szAltSearchPattern, lpszFolder, lpszBuf);
	

				if (AcceptFile(szSearchPattern)) 
				{
					// File was accepted (it is an allowed extension and not requested to be ignored)
					// Lets populate a _mediaFile struct for the file and add it to our mediaFiles vector
					// We will use ANSItoUTF8 so that some files can be correctly located in our FTP->ShouldUpload func
					tmpMedia.lpszFileName = (LPSTR)Mem::Alloc(lstrlenA(findFileData.cFileName) + 1);
					ANSIToUTF8(findFileData.cFileName, tmpMedia.lpszFileName);

					tmpMedia.lpszLocalPath = (LPSTR)Mem::Alloc(lstrlenA(szSearchPattern) + 1);
					lstrcpyA(tmpMedia.lpszLocalPath, szSearchPattern);
					tmpMedia.lpszAltLocalPath = (LPSTR)Mem::Alloc(lstrlenA(szAltSearchPattern) + 1);
					lstrcpyA(tmpMedia.lpszAltLocalPath, szAltSearchPattern);

					lstrcpyA(szUTF8_FilePath, "");
					ANSIToUTF8(szSearchPattern, szUTF8_FilePath);

					tmpMedia.lpszRemotePath = RemoteFilePath(szUTF8_FilePath);
					tmpMedia.lpszRemoteFolder = RemoteFileFolder(tmpMedia.lpszRemotePath);
					tmpMedia.dwFileSize = findFileData.nFileSizeHigh <= 0 ? findFileData.nFileSizeLow : findFileData.nFileSizeHigh;

					mediaFiles.push_back(tmpMedia);

					nTotalFiles++;
					printf("Accepted: %s\n", findFileData.cFileName);

				} else
					printf("Denied: %s\n", findFileData.cFileName);

			}
		}

		FindClose(hFindFile);
	}

	return nTotalFiles;
}

// Function will generate a remote file path from local file path 
// e.g. C:\Media\TV\Bones\Season 01\Bones - 1x01 - Episode Name.mp4 - becomes - /Public/Shared/TV/Bones/Season 01/Bones - 1x01 - Episode Name.mp4
LPSTR Media::RemoteFilePath(LPSTR lpszLocalFilePath)
{
	LPSTR pch = NULL, lpszBuf = NULL;
	CHAR  szTmp[MAX_PATH] = { 0 };
	INT nPos = 0;

	lstrcpyA(szTmp, lpszLocalFilePath);

	pch = strstr(szTmp, lpszConfig_LocalPath);
	pch += lstrlenA(lpszConfig_LocalPath);

	for (nPos = 0; nPos < lstrlenA(pch); nPos++)
		if (pch[nPos] == '\\')
			pch[nPos] = '/';

	if ((lpszBuf = (LPSTR)Mem::Alloc(lstrlenA(lpszServer_RemotePath) + lstrlenA(pch) + 1)) == NULL)
		return NULL;

	lstrcpyA(lpszBuf, lpszServer_RemotePath);
	lstrcatA(lpszBuf, pch);

	return lpszBuf;
}

// Function will grab the remote folder path from the remote file path.
// e.g. /Public/Shared/TV/Bones/Season 01/Bones - 1x01 - Episode Name.mp4 - becomes - /Public/Shared/TV/Bones/Season 01
LPSTR Media::RemoteFileFolder(LPSTR lpszRemoteFilePath)
{
	LPSTR pch = NULL, lpszBuf = NULL;
	CHAR  szTmp[MAX_PATH] = { 0 };

	if (lpszRemoteFilePath == NULL)
		return NULL;

	lstrcpyA(szTmp, lpszRemoteFilePath);

	pch = strrchr(szTmp, '/');
	pch[0] = '\0';

	if ((lpszBuf = (LPSTR)Mem::Alloc(lstrlenA(szTmp) + 1)) == NULL)
		return NULL;

	lstrcpyA(lpszBuf, szTmp);

	return lpszBuf;
}