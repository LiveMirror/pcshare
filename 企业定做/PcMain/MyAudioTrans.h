// MyAudioTrans.h: interface for the CMyAudioTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYAUDIOTRANS_H__D111E7A4_A389_4D76_94D7_7FDB145F1BB4__INCLUDED_)
#define AFX_MYAUDIOTRANS_H__D111E7A4_A389_4D76_94D7_7FDB145F1BB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyClientTran.h"

class CMyAudioTrans : public CMyClientTran  
{
public:
	CMyAudioTrans();
	virtual ~CMyAudioTrans();

	void StartWork(HWND hWnd);

	BOOL			m_IsInitAudio, m_IsExitAudio;
	WAVEFORMATEX	m_WaveInfo;
	DWORD			nThreadAudio;

	LPWAVEHDR		*pWareBuf;
	DWORD			nWareBufCount;
	inline void GetAudioData(HWAVEIN m_hRecord, LPWAVEHDR lpWHdr);
	static void CALLBACK OnCaptureAudio(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
};

#endif // !defined(AFX_MYAUDIOTRANS_H__D111E7A4_A389_4D76_94D7_7FDB145F1BB4__INCLUDED_)
