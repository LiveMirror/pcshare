// XTPImageEditor.h : interface for the CXTPImageEditorDlg class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©1998-2005 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTP_IMAGEEDITORDLG_H__)
#define __XTP_IMAGEEDITORDLG_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPImageManagerIconHandle;
class CXTPImageEditorDlg;

//{{AFX_CODEJOCK_PRIVATE

enum XTPImageEditorTools
{
	xtpToolPencil,
	xtpToolFill,
	xtpToolGetColor,
	xtpToolLine,
	xtpToolRectangle,
	xtpToolCircle
};



#define XTPCOLOR_ERASE (0xFFFEFF)


//===========================================================================
// Summary:
//     Internal class used int CXTPImageEditorDlg
//===========================================================================
class _XTP_EXT_CLASS CXTPImageEditorPicker : public CStatic
{
	DECLARE_DYNAMIC(CXTPImageEditorPicker)

public:
	CXTPImageEditorPicker();

public:
	void SetColor(COLORREF clr);
	COLORREF GetColor();
	void SetSelected(BOOL bSelected = TRUE);

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	COLORREF m_clr;
	BOOL m_bSelected;
};


//===========================================================================
// Summary:
//     Internal class used int CXTPImageEditorDlg
//===========================================================================
class _XTP_EXT_CLASS CXTPImageEditorPicture : public CStatic
{
public:
	class CAlphaBitmap : public CBitmap
	{
	public:
		CAlphaBitmap(BOOL bAlpha = FALSE)
		{
			m_bAlpha = bAlpha;
		}
	public:
		void CreateEditorBitmap(int nWidth, int nHeight);

	public:
		BOOL m_bAlpha;

	};

public:
	CXTPImageEditorPicture();
	~CXTPImageEditorPicture();

public:
	void Apply();
	void Init(UINT nID, CXTPImageEditorDlg* pParentWnd);
	XTPImageEditorTools GetSelectedTool();
	COLORREF GetSelectedColor();
	CPoint ClientToPicture(CPoint pt);
	static void CopyBitmap(CAlphaBitmap* pBitmapCopyTo, CAlphaBitmap* pBitmapCopyFrom, CSize sz, CPoint ptOffset = 0, COLORREF clrTransparentFrom = XTPCOLOR_ERASE, COLORREF clrTransparentTo = XTPCOLOR_ERASE, CDC* pDC = 0);
	COLORREF GetPicturePixel(CPoint point);
	void FixAlphaLayer(CAlphaBitmap* pBitmap, CAlphaBitmap* pPictureBitmap);
	void Undo();
	void Redo();
	void Clear();
	void Copy();
	void Paste();
	void Load();
	void MovePicture(CPoint ptOffset);
	void OnPictureChanged();


protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

public:
	CSize m_szPicture;
	CSize m_szItem;
	CXTPImageEditorDlg* m_pParentWnd;

	CDC m_dcPicture;

	CAlphaBitmap* m_pCurrentBitmap;
	CAlphaBitmap* m_pPictureBitmap;

	CList<CAlphaBitmap*, CAlphaBitmap*> m_lstUndo;
	CList<CAlphaBitmap*, CAlphaBitmap*> m_lstRedo;
	CPoint m_ptTracked;
	BOOL m_bTracked;
	COLORREF m_clrDraw;

	HCURSOR m_hCursorLine;
	HCURSOR m_hCursorFillColor;
	HCURSOR m_hCursorPencil;
	HCURSOR m_hCursorPickColor;
};

//===========================================================================
// Summary:
//     Internal class used int CXTPImageEditorDlg
//===========================================================================
class _XTP_EXT_CLASS CXTPImageEditorPreview : public CStatic
{
public:

	CXTPImageEditorPreview();

public:
	void Init(UINT nID, CXTPImageEditorDlg* pParentWnd);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

protected:
	CXTPImageEditorDlg* m_pParentWnd;
	CSize m_szPicture;
};

//}}AFX_CODEJOCK_PRIVATE



//===========================================================================
// Summary
//     CXTPImageEditorDlg is a CDialog derived class. It represents the dialog
//     for icons edition.
//===========================================================================
class _XTP_EXT_CLASS CXTPImageEditorDlg : public CDialog
{
private:
	//-----------------------------------------------------------------------
	// Summary:
	//     Child Tools bar.
	//-----------------------------------------------------------------------
	class CDlgToolBar : public CToolBar
	{
	public:

		CDlgToolBar()
		{
			m_sizeImage = CSize(16, 16);
			m_sizeButton = CSize(23, 23);
		}

		LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
		{
			if (IsWindowVisible())
			{
				CFrameWnd* pParent = (CFrameWnd*)GetParent();
				if (pParent)
					OnUpdateCmdUI(pParent, (BOOL)wParam);
			}
			return 0L;
		}
		DECLARE_MESSAGE_MAP()
	};

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPImageEditorDlg object
	// Parameters:
	//     pParent - Points to the objects parent window.
	//-----------------------------------------------------------------------
	CXTPImageEditorDlg(CWnd* pParent = NULL);

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon to edit
	// Parameters:
	//     pHandle - Reference to a CXTPImageManagerIconHandle object that
	//               represents the handle of the icon.
	//-----------------------------------------------------------------------
	void SetIcon(const CXTPImageManagerIconHandle& pHandle);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon size to edit.
	// Parameters:
	//     szIcon - Icon size.
	//-----------------------------------------------------------------------
	void SetIconSize(CSize szIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves the edited icon
	// Parameters:
	//     pHandle - Handle of the icon
	//-----------------------------------------------------------------------
	void GetIcon(CXTPImageManagerIconHandle* pHandle);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTPImageEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPImageEditorDlg)
	afx_msg void OnUpdateButtonTool(CCmdUI* pCmdUI);
	afx_msg void OnButtonTool(UINT nID);
	afx_msg LRESULT OnKickIdle(WPARAM wp, LPARAM lCount);
	afx_msg void OnUpdateButtonUndo(CCmdUI* pCmdUI);
	afx_msg void OnButtonUndo();
	afx_msg void OnUpdateButtonRedo(CCmdUI* pCmdUI);
	afx_msg void OnButtonRedo();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonCopy();
	afx_msg void OnButtonPaste();
	afx_msg void OnButtonOpen();
	afx_msg void OnMoveButton(UINT nID);
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	void OnPictureChanged();
	void OnPickerClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnSelectorClick(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
	void OnEraseClick(NMHDR* pNMHDR, LRESULT* pResult);
	void SetSelected(CXTPImageEditorPicker* pPicker);
	void FixAlphaLayer(CBitmap* pSrc);

public:
//{{AFX_CODEJOCK_PRIVATE
	//{{AFX_DATA(CXTPImageEditorDlg)
	enum { IDD = XTP_IDD_IMAGEEDITOR_DIALOG };
	//}}AFX_DATA
//}}AFX_CODEJOCK_PRIVATE

private:
	CButton m_gboxTools;
	CButton m_gboxMove;

	CDlgToolBar m_wndToolbarTools;
	CImageList m_ilToolbarTools;

	CDlgToolBar m_wndToolbarMove;
	CImageList m_ilToolbarMove;


	CXTPImageEditorPicker m_wndSelector[16];
	CXTPImageEditorPicker m_wndPicker;
	CXTPImageEditorPicker m_wndSelectorErase;
	CXTPImageEditorPicture m_wndPicture;
	CXTPImageEditorPreview m_wndPreview;
	CXTPImageEditorPicker* m_pSelected;

	XTPImageEditorTools m_toolSelected;
	XTPImageEditorTools m_toolPrevious;
	CSize m_szPicture;

private:
	friend class CXTPImageEditorPicture;
	friend class CXTPImageEditorPreview;
};

#endif // !defined(__XTP_IMAGEEDITORDLG_H__)
