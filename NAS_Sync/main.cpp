#include "core.h"
#include <atlstr.h>

LPSTR lpszSearch_Folder = NULL;
LPSTR lpszServer_Addr = NULL;
LPSTR lpszServer_User = NULL;
LPSTR lpszServer_Pass = NULL;
DWORD dwServer_Port = 0;

INT nFilesUploaded = 0;
INT nFilesSkipped = 0;


int main()
{
	FTP client;
	CHAR szCategory[MAX_PATH] = { 0 };

	// Load configuration settings
	lpszServer_Addr = Config::ReadString("ftp", "addr");
	lpszServer_User = Config::ReadString("ftp", "user");
	lpszServer_Pass = Config::ReadString("ftp", "pass");
	dwServer_Port =	  Config::ReadNumber("ftp", "port");
	if (lpszServer_Addr == NULL || lpszServer_User == NULL || lpszServer_Pass == NULL || dwServer_Port == 0) 
	{
		printf("Please correct your configuration file and try again!\n");
		goto finish;
	}

	printf("Server: %s\n", lpszServer_Addr);
	printf("Port: %d\n", dwServer_Port);
	printf("User: %s\n", lpszServer_User);
	printf("Pass: %s\n\n", lpszServer_Pass);

	printf("Please enter the category to use: ");
	scanf("%s", &szCategory);

	BOOL mInit = Media::Init(szCategory);

	if (mInit == FALSE) 
	{
		printf("Error initializing media, please check your configuration file...\n");
		goto finish;
	}
	lpszSearch_Folder = Config::ReadString(szCategory, "local");

	printf("Scanning for media in: \"%s\"\n\n", lpszSearch_Folder);
	int nTotal = Media::Search(lpszSearch_Folder);
	printf("\nTotal files accepted: %d\n\n", nTotal);

	if (client.Connect(lpszServer_Addr, dwServer_Port, lpszServer_User, lpszServer_Pass))
	{
	
		BOOL mUploaded = FALSE, mShouldUpload = FALSE;

		for (int x = 0; x < (int)mediaFiles.size(); x++)
		{
			mShouldUpload = client.ShouldUploadFile(mediaFiles[x]);
			if (mShouldUpload == TRUE) 
			{
				printf("Uploading: %s... ", mediaFiles[x].lpszFileName);
		
				mUploaded = client.TransferFile(mediaFiles[x].lpszAltLocalPath, mediaFiles[x].lpszRemotePath, mediaFiles[x].lpszRemoteFolder);
				if (mUploaded == TRUE)
				{
					printf("SUCCESS!\n");
					nFilesUploaded++;
				}

			}
			else
				nFilesSkipped++;

		}
	}
		
	printf("\nSkipped %d files\nUploaded %d files!\n", nFilesSkipped, nFilesUploaded);

finish:

	if (lpszServer_Addr) Mem::Free(lpszServer_Addr);
	if (lpszServer_User) Mem::Free(lpszServer_User);
	if (lpszServer_Pass) Mem::Free(lpszServer_Pass);
	if (lpszSearch_Folder) Mem::Free(lpszSearch_Folder);

	system("pause");
	return 0;
}

