#include "stdafx.h"
#include "PcShare.h"
#include "Lzw.h"
#include "md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BYTE3INT(X)  (    ( X[0] & 0x000000FF ) \
                      | ( ( X[1] & 0x000000FF ) <<  8 ) \
                      | ( ( X[2] & 0x000000FF ) << 16 )  )

#define BYTE4INT(X)  (    ( X[0] & 0x000000FF ) \
                      | ( ( X[1] & 0x000000FF ) <<  8 ) \
                      | ( ( X[2] & 0x000000FF ) << 16 ) \
                      | ( ( X[3] & 0x000000FF ) << 24 )  )

struct INDEXITEM 
{
    BYTE ip[4];
    BYTE offset[3];
};

TCHAR m_IniFile[256] = {0};

struct INDEXITEM LookForIndexItem(struct INDEXITEM* const pAimItem, 
								  FILE* pFile, 
								  unsigned int indexBasePos, 
								  unsigned int indexAmount );

int Compare(BYTE pA[4], BYTE pB[4]);

void RetrieveContent(struct INDEXITEM* const pIndexItem, BYTE ip[4], FILE* pFile,  char* content);


int LocalityFromIP2(BYTE* IP, TCHAR* pFileName, TCHAR* pResult)
{
	char m_FileName[256] = {0};
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0,	pFileName, lstrlen(pFileName) + 1, 
		m_FileName, 256, NULL, NULL);
#else
	lstrcpy(m_FileName, pFileName);
#endif

 	FILE * ipfp = fopen(m_FileName, "rb");
	if(ipfp == NULL)
	{
		return -1;
	}

	char pbuf[256] = {0};

	DWORD indexHeadPos = 0;
	DWORD indexTailPos = 0;

	struct INDEXITEM target;
	CopyMemory(target.ip, IP, 4);

	target.ip[0] = 5;
	target.ip[1] = IP[2];
	target.ip[2] = IP[1];
	target.ip[3] = IP[0];

	fread(&indexHeadPos, sizeof(indexHeadPos), 1, ipfp);
	fread(&indexTailPos, sizeof(indexTailPos), 1, ipfp);

	int amount = (indexTailPos - indexHeadPos)/sizeof(struct INDEXITEM);
	struct INDEXITEM start = LookForIndexItem(&target, ipfp, indexHeadPos, amount);

	char result[255] = {0};
	RetrieveContent(&start, target.ip, ipfp, result);
	fclose(ipfp);

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, result, lstrlenA(result) + 1, pResult, 256);
#else
	lstrcpy(pResult, result);
#endif
    return 0;
}

struct INDEXITEM LookForIndexItem(struct INDEXITEM* const pAimItem, 
								  FILE* pFile, 
								  unsigned int indexBasePos, 
								  unsigned int indexAmount )
{
    struct INDEXITEM tmp;
    int i = 0;
    int j = indexAmount;
    int s = (int)sizeof(struct INDEXITEM);
    while ( i < j - 1 ) 
	{
        int k = (int) (i+j)/2;
        int offset = (int)( k * s );
        fseek(pFile, indexBasePos+offset, SEEK_SET);
        fread(&tmp, s, 1, pFile);
        int c = Compare( tmp.ip, pAimItem->ip );
        if ( c > 0 )
		{
            j = k;
		}
        else if ( c < 0 )
		{
            i = k;
		}
        else 
		{
            i = k;
            j = k;
        }
    }

    fseek(pFile, indexBasePos+i*s, SEEK_SET);
    fread(&tmp, s, 1, pFile);
    return tmp;
}

int Compare(unsigned char pA[4], unsigned char pB[4])
{
    unsigned int a = BYTE4INT(pA);
    unsigned int b = BYTE4INT(pB);

    if ( a > b )
	{
        return 1;
	}
    else if ( a < b )
	{
        return -1;
	}
    else
	{
        return 0;
	}
}

void GetData(unsigned char* str, FILE* pFile, int max)
{
    int i = 0;
    while ( (*(str+i)=fgetc(pFile)) && (i<(max-2)) )
	{
        i++;
	}
    str[i] = 0;
}

void RetrieveContent(struct INDEXITEM* const pIndexItem, 
					 unsigned char ip[4],
					 FILE* pFile, 
					 char* content )
{
    // to get the pos from the offset array
    long tmp = 0;
    unsigned char buf[80];

    int pos = BYTE3INT(pIndexItem->offset);
    fseek(pFile, pos, SEEK_SET);
    fread(buf, 4, 1, pFile);
    if (Compare(ip, buf) > 0 ) 
	{
        lstrcatA(content, "未知");
        //printf("未知数据\n");
        return;
    }

    // 获取资料
    fread(buf, 1, 1, pFile);
    if ( buf[0] == 0x01 ) 
	{ // 国家地区均重复, 跳转至新地址
        fread(buf, 3, 1, pFile);
        pos = BYTE3INT(buf);
        fseek(pFile, pos, SEEK_SET);
        fread(buf, 1, 1, pFile);
    }

    // 获取国家
    if ( buf[0] == 0x02 ) 
	{
        // 获取国家偏移
        fread(buf, 3, 1, pFile);
        // 保存地区信息
        tmp = ftell(pFile);
        pos = BYTE3INT(buf);
        fseek(pFile, pos, SEEK_SET);
        fread(buf, 1, 1, pFile);
    }
    if ( buf[0] == 0x01 || buf[0] == 0x02 ) 
	{
        lstrcatA(content, "未知");
        //printf("There is something wrong....\n");
        return;
    }

    if ( buf[0] )
	{
        GetData(buf+1, pFile, 40);
	}
    lstrcatA(content, (char*)buf);
    //printf("%s\t", buf);

    // 获取地区
    if ( tmp )
	{
        fseek(pFile, tmp, SEEK_SET);
	}
    fread(buf, 1, 1, pFile);
    while ( buf[0] == 0x02 ) 
	{
        // 获取地区偏移
        fread(buf, 3, 1, pFile);
        pos = BYTE3INT(buf);
        fseek(pFile, pos, SEEK_SET);
        fread(buf, 1, 1, pFile);
    }
    if ( buf[0] == 0x01 || buf[0] == 0x02 ) 
	{
        lstrcatA(content, "未知");
        //printf("There is something wrong....\n");
        return;
    }
    if ( buf[0] )
	{
        GetData(buf+1, pFile, 40);
	}
      lstrcatA(content, (char*) buf);
    return;
}

void BcdToAsc(BYTE* BcdStr, BYTE* AscStr, int Bcdlstrlen)
{
	int i = 0, j = 0;
	for(i = 0; i < Bcdlstrlen; i++)
	{
		j = (BcdStr[i] >> 4) & 0x0f;
                
		if(j > 9)
		{
			AscStr[2 * i] = j + 'A' - 10;
		}
		else
		{
			AscStr[2 * i] = j + '0';
		}

		j = BcdStr[i] & 0x0f;

		if(j > 9)
		{
			AscStr[2 * i + 1 ] = j + 'A' - 10;
		}
		else
		{
			AscStr[2 * i + 1] = j + '0';
		}
	}
}


LPCTSTR GetIpList()
{
	static TCHAR m_wAddrList[256] = {0};

	//取IP地址列表信息
	if(lstrlen(m_wAddrList) != 0)
	{
		return m_wAddrList;
	}

	char m_AddrList[1024] = {0};

	PHOSTENT hostinfo = NULL;
	char name[512] = {0};
	if(gethostname(name, sizeof(name)) != 0 || (hostinfo = gethostbyname(name)) == NULL)
	{
		return _T("");
	}

	struct sockaddr_in dest;
	memset(m_AddrList, 0 ,sizeof(m_AddrList));
	for(DWORD k = 0; hostinfo->h_addr_list[k] != NULL ;k++)
	{
		memcpy(&(dest.sin_addr), hostinfo->h_addr_list[k], hostinfo->h_length);
		lstrcatA(m_AddrList, inet_ntoa(dest.sin_addr));
		lstrcatA(m_AddrList, "-");
	}
	m_AddrList[lstrlenA(m_AddrList) - 1] = 0;

#ifdef UNICODE
	//转成宽字节
	MultiByteToWideChar(CP_ACP, 0, m_AddrList, lstrlenA(m_AddrList) + 1, m_wAddrList, 256);
#else
	lstrcpy(m_wAddrList, m_AddrList);
#endif
	return m_wAddrList;
}

BOOL GetStrValue(LPCTSTR pStr, LPCTSTR pName, LPTSTR pValue)
{
	TCHAR m_Str[4096] = {0};
	TCHAR m_Name[1024] = {0};
	TCHAR m_Value[1024] = {0};
	wsprintf(m_Name, _T("%s="), pName);
	lstrcpy(m_Str, pStr);

	TCHAR* pStart = StrStr(m_Str, pName);
	if(pStart == NULL)
	{
		return FALSE;
	}
	pStart += lstrlen(m_Name);
	
	TCHAR* pEnd = StrChr(pStart, _T(';'));
	if(pEnd == NULL)
	{
		pEnd = m_Str + lstrlen(m_Str);
	}
	CopyMemory(pValue, pStart, (pEnd - pStart) * sizeof(TCHAR));
	pValue[pEnd - pStart] = 0;
	return TRUE;
}

BOOL MakeFilePath(TCHAR* pathName)
{
	TCHAR m_TmpStr[100] = {0};		
	TCHAR *pr = NULL;							
	TCHAR *ps = NULL;
	int  nLenth = 0;
	
	TCHAR m_Path[256] = {0};
	lstrcpy(m_Path, pathName);

	//查看是否为文件
	pr = StrChr(ps, _T('\\'));
	if(pFind != NULL)
	{
		pFind = StrRChr(m_Path, NULL, _T('\\'));
		if(pFind != NULL)
		{
			*pFind = 0;
		}
	}

	//目录已经存在
	CFileStatus m_FileStatus;
	if(CFile::GetStatus(m_Path,m_FileStatus))
	{
		return TRUE;
	}

	//初始化指针
	ps = m_Path;
	nLenth = lstrlen(m_Path);
	while(1)
	{
		pr = StrChr(ps, _T('\\'));
		if(pr == NULL)
		{
			//只有一级目录
			pr = ps + lstrlen(ps) ;
		}
		int len = pr - m_Path;
		memcpy(m_TmpStr, m_Path, len * sizeof(TCHAR));
		if(len > 3)  
		{
			//建立当前目录
			m_TmpStr[len] = _T('\0');
			if(!CFile::GetStatus(m_TmpStr, m_FileStatus))
			{
				if(!CreateDirectory(m_TmpStr, NULL))
				{
					TRACE(m_TmpStr);
					TRACE(_T("\n"));
				}
			}
		}
		ps = pr+1;
		if(ps - m_Path >= nLenth)
		{
			//目录处理完备
			break;
		}
	}

	//确定建立的目录是否存在
	return CFile::GetStatus(m_Path, m_FileStatus);
}

BOOL GetLoginInfo(TCHAR* pUrl, TCHAR* pIpStr)
{
	//初始化HTTP环境
	HINTERNET hMainIe = InternetOpen(_T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; .NET CLR 1.1.4322)"),
						INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	if(hMainIe == NULL)	
	{
		return FALSE;
	}
	
	//打开连接
	HINTERNET hMainFile = InternetOpenUrl(
			hMainIe, pUrl, NULL, 0,
			INTERNET_FLAG_PRAGMA_NOCACHE|
			INTERNET_FLAG_RELOAD|
			INTERNET_FLAG_DONT_CACHE|
			INTERNET_FLAG_HYPERLINK|
			INTERNET_FLAG_NO_COOKIES,
			NULL);
	if(hMainFile == NULL)
	{
		InternetCloseHandle(hMainIe);
		return FALSE;
	}

	//查看返回码
	TCHAR sCode[256] = {0};
	DWORD nSize = 250;
	DWORD nIndex = 0;
	if(!HttpQueryInfo(hMainFile , HTTP_QUERY_STATUS_CODE , 
		sCode , &nSize , &nIndex) || StrToInt(sCode) != 200)
	{
		InternetCloseHandle(hMainFile);
		InternetCloseHandle(hMainIe);
		return FALSE;
	}

	//接收数据
	char m_Response[65535] = {0};
	for(int i = 0; i < 65535; i++)
	{
		char ch = 0;
		DWORD nRead = 0;
		if(!InternetReadFile(hMainFile, &ch, 1, &nRead) || nRead == 0)
		{
			break;
		}
		if(ch == 0x0d || ch == 0x0a)
		{
			ch = 0x00;
			break;
		}
		m_Response[i] = ch;
	}

	InternetCloseHandle(hMainFile);
	InternetCloseHandle(hMainIe);

#ifdef UNICODE
	MultiByteToWideChar(CP_ACP, 0, m_Response, lstrlenA(m_Response) + 1, pIpStr, 256);
#else
	lstrcpy(pIpStr, m_Response);
#endif
	return TRUE;
}

void GetTmpFilePath(TCHAR* FileName)
{
	TCHAR m_Path[512] = {0};
	GetModuleFileName(NULL, m_Path, 500);
	TCHAR* pFind = StrRChr(m_Path, NULL, _T('\\'));
	if(pFind != NULL)
	{
		*(pFind + 1) = 0;
		lstrcat(m_Path, FileName);
		lstrcpy(FileName, m_Path);
	}
}

void GetBinFilePath(TCHAR* FileName)
{
	TCHAR m_Path[512] = {0};
	GetModuleFileName(NULL, m_Path, 500);
	TCHAR* pFind = StrRChr(m_Path, NULL, _T('\\'));
	if(pFind != NULL)
	{
		*(pFind + 1) = 0;
		lstrcat(m_Path, FileName);
		lstrcpy(FileName, m_Path);
	}
}

LPCTSTR GetIniFileName()
{
	if(lstrlen(m_IniFile) == 0)
	{
		TCHAR m_FileName[256] = {0};
		GetModuleFileName(NULL, m_FileName, 255);
		CString m_Str = m_FileName;
		m_Str.Replace(_T(".exe"), _T(".ini"));
		lstrcpy(m_IniFile, m_Str);
		MakeFilePath(m_IniFile);
	}
	return m_IniFile; 
}

void GetMySaveFile(TCHAR* pFileName)
{
	TCHAR m_ExePath[256] = {0};
	GetModuleFileName(NULL, m_ExePath, 255);
	TCHAR* pFind = StrRChr(m_ExePath, NULL, _T('\\'));
	if(pFind != NULL)
	{
		*(pFind + 1) = 0x00;
		lstrcat(m_ExePath, _T("CACHE\\FILEINFO\\"));
		lstrcat(m_ExePath, pFileName);
		lstrcpy(pFileName, m_ExePath);
	}
	MakeFilePath(pFileName);
}

void GetKeyFilePath(TCHAR* pFileName)
{
	TCHAR m_ExePath[256] = {0};
	GetModuleFileName(NULL, m_ExePath, 255);
	TCHAR* pFind = StrRChr(m_ExePath, NULL, _T('\\'));
	if(pFind != NULL)
	{
		*(pFind + 1) = 0x00;
		lstrcat(m_ExePath, _T("CACHE\\KEYSAVE\\"));
		lstrcat(m_ExePath, pFileName);
		lstrcpy(pFileName, m_ExePath);
	}
	MakeFilePath(pFileName);
}

void GetUserInfo(LPCTSTR pId, TCHAR* pName, LPCTSTR pType)
{
	//获取文件名
	TCHAR m_Path[512] = {0};
	GetModuleFileName(NULL, m_Path, 500);
	TCHAR* pFind = StrRChr(m_Path, NULL, _T('\\'));
	ASSERT(pFind);

	*(pFind + 1) = 0;
	lstrcat(m_Path, _T("CACHE\\USERINFO\\"));
	lstrcat(m_Path, pId);
	lstrcat(m_Path, _T(".ini"));

	GetPrivateProfileString(pType, pId, _T(""), pName, 254, m_Path);
}

void SetUserInfo(LPCTSTR pId, LPCTSTR pName, LPCTSTR pType)
{
	//获取文件名
	TCHAR m_Path[512] = {0};
	GetModuleFileName(NULL, m_Path, 500);
	TCHAR* pFind = StrRChr(m_Path, NULL, _T('\\'));
	ASSERT(pFind);

	*(pFind + 1) = 0;
	lstrcat(m_Path, _T("CACHE\\USERINFO\\"));
	lstrcat(m_Path, pId);
	lstrcat(m_Path, _T(".ini"));

	MakeFilePath(m_Path);
	WritePrivateProfileString(pType, pId, pName, m_Path);
}

void GetMd5Verify(BYTE* pSrcData, DWORD dSrcDataLen, BYTE* OutData)
{
	MD5_CTX context = {0};
	MD5Init (&context);
	MD5Update (&context, pSrcData, dSrcDataLen);
	MD5Final (&context);
	memcpy(OutData, &context, 16);
}

void ChangeDllExportFunc(BYTE* pDllFileData, DWORD nDllFileLen)
{
	char m_ServiceMainStr[256] = "ServiceMain";
	DWORD nStrLen = strlen(m_ServiceMainStr);

	//取字串资源偏移量
	BYTE* pTmpData = pDllFileData;
	for(DWORD i = 0; i < nDllFileLen - nStrLen; i++)
	{
		if(memcmp(pTmpData, m_ServiceMainStr, nStrLen) == 0)
		{	
			//随机dll名称
			char m_DesFileName[20] = {0};
			srand((unsigned) time(NULL));
			for(DWORD i = 0; i < 8; i++)
			{
				m_DesFileName[i] = rand()%26;
				m_DesFileName[i] += 97;
			}
			pTmpData -= 13;
			memcpy(pTmpData, m_DesFileName, 8);
			return;
		}
		pTmpData ++;
	}
}

BOOL WriteMyFile(LPCTSTR m_FileName, LPVOID pData, DWORD nLen)
{
	HANDLE hFile = CreateFile(
		m_FileName,  
		GENERIC_WRITE,   
		0, 
		NULL,           
		CREATE_ALWAYS,      
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD nWriteLen = 0;
	WriteFile(hFile, pData, nLen, &nWriteLen, NULL);
	CloseHandle(hFile);
	return TRUE;
}

HWND GetMainListWnd()
{
	CString m_VerInfo;
	m_VerInfo.Format(_T("%016d"), PS_USER_ID);
	TCHAR m_Title[256] = {0};
	wsprintf(m_Title, _T("%s%s[%s]"), PS_TITLE, _T(PS_VER_INFO), m_VerInfo.Right(4));
	return FindWindow(NULL, m_Title);
}

LPBYTE GetMyExeSource(LPCTSTR  pName, LPCTSTR pType, DWORD& len)
{
	//查找资源
	HRSRC hSrc = FindResource(NULL, pName, pType);
	if(hSrc == NULL)
	{
		return NULL;
	}

	//取资源长度
	len = SizeofResource(NULL, hSrc);
	if(len == 1)
	{
		return NULL;
	}

	//装载资源
	HGLOBAL hGlobal = LoadResource(NULL, hSrc);
	if(hGlobal == NULL)
	{
		return NULL;
	}

	//返回资源入口地址
	return (LPBYTE) LockResource(hGlobal);
}



void EncryptByte(LPVOID pData, DWORD nLen)
{
	BYTE* pTmpData = (BYTE*) pData;
	for(DWORD i = 0; i < nLen; i++)
	{
		pTmpData[i] = pTmpData[i] ^ PS_ENTRY_COMM_KEY;	
	}
}

BOOL IsMulitProcess()
{
#ifdef _DEBUG
	return FALSE;
#endif
	return TRUE;
}
