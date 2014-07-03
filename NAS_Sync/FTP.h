
class FTP
{
public:
	BOOL Connect(LPSTR lpszServer, DWORD dwPort, LPSTR lpszUsername, LPSTR lpszPassword);
	BOOL TransferFile(LPSTR lpszLocalFile, LPSTR lpszRemoteFile, LPSTR lpszRemoteFolder);
	BOOL CreateDirectory(LPSTR lpszRemoteFolder);
	DWORD FileSize(LPSTR lpszRemoteFilePath);
	BOOL ShouldUploadFile(_mediaFile media);
private:
	HINTERNET hInternet;
	HINTERNET hSession;
};