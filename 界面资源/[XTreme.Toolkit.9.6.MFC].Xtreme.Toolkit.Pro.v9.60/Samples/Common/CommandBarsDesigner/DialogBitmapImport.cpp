// DialogBitmapImport.cpp : implementation file
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

#include "stdafx.h"
#include "resource.h"
#include "DialogBitmapImport.h"
#include "ResourceManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapImport dialog


CDialogBitmapImport::CDialogBitmapImport(CXTPImageManager* pImageManager, LPCTSTR lpszFileName, CResourceManager* pResourceManager)
	: CDialogBitmapImagesHost(CDialogBitmapImport::IDD, pResourceManager)
	, m_pParentImageManager(pImageManager)
{
	//{{AFX_DATA_INIT(CDialogBitmapImport)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strFileName = lpszFileName;

	m_pImageManager = new CXTPImageManager();

	m_szBitmap = GetBitmapSize();
	m_szIcons = m_szBitmap;

	int delta = m_szBitmap.cx;

	for (int i = 1; i < m_szBitmap.cx - 1; i++)
	{
		int nWidth = m_szBitmap.cx / i;

		if (nWidth * i != m_szBitmap.cx)
			continue;

		if (abs(nWidth - m_szIcons.cy) < delta)
		{
			delta = abs(nWidth - m_szIcons.cy);
			m_szIcons.cx = nWidth;
		}
	}




	ReloadBitmap();
}

CDialogBitmapImport::~CDialogBitmapImport()
{
	if (m_pImageManager)
	{
		delete m_pImageManager;
	}
}


void CDialogBitmapImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBitmapImport)
	DDX_Control(pDX, IDC_COMBO_MASK, m_comboMask);
	DDX_Control(pDX, IDC_COMBO_STATE, m_comboState);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_comboDevice);
	DDX_Control(pDX, IDC_COMBO_ID, m_comboId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogBitmapImport, CDialog)
	//{{AFX_MSG_MAP(CDialogBitmapImport)
	ON_CBN_SELCHANGE(IDC_COMBO_ID, OnIdChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, OnDeviceChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_MASK, OnMaskChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogBitmapImport message handlers

BOOL CDialogBitmapImport::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT(m_pImageManager);

	CXTPWindowRect rc(GetDlgItem(IDC_STATIC_IMAGES));
	ScreenToClient(&rc);

	m_wndImages.Create(AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW)), 0, WS_CHILD|WS_VISIBLE, rc, this, 0);
	m_wndImages.m_bAllowEdit = FALSE;

	for (int i = 1; i < m_szBitmap.cx - 1; i++)
	{
		int nWidth = m_szBitmap.cx / i;

		if (nWidth * i != m_szBitmap.cx)
			continue;

		CString strDevice;
		strDevice.Format(_T("%ix%i"), nWidth, m_szIcons.cy);

		int nIndex = m_comboDevice.AddString(strDevice);
		m_comboDevice.SetItemData(nIndex, MAKELONG(nWidth, m_szIcons.cy));

		if (nWidth == m_szIcons.cx)
			m_comboDevice.SetCurSel(nIndex);

	}

	CMapResources* pResources = m_pResourceManager->GetResources();

	POSITION pos = pResources->GetStartPosition();
	while (pos)
	{
		CResourceInfo* pInfo;
		CString strCaption;
		pResources->GetNextAssoc(pos, strCaption, (CObject*&)pInfo);

		int nIndex = m_comboId.AddString(strCaption);
		m_comboId.SetItemData(nIndex, pInfo->m_nID);

	}

	if (m_bAlpha)
		m_comboMask.EnableWindow(FALSE);


	m_comboMask.AddString(_T("Select Color..."));

	m_comboState.AddString(_T("xtpImageNormal"));
	m_comboState.AddString(_T("xtpImageDisabled"));
	m_comboState.AddString(_T("xtpImageHot"));
	m_comboState.AddString(_T("xtpImageChecked"));
	m_comboState.SetCurSel(0);



	OnUpdateIcons();
	OnImageChanged(0);

	SetWindowText(_T("Import"));
	GetDlgItem(IDOK)->SetWindowText(_T("Import"));


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBitmapImport::OnUpdateIcons()
{
	m_arrImages.RemoveAll();

	CSize sz(m_szIcons);

	CMap<UINT, UINT, CXTPImageManagerIconSet*, CXTPImageManagerIconSet*>* pImages = m_pImageManager->GetImages();

	UINT nID, nWidth;
	CXTPImageManagerIconSet* pIconSet;
	CXTPImageManagerIcon* pIcon;
	POSITION pos = pImages->GetStartPosition();
	while (pos)
	{
		pImages->GetNextAssoc(pos, nID, pIconSet);
		CXTPImageManagerIconSet::CIconSetMap* pIconSetMap = pIconSet->GetIcons();

		POSITION posIconSet  = pIconSetMap->GetStartPosition();
		while (posIconSet)
		{
			pIconSetMap->GetNextAssoc(posIconSet, nWidth, pIcon);

			CImage image;
			image.nID = 0;
			image.imageState = (XTPImageState)-1;
			image.pIcon = pIcon;
			image.pParent = this;

			int nIndex = 0;
			for (; nIndex < m_arrImages.GetSize(); nIndex ++) if (m_arrImages[nIndex].pIcon->GetID() > pIcon->GetID())
				break;
			m_arrImages.InsertAt(nIndex, image);
		}

	}

	for (int i = 0; i < m_arrImages.GetSize(); i++)
		m_arrImages[i].nIndex = i;

	m_pSelected = 0;
	m_wndImages.OnImageCountChanged();
}

void CDialogBitmapImport::OnImageChanged(CImage* pImage)
{
	m_pSelected = pImage;

	m_comboId.EnableWindow(pImage != NULL);
	m_wndImages.Invalidate(FALSE);

	if (pImage != NULL)
	{
		CString strResource = m_pResourceManager->GetStringID(pImage->nID);
		int nIndex = m_comboId.FindStringExact(0, strResource);
		m_comboId.SetCurSel(nIndex);
	}
	else
	{
		m_comboId.SetCurSel(-1);
	}
}

void CDialogBitmapImport::OnIdChanged()
{
	if (m_pSelected == 0)
		return;

	int nIndex = m_comboId.GetCurSel();

	if (nIndex == CB_ERR)
		return;

	UINT nIDSelected = (UINT)m_comboId.GetItemData(nIndex);

	m_pSelected->nID = nIDSelected;
}


CSize CDialogBitmapImport::GetBitmapSize()
{
	HBITMAP hBmp = (HBITMAP)LoadImage(0, m_strFileName,
		IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION |LR_LOADFROMFILE);

	if (!hBmp)
		return 0;

	CBitmap bmpFile;
	bmpFile.Attach(hBmp);

	BITMAP bmpInfo;
	bmpFile.GetBitmap(&bmpInfo);

	m_clrMask = XTPImageManager()->GetBitmapMaskColor(bmpFile);

	return CSize(bmpInfo.bmWidth, bmpInfo.bmHeight);
}

void CDialogBitmapImport::ReloadBitmap()
{
	m_bAlpha = CXTPImageManagerIcon::IsAlphaBitmapFile(m_strFileName);

	HBITMAP hBmp = (HBITMAP)LoadImage(0, m_strFileName,
		IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION |LR_LOADFROMFILE);

	if (!hBmp)
		return;

	CBitmap bmpFile;
	bmpFile.Attach(hBmp);

	ASSERT(m_szBitmap.cx % m_szIcons.cx == 0);

	int nCount = m_szBitmap.cx / m_szIcons.cx;

	m_pImageManager->SetMaskColor(m_clrMask);
	VERIFY(m_pImageManager->SetIcons(bmpFile, 0, nCount, m_szIcons, xtpImageNormal, m_bAlpha));
}


void CDialogBitmapImport::OnDeviceChanged()
{
	int nIndex = m_comboDevice.GetCurSel();

	if (nIndex == CB_ERR)
		return;

	m_pSelected = 0;

	DWORD dwDevice = (DWORD)m_comboDevice.GetItemData(nIndex);
	m_szIcons = CSize(dwDevice);


	m_pImageManager->RemoveAll();
	ReloadBitmap();

	OnUpdateIcons();
	OnImageChanged(NULL);
}

void CDialogBitmapImport::OnMaskChanged()
{
	CColorDialog cd(m_clrMask);

	if (cd.DoModal() != IDOK)
		return;

	m_clrMask = cd.GetColor();
	m_comboMask.SetCurSel(-1);

	m_pSelected = 0;

	m_pImageManager->RemoveAll();
	ReloadBitmap();

	OnUpdateIcons();
	OnImageChanged(NULL);

	m_comboMask.Invalidate(FALSE);

}

void CDialogBitmapImport::OnOK()
{
	XTPImageState imageState = (XTPImageState)m_comboState.GetCurSel();

	for (int i = 0; i < m_arrImages.GetSize(); i++)
	{
		CImage& image = m_arrImages[i];
		if (image.nID == 0)
			continue;

		CXTPImageManagerIconHandle& Handle = image.GetHandle();

		m_pParentImageManager->SetIcon(Handle, image.nID, 0, imageState);

	}

	CDialog::OnOK();
}
