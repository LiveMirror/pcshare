// XTTrayIcon.h: interface for the CXTTrayIcon class.
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
#if !defined(__XTTRAYICON_H__)
#define __XTTRAYICON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE
#ifndef NIIF_NONE
#define NIIF_NONE       0x00000000
#define NIIF_INFO       0x00000001
#define NIIF_WARNING    0x00000002
#define NIIF_ERROR      0x00000003
#define NIIF_ICON_MASK  0x0000000F
#endif

#ifndef NIS_HIDDEN
#define NIS_HIDDEN      0x00000001
#endif

#ifndef NIF_STATE
#define NIF_STATE       0x00000008
#define NIF_INFO        0x00000010
#endif

#ifndef NIN_BALLOONSHOW
#define NIN_BALLOONSHOW      (WM_USER + 2)
#define NIN_BALLOONHIDE      (WM_USER + 3)
#define NIN_BALLOONTIMEOUT   (WM_USER + 4)
#define NIN_BALLOONUSERCLICK (WM_USER + 5)
#endif

#ifndef NIN_SELECT
#define NIN_SELECT          (WM_USER + 0)
#define NINF_KEY            0x1
#define NIN_KEYSELECT       (NIN_SELECT | NINF_KEY)
#endif

#ifndef NIM_SETFOCUS
#define NIM_SETFOCUS    0x00000003
#endif
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     CXTTrayIcon is a CWnd derived class of the NOTIFYICONDATA structure.
//     It is used to display a system tray icon with animation.
//===========================================================================
class _XT_EXT_CLASS CXTTrayIcon : public CWnd
{
private:
	//-----------------------------------------------------------------------
	// Summary:
	//     TRAYICONDATA is a stand alone helper structure. It is used by CXTTrayIcon
	//     to maintain an animation icon list.
	//-----------------------------------------------------------------------
	struct TRAYICONDATA
	{
		HICON   hIcon;      // Handle to the icon displayed.
		CString strToolTip; // A NULL terminated string that represents the tooltip displayed for the tray icon.
	};

	//-----------------------------------------------------------------------
	// Helper structure for compatibility with older versions of
	// Visual C++, see NOTIFYICONDATA.
	//-----------------------------------------------------------------------
	struct NOTIFYICONDATAEX
	{
		DWORD cbSize;          // Size of this structure, in bytes.
		HWND hWnd;             // Handle to the window that will receive notification messages associated with an icon in the taskbar status area. The shell uses hWnd and uID to identify which icon is to be operated on when Shell_NotifyIcon is invoked.
		UINT uID;              // Application-defined identifier of the taskbar icon. The shell uses hWnd and uID to identify which icon is to be operated on when Shell_NotifyIcon is invoked. You can have multiple icons associated with a single hWnd by assigning each a different uID.
		UINT uFlags;           // Array of flags that indicate which of the other members contain valid data
		UINT uCallbackMessage; // Application-defined message identifier
		HICON hIcon;           // Handle to the icon to be added, modified, or deleted.
		TCHAR szTip[128];      // Pointer to a NULL-terminated string with the text for a standard tooltip.
		DWORD dwState;         // State of the icon
		DWORD dwStateMask;     // A value that specifies which bits of the state member will be retrieved or modified.
		TCHAR szInfo[256];     // Pointer to a NULL-terminated string with the text for a balloon-style tooltip
		union {
			UINT  uTimeout;    // The timeout value, in milliseconds, for a balloon-style tooltip.
			UINT  uVersion;    // Used to specify whether the shell notify icon interface should use Windows 95 or Windows 2000 behavior.
		} DUMMYUNIONNAME;
		TCHAR szInfoTitle[64]; // Pointer to a NULL-terminated string containing a title for a balloon tooltip.
		DWORD dwInfoFlags;     // Flags that can be set to add an icon to a balloon tooltip
	};

	//-----------------------------------------------------------------------
	// Summary:
	//     Animated icon array
	//-----------------------------------------------------------------------
	typedef CList<TRAYICONDATA, TRAYICONDATA&> CTrayIconList;

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTTrayIcon object
	//-----------------------------------------------------------------------
	CXTTrayIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTTrayIcon object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTTrayIcon();


public:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will create the system tray icon.
	// Parameters:
	//     lpszCaption       - NULL terminated string that contains the tooltip text displayed
	//                         when the cursor is over the tray icon.
	//     pParentWnd        - Pointer to the window that will receive notification messages
	//                         associated with an icon in the taskbar status area.
	//     nIconID           - Resource ID for the default tray icon.
	//     uMenuID           - Popup menu resource identifier.
	//     uDefMenuItemID    - Command ID that represents the default item for the menu.
	//     bDefMenuItemByPos - true if the default is defined by its position. false if it is
	//     defined by its command ID.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool Create(LPCTSTR lpszCaption,CWnd* pParentWnd,UINT nIconID,UINT uMenuID=0,UINT uDefMenuItemID=0,bool bDefMenuItemByPos=false);
//{{AFX_CODEJOCK_PRIVATE
	virtual bool Create(LPCTSTR lpszCaption, DWORD dwStyle, CWnd* pParentWnd, UINT nIconID);// DEPRECIATED
//}}AFX_CODEJOCK_PRIVATE

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to display a balloon tooltip for the system
	//     tray icon. Must have Windows 2000 or later.
	// Parameters:
	//     lpszInfo      - Pointer to a null-terminated string with the text for a balloon
	//                     ToolTip. It can have a maximum of 255 characters.
	//     lpszInfoTitle - Pointer to a null-terminated string containing a title for a
	//                     balloon ToolTip. This title appears in boldface above the text.
	//                     It can have a maximum of 63 characters.
	//     dwInfoFlags   - Flags that can be set to add an icon to a balloon ToolTip.
	//                     It is placed to the left of the title. If the 'lpszInfoTitle' member
	//                     is zero-length, the icon is not shown, and can be any of the values listed in the Remarks section.
	//     uTimeout      - The timeout value in milliseconds for a balloon ToolTip.
	//                     The system enforces minimum and maximum timeout values. 'uTimeout'
	//                     values that are too large are set to the maximum value, and values
	//                     that are too small default to the minimum value. The system minimum
	//                     and maximum timeout values are currently set at 10 seconds and 30
	//                     seconds, respectively.
	// Remarks:
	//    The icon shown can be any of the following values:<p/>
	//
	//    * <b>NIIF_ERROR</b> An error icon.
	//    * <b>NIIF_INFO</b> An information icon.
	//    * <b>NIIF_NONE</b> No icon.
	//    * <b>NIIF_WARNING</b> A warning icon.
	// Returns:
	//    true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowBalloonTip(LPCTSTR lpszInfo=NULL,LPCTSTR lpszInfoTitle=NULL, DWORD dwInfoFlags=NIIF_NONE,UINT uTimeout=10);


	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will stop the tray icon animation timer
	//-----------------------------------------------------------------------
	virtual void StopAnimation();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will start the tray icon animation timer.
	// Parameters:
	//     uElapse - Specifies the time-out value in milliseconds between frames.
	//-----------------------------------------------------------------------
	virtual void StartAnimation(UINT uElapse=500);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the icons used for animated system
	//     tray icons. To use, call SetAnimationIcons, then SetTimer.
	// Parameters:
	//     lpIDArray     - An array of resource IDs that represent the icons to display
	//                     in the caption area.
	//     nIDCount      - Size of the array 'lpStrTipArray'.
	//     lpStrTipArray - An array of tooltips that match the icons passed in as 'lpStrTipArray'.
	//                     If NULL, the default tooltip will be used.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	virtual bool SetAnimationIcons(const UINT* lpIDArray,int nIDCount,const CString* lpStrTipArray=NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the tooltip text for the tray icon.
	// Parameters:
	//     lpszTipText - Null terminated string that represents the tooltip text to display
	//                   for the icon.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetTooltipText(LPCTSTR lpszTipText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the tooltip text for the tray icon.
	// Parameters:
	//     nTipText - Windows string resource ID that represents the tooltip text
	//                to display for the icon.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetTooltipText(UINT nTipText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the tooltip text that is displayed
	//     by the tray icon.
	// Returns:
	//     A CString object that represents the tooltip text.
	//-----------------------------------------------------------------------
	CString GetTooltipText() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to reset the tray icon control settings
	//     to their default value.
	//-----------------------------------------------------------------------
	void SetDefaultValues();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the callback message to be used by the
	//     tray icon.
	// Parameters:
	//     uNewCallbackMessage - Notification message ID.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetCallbackMessage(UINT uNewCallbackMessage);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will retrieve the notification message ID used
	//     by the tray icon.
	// Returns:
	//     An ID to a notification message.
	//-----------------------------------------------------------------------
	UINT GetCallbackMessage();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the default menu item for the tray icon's
	//     popup menu if the tray icon handles notification messages.
	// Parameters:
	//     uItem  - Command identifier or position of the default menu item.
	//     bByPos - true if 'uItem' is the menu item's index; false if it is the menu
	//              item's command ID.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetDefaultMenuItem(UINT uItem,bool bByPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve the item ID and position flag
	//     for the tray icon.
	// Parameters:
	//     uItem  - Reference to a UINT that is to receive the default menu item ID.
	//     bByPos - Reference to a bool that is to receive the default menu item position
	//              flag.
	//-----------------------------------------------------------------------
	void GetDefaultMenuItem(UINT& uItem,bool& bByPos);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the window that is to receive notification
	//     messages from the tray icon.
	// Parameters:
	//     pWndNotify - Points to a valid CWnd object.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetNotificationWnd(CWnd* pWndNotify);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to retrieve a pointer to the window that
	//     receives notification messages from the tray icon.
	// Returns:
	//     A pointer to a valid CWnd object.
	//-----------------------------------------------------------------------
	CWnd* GetNotificationWnd();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the icon that is displayed in the
	//     system tray.
	// Parameters:
	//     hIcon - Handle to the icon to display.
	// Returns:
	//     true if the icon was set, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetIcon(HICON hIcon);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the icon that is displayed in the
	//     system tray.
	// Parameters:
	//     lpszIconName - Resource name of the icon to display.
	// Returns:
	//     true if the icon was set, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetIcon(LPCTSTR lpszIconName);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the icon that is displayed in the
	//     system tray.
	// Parameters:
	//     nIDResource - Resource identifier of the icon to display.
	// Returns:
	//     true if the icon was set, otherwise returns false.
	//-----------------------------------------------------------------------
	bool SetIcon(UINT nIDResource);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return a handle to the icon displayed
	//     in the system tray.
	// Returns:
	//     A handle to the icon displayed in the system tray.
	//-----------------------------------------------------------------------
	HICON GetIcon() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to add the icon to the system tray.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool AddIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to remove the icon from the system tray.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool RemoveIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to hide the system tray icon in Windows
	//     2000 or greater.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool HideIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to show the system tray icon in Windows
	//     2000 or greater.
	// Returns:
	//     true if successful, otherwise returns false.
	//-----------------------------------------------------------------------
	bool ShowIcon();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to minimize the specified window to the system
	//     tray. If window animation is supported, the window will glide down
	//     to the system tray.
	// Parameters:
	//     pWnd - Window to minimize.
	//-----------------------------------------------------------------------
	void MinimizeToTray(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to maximize the specified window from the
	//     system tray. If window animation is supported, the window will glide
	//     up from the system tray.
	// Parameters:
	//     pWnd - Window to maximize.
	//-----------------------------------------------------------------------
	void MaximizeFromTray(CWnd* pWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sends a message to the taskbar's status area.
	// Parameters:
	//     dwMessage - [in] Variable of type DWORD that specifies the action to
	//                 be taken. See remarks section for a list of values.
	// Remarks:
	//     <i>dwMessage</i> can have one of the following values. <p/>
	//     * <b>NIM_ADD</b>        Adds an icon to the status area. The hWnd and
	//                             uID members of the NOTIFYICONDATA structure
	//                             pointed to by lpdata will be used to identify
	//                             the icon in later calls to Shell_NotifyIcon.
	//     * <b>NIM_DELETE</b>     Deletes an icon from the status area. Use the
	//                             hWnd and uID members of the NOTIFYICONDATA
	//                             structure pointed to by lpdata to identify
	//                             the icon to be deleted.
	//     * <b>NIM_MODIFY</b>     Modifies an icon in the status area. Use the
	//                             hWnd and uID members of the NOTIFYICONDATA
	//                             structure pointed to by lpdata to identify
	//                             the icon to be modified.
	//     * <b>NIM_SETFOCUS</b>   Version 5.0. Returns focus to the taskbar notification
	//                             area. Taskbar icons should use this message
	//                             when they have completed their user interface
	//                             operation. For example, if the taskbar icon
	//                             displays a shortcut menu, but the user presses
	//                             ESCAPE to cancel it, use NIM_SETFOCUS to return
	//                             focus to the taskbar notification area.
	//     * <b>NIM_SETVERSION</b> Version 5.0. Instructs the taskbar to behave
	//                             according to the version number specified in
	//                             the uVersion member of the structure pointed
	//                             to by lpdata. This message allows you to specify
	//                             whether you want the version 5.0 behavior found
	//                             on Microsoft® Windows® 2000 systems, or that
	//                             found with earlier Shell versions. The default
	//                             value for uVersion is zero, indicating that
	//                             the original Windows 95 notify icon behavior
	//                             should be used. For details, see the Remarks
	//                             section.
	//     The taskbar notification area is sometimes erroneously called the "tray."
	//
	//     Version 5.0 of the Shell, found on Windows 2000, handles Shell_NotifyIcon
	//     mouse and keyboard events differently than earlier Shell versions found
	//     on Windows NT® 4.0, Windows 95, and Windows 98. The differences are:
	//
	//     If a user selects a notify icon's shortcut menu with the keyboard, the
	//     version 5.0 Shell sends the associated application a WM_CONTEXTMENU message.
	//     Earlier versions send WM_RBUTTONDOWN and WM_RBUTTONUP messages. <p/>
	//
	//     If a user selects a notify icon with the keyboard and activates it with
	//     the space bar or ENTER key, the version 5.0 Shell sends the associated
	//     application an NIN_KEYSELECT notification. Earlier versions send WM_RBUTTONDOWN
	//     and WM_RBUTTONUP messages. <p/>
	//
	//     If a user selects a notify icon with the mouse and activates it with
	//     the ENTER key, the version 5.0 Shell sends the associated application
	//     an NIN_SELECT notification. Earlier versions send WM_RBUTTONDOWN and
	//     WM_RBUTTONUP messages. <p/>
	//
	//     If a user passes the mouse pointer over an icon with which a balloon
	//     ToolTip is associated, the version 5.0 Shell sends the following messages.<p/>
	//
	//     * <b>NIN_BALLOONSHOW</b>      Sent when the balloon is shown (balloons
	//                                   are queued).
	//     * <b>NIN_BALLOONHIDE</b>      Sent when the balloon disappears—when
	//                                   the icon is deleted, for example. This
	//                                   message is not sent if the balloon is
	//                                   dismissed because of a timeout or mouse
	//                                   click by the user.
	//     * <b>NIN_BALLOONTIMEOUT</b>   Sent when the balloon is dismissed because
	//                                   of a timeout.
	//     * <b>NIN_BALLOONUSERCLICK</b> Sent when the balloon is dismissed because
	//                                   the user clicked the mouse.
	//
	//     You can select which way the Shell should behave by calling Shell_NotifyIcon
	//     with dwMessage set to NIM_SETVERSION. Set the uVersion member of the
	//     NOTIFYICONDATA structure to indicate whether you want pre-version 5.0
	//     behavior or version 5.0 and later behavior.
	// Returns:
	//     Returns TRUE if successful or FALSE otherwise. If dwMessage is set to
	//     NIM_SETVERSION, the function returns TRUE if the version was successfully
	//     changed or FALSE if the requested version is not supported.
	//-----------------------------------------------------------------------
	BOOL ShellNotify(DWORD dwMessage);

	//-------------------------------------------------------------------------
	// Summary:
	//     Adds an icon to the system tray.
	// Remarks:
	//     This member is called by the CXTTrayIcon object to initialize the
	//     icon display in the system tray. It checks to see if there is a
	//     pending request or if the icon is hidden first before attempting to
	//     add the icon. You can override this member to provide additional
	//     functionality.
	//-------------------------------------------------------------------------
	virtual void InstallIconPending();

	//-----------------------------------------------------------------------
	// Summary:
	//     Creates the tray icons min / max window.
	// Remarks:
	//     Called to create the minimize window used during minimize / maximize
	//     operations.
	// Parameters:
	//     pWndApp - Points to the calling applications main window.
	// Returns:
	//     true if the minimize window was successfully created, otherwise false.
	//-----------------------------------------------------------------------
	virtual bool CreateMinimizeWnd(CWnd* pWndApp);

	//-------------------------------------------------------------------------
	// Summary:
	//     Clears animated icon array.
	// Remarks:
	//     Called to clear the animated icon array and free any associated
	//     GDI resources.
	//-------------------------------------------------------------------------
	virtual void RemoveAnimationIcons();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the tooltip for the CXTTrayIcon object.
	// Parameters:
	//     strToolTip - NULL terminated string that contains the tooltip text displayed
	//                  when the cursor is over the tray icon.
	// Remarks:
	//     This member function is called to set the tooltip text for the
	//     icon that is displayed in the system tray.
	//-----------------------------------------------------------------------
	virtual void SetTooltip(CString strToolTip);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the icon for the CXTTrayIcon object.
	// Parameters:
	//     nIcon     - Resource id of the new icon to set.
	//     dwMessage - Message to be sent, see ShellNotify for more details.
	// Remarks:
	//     This member function is called to set a new icon to be displayed
	//     for the CXTTrayIcon object.
	//-----------------------------------------------------------------------
	virtual void SetTrayIcon(UINT nIcon, DWORD dwMessage=NIM_ADD);

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets animated icons for the CXTTrayIcon object.
	// Parameters:
	//     lpIDArray     - [in] An array of icon resource IDs.
	//     lpStrTipArray - [in] An array of CString objects representing the tooltip
	//                     text to display for each icon in <i>lpIDArray</i>.
	//     nIDCount      - [in] Zero based size of the array passed. <i>lpIDArray</i> and
	//                     <i>lpStrTipArray</i> must contain the same number
	//                     of items and must match value specified by <i>nIDCount</i>.
	//-----------------------------------------------------------------------
	virtual void SetAnimationIcons(const UINT* lpIDArray,const CString* lpStrTipArray,int nIDCount);

	//-------------------------------------------------------------------------
	// Summary:
	//     Stops the animation timer.
	// Remarks:
	//     Called by the CXTTrayIcon object to stop the animation timer event
	//     and sets the default icon and tooltip text. You can override this
	//     member to provide additional functionality.
	//-------------------------------------------------------------------------
	virtual void KillTimer();

	//-----------------------------------------------------------------------
	// Summary:
	//     Sets the animation timer.
	// Parameters:
	//     nIDEvent - Specifies a nonzero timer identifier.
	//     uElapse  - Specifies the time-out value, in milliseconds.
	// Remarks:
	//     Called by the CXTTrayIcon object to initialize and start the
	//     animation timer. you can override this member to provide additional
	//     functionality.
	//-----------------------------------------------------------------------
	virtual void SetTimer(UINT nIDEvent, UINT uElapse);

protected:
//{{AFX_CODEJOCK_PRIVATE
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(CXTTrayIcon)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTTrayIcon)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	bool CanAnimate() const;
	static BOOL CALLBACK FindTrayWnd(HWND hWnd, LPARAM lParam);
	bool GetTrayWindowRect(CRect& rect);

protected:

	UINT                m_uFlags;            // Style settings for icon restore.
	UINT                m_nIconID;           // Resource ID for the default icon.
	UINT                m_nIDEvent;          // Timer event ID.
	UINT                m_nCounter;          // Holds the current position in the timer loop.
	UINT                m_uDefMenuItemID;    // Default popup menu item ID.
	bool                m_bDefMenuItemByPos; // Determines if the default menu item is a command or index.
	bool                m_bHidden;           // State of the icon. true to indicate the icon is hidden.
	bool                m_bRemoved;          // true if the icon has been removed from the system tray.
	bool                m_bShowPending;      // true if the icon display is pending.
	HWND                m_hWndNotify;        // Handle to the window that receives command notification.
	CWnd                m_wndMinimize;       // Hidden window used during minimize and restore functions.
	size_t              m_iMaxTipSize;       // Maximum size for tooltip string.
	CString             m_strToolTip;        // Tooltip for the default icon.
	CXTIconHandle       m_hIcon;             // Default icon.
	CTrayIconList       m_arTrayIcons;       // Array of icons and text that are displayed during animation.
	NOTIFYICONDATAEX*   m_pNIData;           // Tray icon structure see NOTIFYICONDATA.


};

//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTTrayIcon::Create(LPCTSTR lpszCaption, DWORD dwStyle, CWnd* pParentWnd, UINT nIconID) {
	UNREFERENCED_PARAMETER(dwStyle); return Create(lpszCaption, pParentWnd, nIconID);
}
AFX_INLINE void CXTTrayIcon::SetTooltip(CString strToolTip) {
	SetTooltipText(strToolTip);
}
AFX_INLINE void CXTTrayIcon::SetTrayIcon(UINT nIcon, DWORD dwMessage) {
	UNREFERENCED_PARAMETER(dwMessage); SetIcon(nIcon);
}
AFX_INLINE void CXTTrayIcon::SetAnimationIcons(const UINT* lpIDArray,const CString* lpStrTipArray,int nIDCount) {
	SetAnimationIcons(lpIDArray, nIDCount, lpStrTipArray);
}
AFX_INLINE void CXTTrayIcon::KillTimer() {
	StopAnimation();
}
AFX_INLINE void CXTTrayIcon::SetTimer(UINT nIDEvent, UINT uElapse) {
	m_nIDEvent = nIDEvent; StartAnimation(uElapse);
}
AFX_INLINE BOOL CXTTrayIcon::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

#endif // #if !defined(__TRAYICON_H__)
