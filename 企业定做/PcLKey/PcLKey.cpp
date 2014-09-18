// PcLKey.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

extern HINSTANCE m_ghInstance;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if(DLL_PROCESS_ATTACH == ul_reason_for_call)
	{
		m_ghInstance = (HINSTANCE) hModule;
	}
    return TRUE;
}

