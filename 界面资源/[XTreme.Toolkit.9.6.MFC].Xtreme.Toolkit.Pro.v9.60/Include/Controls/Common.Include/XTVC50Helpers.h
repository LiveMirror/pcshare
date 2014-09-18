// XTVC50Helpers.h : Visual C++ 5.0 helpers
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
#if !defined(__XTVC50HELPERS_H__)
#define __XTVC50HELPERS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// ---------------------------------------------------------------------
// Summary:
//     The CXTListViewHelper class is a helper class that provides
//     helper functions for finding selected items for the MFC
//     CListView class.
// See Also:
//     GetFirstSelectedItemPosition, GetNextSelectedItem
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTListViewHelper : public CListView
{
public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Gets the position of the first selected item in the list view
	//     control.
	// Returns:
	//     A POSITION value that can be used for iteration or object
	//     pointer retrieval; NULL if no items are selected.
	// Example:
	//     The following code sample demonstrates the usage of this function.
	// <code>
	// CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_YOURLISTCONTROL);
	// ASSERT(pListCtrl != NULL);
	//
	// POSITION pos = pList-\>GetFirstSelectedItemPosition();
	// if (pos == NULL)
	//     TRACE0("No items were selected!\\n");
	// else
	// {
	//     while (pos)
	//     {
	//         int nItem = pList-\>GetNextSelectedItem(pos);
	//         TRACE1("Item %d was selected!\\n", nItem);
	//         // you could do your own processing on nItem here
	//     }
	// }
	// </code>
	// See Also:
	//     GetNextSelectedItem
	// ------------------------------------------------------------------------
	POSITION GetFirstSelectedItemPosition() const;

	// ---------------------------------------------------------------------------------------------
	// Summary:
	//     Gets the index of the list item identified by pos, then sets pos
	//     to the POSITION value.
	// Parameters:
	//     pos -  A reference to a POSITION value returned by a previous
	//            call to GetNextSelectedItem or GetFirstSelectedItemPosition.
	//            The value is updated to the next position by this call.
	// Returns:
	//     The index of the list item identified by <i>pos</i>.
	// Remarks:
	//     You can use GetNextSelectedItem in a forward iteration loop
	//     if you establish the initial position with a call to GetFirstSelectedItemPosition.
	//
	// You must ensure that your POSITION value is valid. If it is
	//     invalid, then the Debug version of the Microsoft Foundation Class
	//     Library asserts.
	// Example:
	//     The following code sample demonstrates the usage of this function.
	// <code>
	// CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_YOURLISTCONTROL);
	// ASSERT(pListCtrl != NULL);
	//
	// POSITION pos = pList-\>GetFirstSelectedItemPosition();
	// if (pos == NULL)
	//     TRACE0("No items were selected!\\n");
	// else
	// {
	//     while (pos)
	//     {
	//         int nItem = pList-\>GetNextSelectedItem(pos);
	//         TRACE1("Item %d was selected!\\n", nItem);
	//         // you could do your own processing on nItem here
	//     }
	// }
	// </code>
	// See Also:
	//     GetFirstSelectedItemPosition
	// ---------------------------------------------------------------------------------------------
	int GetNextSelectedItem(POSITION& pos) const;
};


//{{AFX_CODEJOCK_PRIVATE
#if (_MSC_VER >= 1200) // Using Visual C++ 6.0 or later.
//}}AFX_CODEJOCK_PRIVATE


// ---------------------------------------------------------------------
// Summary:
//     Helper macro used for compatibility with Visual C++ 5.0, Visual C++ 6.0
//     and later should use CString.
// ---------------------------------------------------------------------
#define CXTStringHelper CString

// ---------------------------------------------------------------------
// Summary:
//     Helper macro used for compatibility with Visual C++ 5.0, Visual C++ 6.0
//     and later should use CHeaderCtrl.
// ---------------------------------------------------------------------
#define CXTHeaderCtrlHelper CHeaderCtrl

// ---------------------------------------------------------------------
// Summary:
//     Helper macro used for compatibility with Visual C++ 5.0, Visual C++ 6.0
//     and later should use CListCtrl.
// ---------------------------------------------------------------------
#define CXTListCtrlHelper CListCtrl


//{{AFX_CODEJOCK_PRIVATE
#else  // Using Visual C++ 5.0

// ------------------------------------------------------------------------
// Summary:
//     The CXTHeaderCtrlHelper class is a helper class that is used
//     with Visual C++ 5.0 to provide compatibility with later versions
//     \of Visual C++. For Visual C++ 6.0 and later CXTHeaderCtrlHelper
//     is defined as CHeaderCtrl.
// ------------------------------------------------------------------------
class _XT_EXT_CLASS CXTHeaderCtrlHelper : public CHeaderCtrl
{
protected:
	// ----------------------------------------------------------------
	// Summary:
	//     Retrieves a count of the items in a header control.
	// Returns:
	//     Number of header control items if successful; otherwise – 1.
	// ----------------------------------------------------------------
	virtual int GetItemCount() const;

	// ---------------------------------------------------------------------------
	// Summary:
	//     Retrieves the left-to-right order of items in a header control.
	// Parameters:
	//     piArray -  A pointer to the address of a buffer that receives the index
	//                values of the items in the header control, in the order in
	//                which they appear from left to right.
	//     iCount -   A pointer to the address of a buffer that receives the index
	//                values of the items in the header control, in the order in
	//                which they appear from left to right.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// Remarks:
	//     This member function implements the behavior of the Win32 message
	//     HDM_GETORDERARRAY, as described in the Platform SDK. It is
	//     provided to support header item ordering.
	// Example:
	// <code>
	// // The pointer to my header control.
	// extern CHeaderCtrl* pmyHeaderCtrl;
	//
	// // Reverse the order of the items in the header control.
	// // (i.e. make the first item the last one, the last item
	// // the first one, and so on ...).
	// int   nCount = pmyHeaderCtrl-\>GetItemCount();
	// LPINT pnOrder = (LPINT) malloc(nCount*sizeof(int));
	// ASSERT(pnOrder != NULL);
	//
	// pmyHeaderCtrl-\>GetOrderArray(pnOrder, nCount);
	//
	// int i, j, nTemp;
	// for (i=0,j=nCount-1;i \< j;i++,j--)
	// {
	//     nTemp = pnOrder[i];
	//     pnOrder[i] = pnOrder[j];
	//     pnOrder[j] = nTemp;
	// }
	//
	// pmyHeaderCtrl-\>SetOrderArray(nCount, pnOrder);
	// free(pnOrder);
	// </code>
	// ---------------------------------------------------------------------------
	virtual BOOL GetOrderArray(LPINT piArray,int iCount=-1);

	// -----------------------------------------------------------------------
	// Summary:
	//     Number of header control items if successful; otherwise – 1.
	// Parameters:
	//     nIndex -  Number of header control items if successful; otherwise –
	//               1.
	//     lpRect -  Number of header control items if successful; otherwise –
	//               1.
	// Returns:
	//     Nonzero if successful; otherwise 0.
	// Remarks:
	//     This member function implements the behavior of the Win32 message
	//     HDM_GETITEMRECT, as described in the Platform SDK.
	// -----------------------------------------------------------------------
	virtual BOOL GetItemRect(int nIndex,LPRECT lpRect) const;
};

// ----------------------------------------------------------------------
// Summary:
//     The CXTListCtrlHelper class is a helper class that is used
//     with Visual C++ 5.0 to provide compatibility with later versions
//     \of Visual C++. For Visual C++ 6.0 and later CXTListCtrlHelper
//     is defined as CListCtrl.
// ----------------------------------------------------------------------
class _XT_EXT_CLASS CXTListCtrlHelper : public CListCtrl
{
public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Gets the position of the first selected item in the list view
	//     control.
	// Returns:
	//     A POSITION value that can be used for iteration or object
	//     pointer retrieval; NULL if no items are selected.
	// Example:
	//     The following code sample demonstrates the usage of this function.
	// <code>
	//     CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_YOURLISTCONTROL);
	// ASSERT(pListCtrl != NULL);
	//
	// POSITION pos = pList-\>GetFirstSelectedItemPosition();
	// if (pos == NULL)
	// TRACE0("No items were selected!\\n");
	// else
	// {
	//     while (pos)
	//     {
	//         int nItem = pList-\>GetNextSelectedItem(pos);
	//         TRACE1("Item %d was selected!\\n", nItem);
	//         // you could do your own processing on nItem here
	//     }
	// }
	//     </code>
	// See Also:
	//     GetNextSelectedItem
	// ------------------------------------------------------------------------
	POSITION GetFirstSelectedItemPosition() const;

	// ---------------------------------------------------------------------------------------------
	// Summary:
	//     Gets the index of the list item identified by pos, then sets pos
	//     to the POSITION value.
	// Parameters:
	//     pos -  A reference to a POSITION value returned by a previous
	//            call to GetNextSelectedItem or GetFirstSelectedItemPosition.
	//            The value is updated to the next position by this call.
	// Returns:
	//     The index of the list item identified by <i>pos</i>.
	// Remarks:
	//     You can use GetNextSelectedItem in a forward iteration loop
	//     if you establish the initial position with a call to GetFirstSelectedItemPosition.
	//
	// You must ensure that your POSITION value is valid. If it is
	//     invalid, then the Debug version of the Microsoft Foundation Class
	//     Library asserts.
	// Example:
	//     The following code sample demonstrates the usage of this function.
	// <code>
	// CListCtrl* pListCtrl = (CListCtrl*) GetDlgItem(IDC_YOURLISTCONTROL);
	// ASSERT(pListCtrl != NULL);
	//
	// POSITION pos = pList-\>GetFirstSelectedItemPosition();
	// if (pos == NULL)
	// TRACE0("No items were selected!\\n");
	// else
	// {
	//     while (pos)
	//     {
	//         int nItem = pList-\>GetNextSelectedItem(pos);
	//         TRACE1("Item %d was selected!\\n", nItem);
	//         // you could do your own processing on nItem here
	//     }
	// }
	// </code>
	// See Also:
	//     GetFirstSelectedItemPosition
	// ---------------------------------------------------------------------------------------------
	int GetNextSelectedItem(POSITION& pos) const;
};

// ---------------------------------------------------------------------
// Summary:
//     The CXTStringHelper class is a helper class that is used with
//     Visual C++ 5.0 to provide compatibility with later versions of
//     Visual C++. For Visual C++ 6.0 and later CXTStringHelper is
//     defined as CString.
// ---------------------------------------------------------------------
class _XT_EXT_CLASS CXTStringHelper : public CString
{
public:
	// -------------------------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTStringHelper object.
	// Parameters:
	//     strIn -  An existing CXTStringHelper object to be copied into this
	//              CXTStringHelper object.
	// Remarks:
	//     Because the constructors copy the input data into new allocated
	//     storage, you should be aware that memory exceptions may result.
	//     \Note that some of these constructors act as conversion functions.
	//     This allows you to substitute, for example, an LPTSTR where
	//     a CXTStringHelper object is expected.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::CXTStringHelper.
	// <code>
	// CString s1;                    // Empty string
	// CString s2( "cat" );           // From a C string literal
	// CString s3 = s2;               // Copy constructor
	// CString s4( s2 + " " + s3 );   // From a string expression
	// </code>
	// -------------------------------------------------------------------------------------------
	CXTStringHelper();
	CXTStringHelper(CString strIn); //<COMBINE CXTStringHelper::CXTStringHelper>

	// ----------------------------------------------------------------------------
	// Summary:
	//     Searches this string for the first match of a character or
	//     substring.
	// Parameters:
	//     lpszSub -  A substring to search for..
	//     nStart -   The index of the character in the string to begin the
	//                search with, or 0 to start from the beginning.
	//     ch -       A single character to search for.
	// Returns:
	//     The zero-based index of the first character in this CXTStringHelper
	//     \object that matches the requested substring or characters; -1 if
	//     the substring or character is not found.
	// Remarks:
	//     The function is overloaded to accept both single characters
	//     (similar to the run-time function strchr) and strings
	//     (similar to strstr).
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::Find.
	// <code>
	// CXTStringHelper s( "abcdef" );
	// _ASSERT( s.Find( 'c' ) == 2 );
	// _ASSERT( s.Find( "de" ) == 3 );
	//
	// CXTStringHelper str("The waves are still");
	// int n = str.Find('e', 5);
	// _ASSERT(n == 7);
	//     </code>
	// ----------------------------------------------------------------------------
	int Find(TCHAR ch) const;
	int Find(LPCTSTR lpszSub) const;             //<COMBINE CXTStringHelper::Find@TCHAR@const>
	int Find(LPCTSTR lpszSub, int nStart) const; //<COMBINE CXTStringHelper::Find@TCHAR@const>

	// --------------------------------------------------------------------------------
	// Summary:
	//     Inserts a single character or a substring at the given index
	//     within the string.
	// Parameters:
	//     nIndex -  The index of the character before which the insertion will
	//               take place.
	//     pstr -    A pointer to the substring to be inserted.
	//     ch -      The character to be inserted.
	// Returns:
	//     The length of the changed string.
	// Remarks:
	//     The <i>nIndex</i> parameter identifies the first character that
	//     will be moved to make room for the character or substring. If <i>nIndex</i>
	//     is zero, the insertion will occur before the entire string. If <i>nIndex</i>
	//     is higher than the length of the string, the function will
	//     concatenate the present string and the new material provided by
	//     either <i>ch</i> or <i>psz</i>.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::Find.
	// <code>
	// CXTStringHelper str("SoccerBest");
	// int n = str.Insert(6, "is ");
	// _ASSERT(n == str.GetLength());
	// printf("1: %sn", (LPCTSTR) str);
	//
	// n = str.Insert(6, ' ');
	// _ASSERT(n == str.GetLength());
	// printf("2: %sn", (LPCTSTR) str);
	//
	// n = str.Insert(55, '!');
	// _ASSERT(n == str.GetLength());
	// printf("3: %sn", (LPCTSTR) str);
	// </code>
	//
	// Output
	//
	// 1: Soccer is Best
	//
	// 2: Soccer is Best
	//
	// 3: Soccer is Best!
	// --------------------------------------------------------------------------------
	int Insert(int nIndex, TCHAR ch);
	int Insert(int nIndex, LPCTSTR pstr); //<COMBINE CXTStringHelper::Insert@int@TCHAR>

	// ---------------------------------------------------------------------------------
	// Summary:
	//     The CXTStringHelper assignment (=) operator reinitialize's an
	//     existing CXTStringHelper object with new data.
	// Parameters:
	//     stringSrc -  A CString to be copied into this CString
	//                  object.
	// ch -         A single character.
	// psz -        A null\-terminated string to be copied into this CXTStringHelper
	//                  object.
	// lpsz -       A null\-terminated string to be copied into this CXTStringHelper
	//                  object.
	// Remarks:
	//     If the destination string (that is, the left side) is already
	//     large enough to store the new data, no new memory allocation is
	//     performed. You should be aware that memory exceptions may occur
	//     whenever you use the assignment operator because new storage is
	//     \often allocated to hold the resulting CXTStringHelper object.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::operator
	//     =.
	// <code>
	//     CXTStringHelper s1, s2;   // Empty CXTStringHelper objects
	//
	// s1 = "cat";            // s1 = "cat"
	// s2 = s1;               // s1 and s2 each = "cat"
	// s1 = "the " + s1;      // Or expressions
	// s1 = 'x';              // Or just individual characters
	//     </code>
	// ---------------------------------------------------------------------------------
	const CString& operator =(TCHAR ch);
	const CString& operator =(LPCSTR lpsz);              //<COMBINE CXTStringHelper::=@TCHAR>
	const CString& operator =(LPCWSTR lpsz);             //<COMBINE CXTStringHelper::=@TCHAR>
	const CString& operator =(const CString& stringSrc); //<COMBINE CXTStringHelper::=@TCHAR>
	const CString& operator =(const unsigned char* psz); //<COMBINE CXTStringHelper::=@TCHAR>

	// ------------------------------------------------------------------------------
	// Summary:
	//     Removes instances of the specified character from the string.
	// Parameters:
	//     chRemove -  The character to be removed from a string.
	// Returns:
	//     The count of characters removed from the string. Zero if the
	//     string is not changed.
	// Remarks:
	//     Comparisons for the character are case sensitive.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::Remove.
	// <code>
	// CXTStringHelper str("This is a test.");
	// int n = str.Remove('t');
	// _ASSERT(n == 2);
	// _ASSERT(str == "This is a es.");
	// </code>
	// ------------------------------------------------------------------------------
	int Remove(TCHAR chRemove);

	// ----------------------------------------------------------------------------------
	// Summary:
	//     Replaces a character or substring with another.
	// Parameters:
	//     lpszOld -  A pointer to a string containing the character to be
	//                replaced by <i>pszNew</i>.
	//     lpszNew -  A pointer to a string containing the character replacing <i>pszOld</i>.
	//     chOld -    The character to be replaced by <i>chNew</i>.
	//     chNew -    The character replacing <i>chOld</i>.
	// Returns:
	//     The number of replaced instances of the character or substring.
	//     Zero if the string is not changed.
	// Remarks:
	//     This function replaces instances of the specified character or
	//     substring with instances of the new character or substring. The
	//     string may grow or shrink as a result of the replacement; that is,
	//     <i>lpszNew</i> and <i>lpszOld</i> do not have to be equal in
	//     length. The function performs a case-sensitive match.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::Replace.
	// <code>
	// CXTStringHelper strBang(_T("Everybody likes field hockey"));
	// int n = strBang.Replace(_T("field hockey"), _T("soccer"));
	// _ASSERT(n == 1);
	// </code>
	// ----------------------------------------------------------------------------------
	int Replace(TCHAR chOld, TCHAR chNew);
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew); //<COMBINE CXTStringHelper::Replace@TCHAR@TCHAR>

	// -------------------------------------------------------------------------------------
	// Summary:
	//     Deletes a character or characters from a string starting with the
	//     character at the given index.
	// Parameters:
	//     nIndex -  The zero\-based index of the first character in the CXTStringHelper
	//               object to delete.
	//     nCount -  The number of characters to be removed.
	// Returns:
	//     The length of the changed string.
	// Remarks:
	//     If nCount is longer than the string, the remainder of the string
	//     will be removed.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::Delete.
	// <code>
	// CXTStringHelper str( "Soccer is best!");
	// printf("Before: %s\\n", (LPCTSTR) str);
	// int n = str.Delete(6, 3);
	// printf("After: %s\\n", (LPCTSTR) str);
	// _ASSERT(n == str.GetLength());
	// </code>
	//
	// Output
	//
	// Before: Soccer is best!
	//
	// After: Soccer best!
	// -------------------------------------------------------------------------------------
	int Delete(int nIndex, int nCount=1);

	// -------------------------------------------------------------------------------
	// Summary:
	//     Extracts the first (that is, leftmost) <i>nCount</i> characters
	//     from this CXTStringHelper object and returns a copy of the
	//     extracted substring.
	// Parameters:
	//     nCount -  The number of characters to extract from this CXTStringHelper
	//               object.
	// Returns:
	//     A CXTStringHelper object containing a copy of the specified
	//     range of characters. Note that the returned CXTStringHelper
	//     \object may be empty.
	// Remarks:
	//     If <i>nCount</i> exceeds the string length, then the entire string
	//     is extracted. Left is similar to the Basic Left function
	//     (except that indexes in Basic are zero based). For multibyte
	//     character sets (MBCS), <i>nCount</i> refers to each 8-bit
	//     character; that is, a lead and trail byte in one multibyte
	//     character are counted as two characters.
	// Example:
	//     The following example demonstrates the use of CXTStringHelper::Left.
	// <code>
	// CXTStringHelper s( _T("abcdef") );
	// _ASSERT( s.Left(2) == _T("ab") );
	// </code>
	// -------------------------------------------------------------------------------
	CXTStringHelper Left(int nCount) const;
};

#ifndef VERSION_WIN4
#define VERSION_WIN4       MAKELONG(0, 4)
#endif

#ifndef VERSION_IE401
#define VERSION_IE401      MAKELONG(72,4)
#endif

#ifndef CBRS_GRIPPER
#define CBRS_GRIPPER       0x00400000L
#endif

#ifndef AFX_IDW_REBAR
#define AFX_IDW_REBAR      0xE804
#endif

#ifndef ID_VIEW_REBAR
#define ID_VIEW_REBAR      0xE804
#endif

#ifndef AFX_IDW_DIALOGBAR
#define AFX_IDW_DIALOGBAR  0xE805
#endif

#ifndef SPI_GETFLATMENU
#define SPI_GETFLATMENU    0x1022
#endif

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif

#ifndef AFX_INLINE
#define AFX_INLINE         inline
#endif

#ifndef AFX_STATIC
#define AFX_STATIC         static
#endif

#endif
//}}AFX_CODEJOCK_PRIVATE




//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTVC50HELPERS_H__)
