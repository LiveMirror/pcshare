// [!output MAIN_FRAME_HEADER] : interface of the [!output MAIN_FRAME_CLASS] class
//


#pragma once
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
[!if PROJECT_STYLE_EXPLORER]

class [!output VIEW_CLASS];
[!else]
[!if !DOCVIEW]

#include "[!output WND_VIEW_HEADER]"
[!endif]
[!endif]
[!endif] 
[!if DOCK_PANE]
#include "DockPaneWnd.h"
[!endif] 

[!if APP_TYPE_MDI]
class [!output MAIN_FRAME_CLASS] : public [!output XTP_MAIN_FRAME_BASE_CLASS]
{
	DECLARE_DYNAMIC([!output MAIN_FRAME_CLASS])
public:
	[!output MAIN_FRAME_CLASS]();
[!else]
class [!output MAIN_FRAME_CLASS] : public [!output XTP_MAIN_FRAME_BASE_CLASS]
{
	
[!if !DOCVIEW]
public:
	[!output MAIN_FRAME_CLASS]();
protected: 
	DECLARE_DYNAMIC([!output MAIN_FRAME_CLASS])
[!else]
protected: // create from serialization only
	[!output MAIN_FRAME_CLASS]();
	DECLARE_DYNCREATE([!output MAIN_FRAME_CLASS])
[!endif]
[!endif]

// Attributes
[!if SPLITTER || PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
protected:
	CSplitterWnd m_wndSplitter;
[!endif]
[!endif]
public:

// Operations
public:

// Overrides
public:
[!if SPLITTER || PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
[!endif] 
[!endif] 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
[!if !DOCVIEW]
[!if APP_TYPE_SDI]
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
[!endif] 
[!endif]
[!if APP_TYPE_MTLD]
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
[!endif]

// Implementation
public:
	virtual ~[!output MAIN_FRAME_CLASS]();
[!if PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
	[!output VIEW_CLASS]* GetRightPane();
[!endif]
[!endif]
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
[!if DOCKING_TOOLBAR || STATUS_BAR]

protected:  // control bar embedded members
[!if STATUS_BAR]
	CXTPStatusBar  m_wndStatusBar;
[!endif]
[!endif]
[!if TOOLBAR_STYLE_REBAR]
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;
[!endif]
[!if !DOCVIEW]
[!if APP_TYPE_SDI]
	[!output WND_VIEW_CLASS]    m_wndView;
[!endif] 
[!endif]
[!if DOCK_PANE]
	CXTPDockingPaneManager m_paneManager;
	CDockPaneWnd<CStatic> m_wndOptions;
	CDockPaneWnd<CEdit> m_wndProperties;
[!endif]
[!if MDITABS]
	CXTPTabClientWnd m_MTIClientWnd;
[!endif]

// Generated message map functions
protected:
[!if DOCKING_TOOLBAR || STATUS_BAR]
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
[!else]
[!if APP_TYPE_SDI && !DOCVIEW]
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
[!endif]
[!endif]
[!if !DOCVIEW]
[!if APP_TYPE_SDI]
	afx_msg void OnSetFocus(CWnd *pOldWnd);
[!endif] 
[!endif]
[!if APP_TYPE_MTLD]
	afx_msg void OnFileClose();
[!endif]
	afx_msg void OnClose();
[!if MDITABS]
	afx_msg void OnDestroy(); 
[!endif]
[!if PROJECT_STYLE_EXPLORER]
[!if APP_TYPE_SDI || APP_TYPE_MTLD]
[!if LIST_VIEW]
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
[!endif]
[!endif]
[!endif]
[!if CUSTOMIZE]
	afx_msg void OnCustomize();
[!if CUSTOMIZE_CUSTOM]
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
[!endif]
[!endif]
[!if DOCK_PANE]
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
[!endif]
	DECLARE_MESSAGE_MAP()
};


