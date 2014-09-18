// XTMaskEditT.h interface for the CXTMaskEditT class.
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
#if !defined(__XTMASKEDITEX_H__)
#define __XTMASKEDITEX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//////////////////////////////////////////////////////////////////////
// Summary:
//     CXTMaskEditT is a template class. It allows text masking to be
//     applied to the control to format it for special editing restrictions.
//////////////////////////////////////////////////////////////////////
template <class TBase>
class CXTMaskEditT : public TBase
{

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTMaskEditT object
	//-----------------------------------------------------------------------
	CXTMaskEditT()
		: m_nStartChar( 0 )
		, m_nEndChar( 0 )
		, m_bOverType( true )
		, m_bUseMask( true )
		, m_bRedo( false )
		, m_bModified( false )
		, m_strWindowText( _T( "" ) )
		, m_strMask( _T( "" ) )
		, m_strLiteral( _T( "" ) )
		, m_strDefault( _T( "" ) )
		, m_strUndoBuffer( _T( "" ) )
		, m_chPrompt( _T('_') )
	{
	}

public:

	//-----------------------------------------------------------------------
	// Parameters:
	//     bUseMask - true to enable the mask. false to disable the mask.
	// Summary:
	//     Call this member function to enable or disable the mask for the mask
	//     edit control.
	//-----------------------------------------------------------------------
	void SetUseMask(bool bUseMask) {
		m_bUseMask = bUseMask;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the mask for the edit
	//     control can be used.
	// Returns:
	//     true if the mask can be used, otherwise returns false.
	//-----------------------------------------------------------------------
	bool CanUseMask() {
		return m_bUseMask && ((GetStyle() & ES_READONLY) == 0);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to enable or disable type over, also
	//     known as insert mode.
	// Parameters:
	//     bOverType - true to enable type over.
	//-----------------------------------------------------------------------
	void SetOverType(bool bOverType) {
		m_bOverType = bOverType;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if type over has been enabled.
	// Returns:
	//     true if type over is enabled, otherwise returns false.
	//-----------------------------------------------------------------------
	bool CanOverType() {
		return m_bOverType;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine if the index specified
	//     by 'iPos' is a valid index for the currently displayed edit text.
	// Parameters:
	//     iPos - Index of the character to check.
	// Returns:
	//     true if the index is valid, otherwise returns false.
	//-----------------------------------------------------------------------
	bool PosInRange(int iPos) {
		return ( ( iPos >= 0 ) && ( iPos < m_strLiteral.GetLength( ) ) );
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the character that is currently used as
	//     the mask prompt. The mask prompt indicates that the field is editable.
	// Returns:
	//     A TCHAR data type.
	//-----------------------------------------------------------------------
	TCHAR GetPromptChar() {
		return m_chPrompt;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to set the prompt character that is
	//     displayed to the user that indicates the field can be edited.
	// Parameters:
	//     ch - A TCHAR data type.
	//-----------------------------------------------------------------------
	void SetPromptChar(TCHAR ch)
	{
		if (m_chPrompt == ch)
			return;

		for (int i = 0; i < m_strLiteral.GetLength(); i++)
			if (m_strLiteral[i] == m_chPrompt) m_strLiteral.SetAt(i, ch);

		for (int j = 0; j < m_strWindowText.GetLength(); j++)
			if (m_strWindowText[j] == m_chPrompt) m_strWindowText.SetAt(j, ch);

		m_chPrompt = ch;
		SetWindowText( m_strWindowText );

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to perform a cut operation using the
	//     currently selected text.
	//-----------------------------------------------------------------------
	void MaskCut()
	{
		if ( !CanUseMask( ))
		{
			DefWindowProc( WM_CUT, 0, 0);
		}
		else
		{
			MaskCopy( );
			MaskClear( );

			m_bRedo = false;
			m_bModified = true;
			SetWindowText( m_strWindowText );

			CorrectPosition( m_nStartChar );
			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to perform a copy operation using the
	//     currently selected text.
	//-----------------------------------------------------------------------
	void MaskCopy()
	{
		if ( !CanUseMask( ))
		{
			DefWindowProc( WM_COPY, 0, 0);
		}
		else
		{
			GetSel( m_nStartChar, m_nEndChar );

			CString strBuffer;
			int i;
			for ( i = m_nStartChar; i < m_nEndChar; ++i )
			{
				if ( m_strLiteral.GetAt( i ) == m_chPrompt )
				{
					if ( m_strWindowText.GetAt( i ) == m_chPrompt )
						continue;

					strBuffer += m_strWindowText.GetAt( i );
				}
			}

			CopyToClipboard( strBuffer );
		}

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to perform a paste operation using the
	//     current clipboard text.
	//-----------------------------------------------------------------------
	void MaskPaste()
	{
		if ( !CanUseMask( ))
		{
			DefWindowProc(WM_PASTE, 0, 0);
		}
		else
		{
			GetSel( m_nStartChar, m_nEndChar );
			MaskClear( );

			if ( !OpenClipboard( ) )
				return;

			LPTSTR lptstrPaste = NULL;
			HGLOBAL hglbPaste = ::GetClipboardData( CF_TEXT );
			if ( hglbPaste != NULL )
			{
				lptstrPaste = ( TCHAR* )::GlobalLock( hglbPaste );
				if ( lptstrPaste != NULL )
				{
					::GlobalUnlock( hglbPaste );
				}
			}

			::CloseClipboard();

			int i, x = m_nStartChar, iLen = (int)_tcslen( lptstrPaste );

			for ( i = 0; i < iLen; ++i )
			{
				UINT ch = lptstrPaste[ i ];

				if ( CheckChar( ch, false ) )
				{
					InsertCharAt( x, ( TCHAR )ch );
					++x;

					if ( x == m_strWindowText.GetLength( ) )
						break;
				}
			}

			m_bRedo = false;
			m_bModified = true;
			SetWindowText( m_strWindowText );

			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to clear the current text selection.
	//-----------------------------------------------------------------------
	void MaskClear()
	{
		if ( !CanUseMask( ))
		{
			DefWindowProc( WM_CLEAR, 0, 0);
		}
		else
		{
			if ( m_nStartChar == m_nEndChar )
				return;

			// check to see if there is anything left to delete
			int iLength = m_strWindowText.GetLength( );

			int i;
			for ( i = m_nStartChar; i < iLength; ++i )
			{
				TCHAR ch1 = m_strLiteral.GetAt( i );
				TCHAR ch2 = m_strWindowText.GetAt( i );

				if ( ( ch1 == m_chPrompt ) && ( ch2 != m_chPrompt ) )
				{
					break;
				}
			}

			if ( i == iLength )
			{
				::MessageBeep( ( UINT )-1 );
				return;
			}

			for ( i = m_nStartChar; i < m_nEndChar; ++i )
			{
				if ( m_strLiteral.GetAt( i ) == m_chPrompt )
				{
					DeleteCharAt( m_nStartChar );
				}
			}

			m_bRedo = false;
			m_bModified = true;
			SetWindowText( m_strWindowText );

			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to undo the previous action.
	//-----------------------------------------------------------------------
	void MaskUndo()
	{
		if ( !CanUseMask( ))
		{
			DefWindowProc( EM_UNDO, 0, 0);
		}
		else
		{
			GetSel( m_nStartChar, m_nEndChar );

			if ( m_bRedo )
			{
				m_bRedo = false;
				m_bModified = true;
				SetWindowText( m_strRedoBuffer );
			}
			else
			{
				m_bRedo = true;
				m_bModified = true;
				GetWindowText( m_strRedoBuffer );
				SetWindowText( m_strUndoBuffer );
			}

			GetWindowText( m_strWindowText );
			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );
		}

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to select all text in the mask edit
	//     control.
	//-----------------------------------------------------------------------
	void MaskSelectAll()
	{
		if ( !CanUseMask( ))
		{
			CEdit::SetSel(0, -1);
		}
		else
		{
			m_nStartChar = 0;
			CorrectPosition( m_nStartChar );
			SetSel(m_nStartChar, -1);
		}
	}

	//-----------------------------------------------------------------------}
	// Summary:
	//     This member function is called to determine if the text has been modified.
	// Returns:
	//     true if the text has changed, otherwise returns false.
	//-----------------------------------------------------------------------
	bool IsModified() {
		return m_bModified;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method called to set masked text for the edit control.
	// Parameters:
	//     lpszMaskedText - Text string without mask.
	// Remarks:
	//     If a mask is used, then the mask will be applied to the text
	//     in lpszMaskedText.
	//-----------------------------------------------------------------------
	void SetMaskedText(LPCTSTR lpszMaskedText)
	{
		CString strMaskedText = lpszMaskedText;
		int nLen = strMaskedText.GetLength();
		int x = 0;

		m_strWindowText = m_strLiteral;

		for (int i = 0; (i < m_strWindowText.GetLength()) && (x < nLen); i++)
		{
			if (strMaskedText[x] == m_strWindowText[i])
			{
				x ++;
			} else if (m_strWindowText[i] == m_chPrompt)
			{
				m_strWindowText.SetAt(i, strMaskedText[x]);
				x ++;
			}
		}

		// set the window text for the control.
		m_bRedo = false;
		m_bModified = false;
		SetWindowText( m_strWindowText );

		m_strUndoBuffer = m_strWindowText;

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the mask for the edit control.
	// Parameters:
	//     lpszMask    - The format for the mask field. For example, if you wanted to set
	//                   the mask for a phone number, and you only wanted digits to be entered,
	//                   your mask might look like this; _T("(000) 000-0000").
	//     lpszLiteral - The literal format is entered here. Wherever you place an underscore
	//                   ('_') is where the user will be allowed to enter data only. Using
	//                   the phone number example; _T("(___) ___-____").
	//     lpszDefault - Text that is to be displayed when the control is initialized. For
	//                   example; _T("(800) 555-1212"). If NULL, 'lpszLiteral' is used to initialize
	//                   the edit text.
	// Remarks:
	//     The values that can be set are:
	//     <TABLE>
	//          <b>Mask Character</b>  <b>Description</b>
	//          ---------------------  ------------------------
	//          0                      Numeric (0-9)
	//          9                      Numeric (0-9) or space (' ')
	//          #                      Numeric (0-9) or space (' ') or ('+') or ('-')
	//          L                      Alpha (a-Z)
	//          ?                      Alpha (a-Z) or space (' ')
	//          A                      Alpha numeric (0-9 and a-Z)
	//          a                      Alpha numeric (0-9 and a-Z) or space (' ')
	//          &                      All print character only
	//          H                      Hex digit (0-9 and A-F)
	//          X                      Hex digit (0-9 and A-F) and space (' ')
	//          >                      Forces characters to upper case (A-Z)
	//          <                      Forces characters to lower case (a-z)
	//     </TABLE>
	//-----------------------------------------------------------------------
	virtual void SetEditMask(LPCTSTR lpszMask,LPCTSTR lpszLiteral,LPCTSTR lpszDefault=NULL)
	{
		ASSERT(lpszMask);
		ASSERT(lpszLiteral);

		// initialize the mask for the control.
		m_strMask    = lpszMask;
		m_strLiteral = lpszLiteral;

		ASSERT(m_strMask.GetLength() == m_strLiteral.GetLength());

		if ( lpszDefault == NULL )
		{
			m_strWindowText = lpszLiteral;
			m_strDefault    = lpszLiteral;
		}
		else
		{
			SetMaskedText(lpszDefault);
			m_strDefault    = m_strWindowText;
		}

		ASSERT(m_strWindowText.GetLength() == m_strLiteral.GetLength());

		// set the window text for the control.
		m_bRedo = false;
		m_bModified = false;
		SetWindowText( m_strWindowText );

		m_strUndoBuffer = m_strWindowText;

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Converts character to Upper/Lower case.
	// Parameters:
	//     nChar - Char to be converted
	//     bUpperCase  - TRUE to convert to upper case
	// Returns:
	//     Converted character.
	//-----------------------------------------------------------------------
	TCHAR ConvertUnicodeAlpha(TCHAR nChar, BOOL bUpperCase)
	{
		CString strTemp(nChar);
		if (bUpperCase) strTemp.MakeUpper(); else strTemp.MakeLower();
		return strTemp[0];
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This method determines if nChar is alpha character
	// Parameters:
	//     nChar - Character need to test
	// Returns:
	//     true if nChar is alpha character.
	//-----------------------------------------------------------------------
	bool IsUnicodeAlpha(TCHAR nChar)
	{
		if (ConvertUnicodeAlpha(nChar, TRUE) != nChar)
			return TRUE;

		if (ConvertUnicodeAlpha(nChar, FALSE) != nChar)
			return TRUE;

		return FALSE;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used internally to validate the character indicated
	//     by 'nChar'.
	// Parameters:
	//     nChar - Contains the character code value of the key.
	//     bBeep - true to enable beep. false to disable beep.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool CheckChar(UINT& nChar,bool bBeep=true)
	{
		// do not use mask
		if ( !CanUseMask( ) )
		{
			return false;
		}

		// control character, OK
		if ( !_istprint( ( TCHAR )nChar ) && !IsUnicodeAlpha(( TCHAR )nChar))
		{
			return true;
		}

		// advance to the first character input position.
		int i;
		for ( i = m_nStartChar; i < m_strLiteral.GetLength( ); ++i )
		{
			if ( m_strLiteral.GetAt( i ) == m_chPrompt )
			{
				SetSel( i, i );
				break;
			}
		}

		// make sure the string is not longer than the mask
		if ( i >= m_strMask.GetLength( ) )
		{
			if ( bBeep )
			{
				:: MessageBeep( ( UINT )-1 );
			}
			return false;
		}

		if ( !ProcessMask( nChar, i ) )
		{
			if ( bBeep )
			{
				:: MessageBeep( ( UINT )-1 );
			}
			return false;
		}

		return true;

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is used internally to process the character passed
	//     in by 'nChar' whose index is specified by 'nEndPos'.
	// Parameters:
	//     nChar - Contains the character code value of the key.
	//     nEndPos - Index of character in display string.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool ProcessMask(UINT& nChar,int nEndPos)
	{
		// check the key against the mask
		switch ( m_strMask.GetAt( nEndPos ) )
		{
		case '0':       // digit only //completely changed this
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case '9':       // digit or space
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case '#':       // digit or space or '+' or '-'
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' || nChar == '-' || nChar == '+' ) {
					return true;
				}
				break;
			}
		case 'd':       // decimal
			{
				if ( _istdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' || nChar == '-' || nChar == '+' || nChar == '.' || nChar == ',') {
					return true;
				}
				break;
			}
		case 'L':       // alpha only
			{
				if ( _istalpha( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar) ) {
					return true;
				}
				break;
			}
		case '?':       // alpha or space
			{
				if ( _istalpha( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case 'A':       // alpha numeric only
			{
				if ( _istalnum( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar)) {
					return true;
				}
				break;
			}
		case 'a':       // alpha numeric or space
			{
				if ( _istalnum( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar)) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case '&':       // all print character only
			{
				if ( _istprint( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar) ) {
					return true;
				}
				break;
			}
		case 'H':       // hex digit
			{
				if ( _istxdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				break;
			}
		case 'X':       // hex digit or space
			{
				if ( _istxdigit( ( TCHAR )nChar ) ) {
					return true;
				}
				if ( nChar == ' ' ) {
					return true;
				}
				break;
			}
		case '>':
			{
				if ( _istalpha( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar))
				{
					nChar = ConvertUnicodeAlpha( ( TCHAR )nChar, TRUE);
					return true;
				}
				break;
			}
		case '<':
			{
				if ( _istalpha( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar))
				{
					nChar = ConvertUnicodeAlpha( ( TCHAR )nChar, FALSE);
					return true;
				}
				break;
			}
		}

		return false;
	}

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Used by class CWinApp to translate window messages before they are dispatched to theTranslateMessage andDispatchMessage Windows functions.
	// Parameters:
	//     pMsg - Points to a MSG structure that contains the message to process.
	// Returns:
	//     Nonzero if the message was translated and should not be dispatched; 0 if the message was not translated and should be dispatched.
	//-----------------------------------------------------------------------
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (!CanUseMask())
		{
			return TBase::PreTranslateMessage(pMsg);
		}

		// intercept Ctrl+C (copy), Ctrl+V (paste), Ctrl+X (cut) and Ctrl+Z (undo)
		// before CEdit base class gets a hold of them.

		if ( pMsg->message == WM_KEYDOWN )
		{
			if ( ::GetKeyState( VK_SUBTRACT ) < 0 )
			{
				OnChar( '-', 1, 1 );
				return TRUE;
			}

			if ( ::GetKeyState( VK_ADD ) < 0 )
			{
				OnChar( '+', 1, 1 );
				return TRUE;
			}

			if ( ::GetKeyState( VK_CONTROL ) < 0 )
			{
				switch ( pMsg->wParam )
				{
				case 'X':
				case 'x':
					{
						MaskCut( );
						return TRUE;
					}

				case 'C':
				case 'c':
					{
						MaskCopy( );
						return TRUE;
					}

				case 'V':
				case 'v':
					{
						MaskPaste( );
						return TRUE;
					}

				case 'Z':
				case 'z':
					{
						MaskUndo( );
						return TRUE;
					}
				}
			}
			else
			{
				switch ( pMsg->wParam )
				{
				case '-':
					{
						OnChar( '-', 1, 1 );
						return TRUE;
					}

				case '+':
					{
						OnChar( '+', 1, 1 );
						return TRUE;
					}
				}
			}
		}

		return TBase::PreTranslateMessage(pMsg);

	}

	//{{AFX_CODEJOCK_PRIVATE
	virtual bool CorrectPosition(int& iPos, bool bForward=true, bool bBeep=true) // used internally
	{
		GetWindowText( m_strWindowText );
		int iLen = m_strWindowText.GetLength( );

		if ( iPos == iLen )
		{
			if ( bBeep )
			{
				::MessageBeep( ( UINT )-1 );
			}
			return false;
		}

		if (! PosInRange(iPos))
			return false;

		if ( m_strLiteral.GetAt( iPos ) != m_chPrompt )
		{
			int i;
			if ( bForward )
			{
				for ( i = iPos; i < iLen; ++i )
				{
					if ( m_strLiteral.GetAt( i ) == m_chPrompt )
					{
						iPos = i;
						break;
					}
				}
			}
			else
			{
				for ( i = iPos; i >= 0; --i )
				{
					if ( m_strLiteral.GetAt( i ) == m_chPrompt )
					{
						iPos = i;
						break;
					}
				}

				if ( i == -1 )
				{
					iPos++;

					if ( bBeep )
					{
						::MessageBeep( ( UINT )-1 );
					}
					return false;
				}
			}

			if ( i == iLen )
			{
				if ( bBeep )
				{
					::MessageBeep( ( UINT )-1 );
				}
				return false;
			}
		}
		return true;

	}
	//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Deletes character in specified position
	// Parameters:
	//     iPos - Position for character to be deleted.
	//-----------------------------------------------------------------------
	virtual void DeleteCharAt(int iPos)
	{
		if (!PosInRange(iPos))
			return;

		CString strBuffer = GetMaskedText( iPos+1 );
		strBuffer += m_chPrompt;

		int x = 0;
		int iLen = strBuffer.GetLength( );
		int i;
		for (i = iPos; i < m_strLiteral.GetLength( ); ++i )
		{
			if ( ( m_strLiteral.GetAt( i ) == m_chPrompt ) )
			{
				m_strWindowText.SetAt( i, strBuffer.GetAt( x ) );
				++x;

				if ( x == iLen )
					break;
			}
		}


	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Inserts character to specified position.
	// Parameters:
	//     iPos - Position to insert
	//     nChar - Character to be inserted
	//-----------------------------------------------------------------------
	virtual void InsertCharAt(int iPos, TCHAR nChar)
	{
		if (!PosInRange(iPos))
		{
			return;
		}

		UINT uChar = nChar;
		if ( !CheckChar( uChar, false ) )
		{
			return;
		}

		CString strBuffer = GetMaskedText( iPos );

		CString strTemp(nChar);
		int i, x = 0, iLen = strBuffer.GetLength( );
		for ( i = 1; i < strBuffer.GetLength(); ++i )
		{
			strTemp += strBuffer.GetAt( i-1 );
		}
		strBuffer = strTemp;

		for ( i = iPos; i < m_strWindowText.GetLength( ); ++i )
		{
			if ( m_strLiteral.GetAt( i ) == m_chPrompt )
			{
				m_strWindowText.SetAt( i, strBuffer[ x ] );
				++x;

				if ( x == iLen )
					break;
			}
		}
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Copies text to system clipboard
	// Parameters:
	//     lpszText - Text to be copied
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual bool CopyToClipboard(LPCTSTR lpszText)
	{
		if ( !OpenClipboard( ) )
			return false;

		::EmptyClipboard( );

		int iLen = (int)_tcslen( lpszText ) + 1;

		HGLOBAL hglbCopy = ::GlobalAlloc( GMEM_MOVEABLE, iLen );

		if ( hglbCopy == NULL )
		{
			::CloseClipboard( );
			return false;
		}

		LPTSTR lptstrCopy = ( TCHAR* )::GlobalLock( hglbCopy );
		MEMCPY_S(lptstrCopy, lpszText, iLen);
		::GlobalUnlock( hglbCopy );

		::SetClipboardData( CF_TEXT, hglbCopy );

		if ( !::CloseClipboard( ) )
			return false;

		return true;

	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Retrieves masked text of the control
	// Parameters:
	//     iPos - Start position
	// Returns:
	//     Masked text of the control.
	//-----------------------------------------------------------------------
	virtual CString GetMaskedText(int iPos = 0)
	{
		int iWndLen = m_strWindowText.GetLength( );

		ASSERT( iWndLen == m_strLiteral.GetLength( ) ); // must be equal in length

		CString strBuffer;
		int i;
		for ( i = iPos; i < iWndLen; ++i )
		{
			if ( m_strLiteral.GetAt( i ) == m_chPrompt )
			{
				strBuffer += m_strWindowText.GetAt( i );
			}
		}

		return strBuffer;

	}


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to determine if a selection has been made
	// Returns:
	//     TRUE if successful; otherwise returns FALSE
	//-----------------------------------------------------------------------
	virtual BOOL SelectionMade() {
		GetSel( m_nStartChar, m_nEndChar ); return ( m_nStartChar != m_nEndChar );
	}


protected:


//{{AFX_CODEJOCK_PRIVATE
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (!CanUseMask())
		{
			TBase::OnKeyDown( nChar, nRepCnt, nFlags ); // default processing.
			return;
		}

		GetWindowText( m_strWindowText ); // refresh
		GetSel( m_nStartChar, m_nEndChar );

		if ( m_strMask.IsEmpty( ) )
		{
			TBase::OnKeyDown( nChar, nRepCnt, nFlags ); // default processing.
			return;
		}

		bool bShift = (::GetKeyState(VK_SHIFT) < 0);

		switch ( nChar )
		{
		case VK_UP:
		case VK_LEFT:
			{
				TBase::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );

				int iStartChar = m_nStartChar;
				CorrectPosition( iStartChar, false, false );

				if ( m_nStartChar < iStartChar )
				{
					m_nStartChar = iStartChar;

					if ( !bShift )
						m_nEndChar = iStartChar;

					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;

		case VK_DOWN:
		case VK_RIGHT:
			{
				TBase::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );

				int iEndChar = m_nEndChar;
				CorrectPosition( iEndChar, true, false );

				if ( m_nEndChar > iEndChar )
				{
					m_nEndChar = iEndChar;

					if ( !bShift )
						m_nStartChar = iEndChar;

					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;

		case VK_HOME:
			{
				TBase::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );

				int iStartChar = m_nStartChar;
				CorrectPosition( iStartChar, true, false );

				if ( m_nStartChar < iStartChar )
				{
					m_nStartChar = iStartChar;

					if (!bShift)
					{
						m_nEndChar = m_nStartChar;
					}
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;

		case VK_END:
			{
				TBase::OnKeyDown( nChar, nRepCnt, nFlags );
				GetSel( m_nStartChar, m_nEndChar );

				int iEndChar = m_nEndChar;
				CorrectPosition( iEndChar, true, false );

				if ( m_nEndChar > iEndChar )
				{
					m_nEndChar = iEndChar;

					if ( !bShift )
						m_nStartChar = iEndChar;

					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			return;

		case VK_INSERT:
			{
				if ( bShift )
				{
					MaskPaste( );
				}
				else
				{
					m_bOverType = !m_bOverType; // set the type-over flag
				}
			}
			return;

		case VK_DELETE:
			{
				if ( m_nStartChar == m_nEndChar )
				{
					if ( CorrectPosition( m_nStartChar ) )
					{
						DeleteCharAt( m_nStartChar );

						m_bRedo = false;
						m_bModified = true;
						SetWindowText( m_strWindowText );

						m_nEndChar = m_nStartChar;
						SetSel( m_nStartChar, m_nEndChar );
					}
				}
				else
				{
					MaskClear( );
				}
			}
			return;

		case VK_SPACE:
			{
				GetWindowText( m_strWindowText ); // refresh

				if ( CorrectPosition( m_nStartChar ) )
				{
					if ( m_nStartChar < m_strLiteral.GetLength( ) )
					{
						if ( ProcessMask( nChar, m_nStartChar ) )
						{
							if ( CanOverType( ) )
							{
								m_strWindowText.SetAt( m_nStartChar, ' ' );
							}
							else
							{
								InsertCharAt( m_nStartChar, ( TCHAR )nChar );
							}
						}
						else
						{
							m_strWindowText.SetAt( m_nStartChar, m_chPrompt );
						}

						m_bRedo = false;
						m_bModified = true;
						SetWindowText( m_strWindowText );

						m_nStartChar++;
						m_nEndChar = m_nStartChar;
						SetSel( m_nStartChar, m_nEndChar );
					}
					else
					{
						::MessageBeep( ( UINT )-1 );
					}
				}
			}
			return;

		case VK_BACK:
			{
				GetWindowText( m_strWindowText ); // refresh

				if ( ( m_nStartChar > 0 ) &&
					( m_nStartChar <= m_strLiteral.GetLength( ) ) )
				{
					m_nStartChar--;

					if ( CorrectPosition( m_nStartChar, false ) )
					{
						TCHAR ch = m_chPrompt;

						// get the masked literal representation.
						if ( !m_strDefault.IsEmpty( ) )
						{
							ch = m_strDefault.GetAt( m_nStartChar );
						}

						m_strWindowText.SetAt( m_nStartChar, ch );

						m_bRedo = false;
						m_bModified = true;
						SetWindowText( m_strWindowText );

						m_nEndChar = m_nStartChar;
						SetSel( m_nStartChar, m_nEndChar );
					}
				}
				else
				{
					::MessageBeep( ( UINT )-1 );
				}
			}
			return;
		}

		TBase::OnKeyDown( nChar, nRepCnt, nFlags );

	}
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (!CanUseMask())
		{
			TBase::OnChar(nChar, nRepCnt, nFlags);
			return;
		}

		switch ( nChar )
		{
		case VK_UP:
		case VK_LEFT:
		case VK_DOWN:
		case VK_RIGHT:
		case VK_HOME:
		case VK_END:
		case VK_DELETE:
		case VK_SPACE:
		case VK_BACK:
			return; // handled in WM_KEYDOWN
		}

		if (!CheckChar(nChar))
		{
			return;
		}

		if ( CanUseMask( ) )
		{
			if ( _istprint( ( TCHAR )nChar ) || IsUnicodeAlpha(( TCHAR )nChar))
			{
				int iLen = m_strLiteral.GetLength( );
				if ( m_nStartChar >= iLen )
				{
					::MessageBeep( ( UINT )-1 );
					return;
				}

				if ( m_nEndChar >= iLen )
					m_nEndChar = iLen-1;

				if ( m_nStartChar != m_nEndChar )
				{
					int i;
					for ( i = m_nStartChar; i < m_nEndChar; ++i )
					{
						if ( m_strLiteral.GetAt( i ) == m_chPrompt )
						{
							DeleteCharAt( m_nStartChar );
						}
					}
				}

				SetWindowText( m_strWindowText );

				if ( CorrectPosition( m_nStartChar ) )
				{
					if ( CanOverType( ) )
					{
						if ( m_nStartChar != m_nEndChar )
						{
							InsertCharAt( m_nStartChar, ( TCHAR )nChar );
						}
						else
						{
							m_strWindowText.SetAt( m_nStartChar, ( TCHAR )nChar );
						}
					}
					else
					{
						InsertCharAt( m_nStartChar, ( TCHAR )nChar );
					}
				}

				m_bRedo = false;
				m_bModified = true;
				SetWindowText( m_strWindowText );

				if (m_nStartChar < iLen )
					m_nStartChar++;

				if ( m_nStartChar < iLen )
					CorrectPosition( m_nStartChar );

				m_nEndChar = m_nStartChar;
				SetSel( m_nStartChar, m_nEndChar );

				return;
			}
		}

		if (nChar != 127)
			TBase::OnChar(nChar, nRepCnt, nFlags);

		// update the window text string.
		GetWindowText( m_strWindowText );

	}
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
	{
		TBase::OnLButtonDown(nFlags, point);

		if (!CanUseMask())
		{
			return;
		}

		GetSel( m_nStartChar, m_nEndChar );
		CorrectPosition( m_nStartChar, true, false );

		if ( ::GetKeyState( VK_SHIFT ) < 0 )
			m_nEndChar = m_nStartChar;

		SetSel( m_nStartChar, m_nEndChar );

	}
	afx_msg void OnSetFocus(CWnd* pOldWnd)
	{
		TBase::OnSetFocus(pOldWnd);

		if (!CanUseMask())
		{
			return;
		}

		GetSel( m_nStartChar, m_nEndChar );
		CorrectPosition( m_nStartChar, true, false );

		m_nEndChar = m_nStartChar;
		SetSel( m_nStartChar, m_nEndChar );

	}
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		TBase::OnLButtonDblClk(nFlags, point);

		if (!CanUseMask())
		{
			return;
		}

		GetSel( m_nStartChar, m_nEndChar );
		CorrectPosition( m_nStartChar, true, false );
		CorrectPosition( m_nEndChar, true, false );

		SetSel( m_nStartChar, m_nEndChar );

	}
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI)
	{
		if (!CanUseMask())
		{
			pCmdUI->Enable( CanUndo( ) );
		}
		else
		{
			pCmdUI->Enable( m_bModified );
		}

	}
//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     The framework calls this member function when the user selects an item from a menu
	// Parameters:
	//     wParam - The low-order word of wParam identifies the command ID of the menu item, control, or accelerator. The high-order word of wParam specifies the notification message if the message is from a control. If the message is from an accelerator, the high-order word is 1. If the message is from a menu, the high-order word is 0
	//     lParam - Identifies the control that sends the message if the message is from a control. Otherwise, lParam is 0.
	// Returns:
	//     An application returns nonzero if it processes this message; otherwise 0.
	//-----------------------------------------------------------------------
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		switch (LOWORD(wParam))
		{
			case ID_EDIT_CUT:
				if ( !CanUseMask( )) CEdit::Cut(); else MaskCut( );
				return TRUE;

			case ID_EDIT_COPY:
				if ( !CanUseMask( )) CEdit::Copy(); else MaskCopy( );
				return TRUE;

			case ID_EDIT_PASTE:
				if ( !CanUseMask()) CEdit::Paste(); else MaskPaste( );
				return TRUE;

			case ID_EDIT_CLEAR:
				if ( !CanUseMask()) CEdit::Clear(); else MaskClear( );
				return TRUE;

			case ID_EDIT_UNDO:
				if ( !CanUseMask()) CEdit::Undo(); else MaskUndo( );
				return TRUE;

			case ID_EDIT_SELECT_ALL:
				MaskSelectAll( );
				return TRUE;
		}
		return TBase::OnCommand(wParam, lParam);
	}

protected:

	int         m_nStartChar;       // Current position of the first character in the current selection.
	int         m_nEndChar;         // Current position of the first non-selected character past the end of the current selection.
	bool        m_bUseMask;         // true to use the edit mask.
	bool        m_bOverType;        // true to over type the text, set with VK_INSERT key press.
	bool        m_bRedo;            // true to redo, or false to undo.
	bool        m_bModified;        // true if mask edit has been modified.
	TCHAR       m_chPrompt;         // Prompt character used to identify the text entry.
	CString     m_strMask;          // Buffer that holds the actual edit mask value.
	CString     m_strDefault;       // Contains the edit controls default display text.
	CString     m_strUndoBuffer;    // Holds the contents of the undo buffer.
	CString     m_strRedoBuffer;    // Holds the contents of the redo buffer.
	CString     m_strWindowText;    // Buffer that holds the actual edit text.
	CString     m_strLiteral;       // Literal format that restricts where the user can enter text.

};

//{{AFX_CODEJOCK_PRIVATE
#define ON_MASKEDIT_REFLECT\
	ON_MESSAGE_VOID(WM_CUT, MaskCut)\
	ON_MESSAGE_VOID(WM_PASTE, MaskPaste)\
	ON_MESSAGE_VOID(WM_CLEAR, MaskClear)\
	ON_MESSAGE_VOID(WM_UNDO, MaskUndo)\
	ON_MESSAGE_VOID(WM_COPY, MaskCopy)\
	ON_WM_KEYDOWN()\
	ON_WM_CHAR()\
	ON_WM_LBUTTONDOWN()\
	ON_WM_SETFOCUS()\
	ON_WM_LBUTTONDBLCLK
//}}AFX_CODEJOCK_PRIVATE
//////////////////////////////////////////////////////////////////////


#endif // #if !defined(__XTMASKEDITEX_H__)
