// [!output CHILD_FRAME_HEADER] : interface of the [!output CHILD_FRAME_CLASS] class
//


#pragma once

[!if PROJECT_STYLE_EXPLORER]
class [!output VIEW_CLASS];
[!endif]
[!if !DOCVIEW]
#include "[!output WND_VIEW_HEADER]"
[!endif]

class [!output CHILD_FRAME_CLASS] : public [!output CHILD_FRAME_BASE_CLASS]
{
	DECLARE_DYNCREATE([!output CHILD_FRAME_CLASS])
public:
	[!output CHILD_FRAME_CLASS]();

// Attributes
[!if APP_TYPE_MDI && SPLITTER || PROJECT_STYLE_EXPLORER]
protected:
	CSplitterWnd m_wndSplitter;
[!endif]
public:

// Operations
public:

// Overrides
[!if APP_TYPE_MDI && SPLITTER]
	public:
[!endif]
[!if PROJECT_STYLE_EXPLORER || !CHILD_FRAME_DEFAULT_STYLES || !DOCVIEW]
	public:
[!endif]
[!if APP_TYPE_MDI && SPLITTER]
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
[!else]
[!if PROJECT_STYLE_EXPLORER]
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
[!endif]
[!endif]
[!if MDITABS]
	virtual void ActivateFrame(int nCmdShow);
[!endif]
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
[!if !DOCVIEW]
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
[!endif]

// Implementation
public:
[!if !DOCVIEW]
	// view for the client area of the frame.
	[!output WND_VIEW_CLASS] m_wndView;
[!endif]
	virtual ~[!output CHILD_FRAME_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

[!if PROJECT_STYLE_EXPLORER]
protected:
	[!output VIEW_CLASS]* GetRightPane();
[!endif]
// Generated message map functions
protected:
[!if !DOCVIEW]
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
[!endif]
[!if PROJECT_STYLE_EXPLORER]
[!if LIST_VIEW]
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
[!endif]
[!endif]
	DECLARE_MESSAGE_MAP()
};
