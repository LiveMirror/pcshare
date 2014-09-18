// XTMaskEdit.cpp : implementation of the CXTMaskEdit class.
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

#include "StdAfx.h"
#include "XTPVC80Helpers.h"  // Visual Studio 2005 helper functions

#include "XTGlobal.h"
#include "XTMaskEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTMaskEdit
/////////////////////////////////////////////////////////////////////////////

CXTMaskEdit::CXTMaskEdit( )
{
}


IMPLEMENT_DYNAMIC(CXTMaskEdit, CEdit)

BEGIN_MESSAGE_MAP(CXTMaskEdit, CEdit)
	ON_MASKEDIT_REFLECT()
END_MESSAGE_MAP()

void CXTMaskEdit::SetEditMask(LPCTSTR lpszMask,LPCTSTR lpszLiteral,LPCTSTR lpszDefault /*=NULL*/)
{
	CXTMaskEditT<CEdit>::SetEditMask(lpszMask, lpszLiteral, lpszDefault);
}

bool CXTMaskEdit::Initialize(CWnd* /*pParentWnd*/)
{
	SetFont( &XTAuxData().font );
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CXTDateEdit class
/////////////////////////////////////////////////////////////////////////////

CXTDateEdit::CXTDateEdit( )
{
	m_bUseMask      = true;
	m_strMask       = _T("00/00/0000");
	m_strLiteral    = _T("__/__/____");
}

COleDateTime CXTDateEdit::ReadOleDateTime(LPCTSTR lpszData)
{
	COleDateTime dt;
	dt.ParseDateTime(lpszData);
	return dt;
}

IMPLEMENT_DYNAMIC(CXTDateEdit, CXTMaskEdit)

void CXTDateEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	strData = dt.Format( _T("%d/%m/%Y") );
}

void CXTDateEdit::SetDateTime(COleDateTime& dt)
{
	CString strText;
	FormatOleDateTime(strText, dt);
	m_strWindowText = m_strDefault = strText;
	SetWindowText(strText);
}

void CXTDateEdit::SetDateTime(CString strDate)
{
	m_strWindowText = m_strDefault = strDate;
	SetWindowText(strDate);
}

COleDateTime CXTDateEdit::GetDateTime( )
{
	CString strText;
	GetWindowText(strText);
	return ReadOleDateTime(strText);
}

CString CXTDateEdit::GetWindowDateTime( )
{
	CString strText;
	GetWindowText(strText);
	return strText;
}

bool CXTDateEdit::ProcessMask(UINT& nChar,int nEndPos)
{
	// check the key against the mask
	switch ( m_strMask.GetAt( nEndPos ) )
	{
	case '0':       // digit only //completely changed this
		{
			bool bReturn = true;

			if ( _istdigit( ( TCHAR )nChar ) )
			{
				if ( nEndPos == 0 )
				{
					if ( nChar > '3' ) {
						bReturn = false;
					}
				}
				else if ( nEndPos == 1 )
				{
					if ( m_strWindowText.GetAt(0) == '3' )
					{
						if ( nChar > '1' ) {
							bReturn = false;
						}
					}
				}
				else if ( nEndPos == 3 )
				{
					if ( nChar > '1' ) {
						bReturn = false;
					}
				}
				else if ( nEndPos == 4 )
				{
					if ( m_strWindowText.GetAt(3) == '1' )
					{
						if ( nChar > '2') {
							bReturn = false;
						}
					}
				}
				return bReturn;
			}
			break;
		}
	}

	MessageBeep((UINT)-1);
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CXTTimeEdit class
/////////////////////////////////////////////////////////////////////////////

CXTTimeEdit::CXTTimeEdit( )
{
	m_bMilitary  = false;
	m_bUseMask   = true;
	m_strMask    = _T("00:00");
	m_strLiteral = _T("__:__");
	m_iHours     = 0;
	m_iMins      = 0;
}

IMPLEMENT_DYNAMIC(CXTTimeEdit, CXTDateEdit)

void CXTTimeEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	if ( dt.m_dt == 0 ) {
		strData = _T("00:00");
	}
	else {
		strData = dt.Format( _T("%H:%M") );
	}
}

bool CXTTimeEdit::ProcessMask(UINT& nChar,int nEndPos)
{
	// check the key against the mask
	switch ( m_strMask.GetAt( nEndPos ) )
	{
	case '0':       // digit only //completely changed this
		{
			if ( _istdigit( (TCHAR)nChar ) )
			{
				switch (nEndPos)
				{
				case 0:
					if (m_bMilitary)
					{
						if (nChar < 48 || nChar > 50)
						{
							MessageBeep((UINT)-1);
							return false;
						}
					}
					else
					{
						if (nChar < 48 || nChar > 49)
						{
							MessageBeep((UINT)-1);
							return false;
						}
					}
					break;

				case 1:
					if (m_bMilitary)
					{
						if (nChar < 48 || nChar > 52)
						{
							MessageBeep((UINT)-1);
							return false;
						}
					}
					else
					{
						if (nChar < 48 || nChar > 50)
						{
							MessageBeep((UINT)-1);
							return false;
						}
					}
					break;

				case 3:
					if (nChar < 48 || nChar > 53)
					{
						MessageBeep((UINT)-1);
						return false;
					}
					break;

				case 4:
					if (nChar < 48 || nChar > 57)
					{
						MessageBeep((UINT)-1);
						return false;
					}
					break;
				}

				return true;
			}
			break;
		}
	}

	MessageBeep((UINT)-1);
	return false;
}

void CXTTimeEdit::SetHours(int nHours)
{
	m_iHours = nHours;

	CString strText;
	strText.Format( _T("%02d:%02d"), m_iHours, m_iMins );
	SetWindowText( strText );
}

void CXTTimeEdit::SetMins(int nMins)
{
	m_iMins = nMins;

	CString strText;
	strText.Format( _T("%02d:%02d"), m_iHours, m_iMins );
	SetWindowText( strText );
}

void CXTTimeEdit::SetTime(int nHours, int nMins)
{
	m_iHours = nHours;
	m_iMins  = nMins;

	CString strText;
	strText.Format( _T("%02d:%02d"), m_iHours, m_iMins );
	SetWindowText( strText );
}

/////////////////////////////////////////////////////////////////////////////
// DDX_ routines
/////////////////////////////////////////////////////////////////////////////

_XT_EXT_CLASS void AFXAPI DDX_XTOleDateTime(CDataExchange* pDX, int nIDC, CXTDateEdit& rControl, COleDateTime& rDateTime)
{
	DDX_Control( pDX, nIDC, rControl );

	if ( pDX->m_bSaveAndValidate ) {
		rDateTime = rControl.GetDateTime( );
	}
	else {
		rControl.SetDateTime( rDateTime );
	}
}
