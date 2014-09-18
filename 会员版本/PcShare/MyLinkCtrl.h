#if !defined(AFX_MYLINK_H__CB717501_4ABD_4331_AE3C_BD28C050F561__INCLUDED_)
#define AFX_MYLINK_H__CB717501_4ABD_4331_AE3C_BD28C050F561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyLinkCtrl window

class CMyLinkCtrl : public CStatic
{
// Construction
public:
	CMyLinkCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyLinkCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:


	CSize GetMySize(LPCTSTR m_Text);
	void SetMyFont(LPCTSTR FontName,int FontSize,int FontWidth,BOOL IsLine);
	virtual ~CMyLinkCtrl();

	// Generated message map functions
protected:

	HCURSOR m_LogoCur;
	CFont m_Font;
	//{{AFX_MSG(CMyLinkCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLINK_H__CB717501_4ABD_4331_AE3C_BD28C050F561__INCLUDED_)
