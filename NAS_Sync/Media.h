// Structs
struct _mediaFile
{
	LPSTR lpszFileName;
	LPSTR lpszLocalPath;
	LPSTR lpszAltLocalPath;
	LPSTR lpszRemotePath;
	LPSTR lpszRemoteFolder;
	DWORD dwFileSize;
};

// Declares
namespace Media
{
	BOOL Init(LPSTR lpszCategory);
	INT LoadAccepts(LPSTR lpszCategory);
	INT LoadIgnores(LPSTR lpszCategory);
	BOOL AcceptFile(LPSTR lpszFileName);
	INT Search(LPSTR lpszFolder);
	LPSTR RemoteFilePath(LPSTR lpszLocalFilePath);
	LPSTR RemoteFileFolder(LPSTR lpszRemoteFilePath);
}

// Shared variables
extern std::vector<_mediaFile> mediaFiles;