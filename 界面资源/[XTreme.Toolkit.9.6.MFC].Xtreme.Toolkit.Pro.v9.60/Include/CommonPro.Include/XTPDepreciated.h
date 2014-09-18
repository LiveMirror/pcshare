// XTPDepreciated.h : depreciated (obsolete) members
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
#if !defined(__XTPDEPRECIATED_H__)
#define __XTPDEPRECIATED_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//{{AFX_CODEJOCK_PRIVATE

// depreciated class names
#define CXTManageState              CXTPManageState
#define CXTDialogState              CXTPDialogState

// depreciated structure names
#define XT_AUX_DATA                 CXTAuxData
#define XT_LOGFONT                  CXTLogFont
#define XT_NONCLIENTMETRICS         CXTNonClientMetrics

#define CXTBarFolder                CXTOutBarFolder
#define CXTBarItem                  CXTOutBarItem
#define CXTEditGroup                CXTEditListBoxToolBar
#define CXTEditItem                 CXTOutBarEditItem

// depreciated global functions
#define InitXtremeExtDLL            XTFuncInitDLL
#define XTPContextMenu              XTFuncContextMenu
#define _xtAfxChangeWindowFont      XTFuncChangeWindowFont
#define _xtAfxDrawEmbossed          XTFuncDrawEmbossed
#define _xtAfxDrawShadedRect        XTFuncDrawShadedRect
#define _xtAfxLoadSysColorBitmap    XTFuncLoadSysColorBitmap
#define _xtAfxChildWindowFromPoint  XTFuncChildWindowFromPoint
#define _xtAfxStripMnemonics        XTFuncStripMnemonics
#define _xtAfxExist                 XTFuncPathExists
#define _xtAfxPathFindNextComponent XTFuncPathFindNextComponent
#define _xtAfxCreateView            XTFuncCreateView
#define _xtAfxShowPrintPreview      XT_ASSERT_MSG(FALSE, "_xtAfxShowPrintPreview is depreciated (obsolete).\nSee the \"PrintPreview\" sample for more details.")

// depreciated helper macros
#define _delete(x)                  SAFE_DELETE(x)
#define _deleteArray(x)             SAFE_DELETE_AR(x)

// Depreciated enumerators
#define ANIMATIONTYPE               XTAnimationType

#define eSTYLE                      XTFontStyle
#define XT_FLB_NAME_GUI             xtFontGUI
#define XT_FLB_NAME_SAMPLE          xtFontSample
#define XT_FLB_BOTH                 xtFontBoth

#define icon_type                   XTArrowIcon
#define arrow_left                  xtArrowIconLeft
#define arrow_left_home             xtArrowIconLeftHome
#define arrow_right                 xtArrowIconRight
#define arrow_right_home            xtArrowIconRightHome

#define FOLDER_HILIGHT              XTMouseState
#define F_NORMAL                    xtMouseNormal
#define F_SELECT                    xtMouseSelect
#define F_HILIGHT                   xtMouseHover

#define EFlags                      XTResize
#define SZ_NOSIZEICON               xtResizeNoSizeIcon
#define SZ_NOHORISONTAL             xtResizeNoHorisontal
#define SZ_NOVERTICAL               xtResizeNoVertical
#define SZ_NOMINSIZE                xtResizeNoMinsize
#define SZ_NOCLIPCHILDREN           xtResizeNoClipChildren
#define SZ_NOTRANSPARENTGROUP       xtResizeNoTransparentGroup

#define FindAttribs                 XTFindType
#define type_drive                  xtFindTypeDrive
#define type_folder                 xtFindTypeFolder

#define XT_DATA_TYPE                XTSortType
#define DT_INT                      xtSortInt
#define DT_STRING                   xtSortString
#define DT_DATETIME                 xtSortDateTime
#define DT_DEC                      xtSortDecimal

#define XT_NAVBTNFLAGS              XTNavBtnState
#define XT_SHOW_ARROWS              xtNavBtnArrows
#define XT_SHOW_CLOSE               xtNavBtnClose
#define XT_SHOW_ALL                 xtNavBtnShowAll

// depreciated global data
#define xtAfxData                   XTAuxData()
#define xtVerInfo                   *XTOSVersionInfo()


// docking panes defines:
#define dockLeftOf                  xtpPaneDockLeft
#define dockRightOf                 xtpPaneDockRight
#define dockTopOf                   xtpPaneDockTop
#define dockBottomOf                xtpPaneDockBottom

#define typeDockingPane             xtpPaneTypeDockingPane
#define typeTabbedContainer         xtpPaneTypeTabbedContainer
#define typeSplitterContainer       xtpPaneTypeSplitterContainer
#define typeMiniWnd                 xtpPaneTypeMiniWnd
#define typeClient                  xtpPaneTypeClient
#define typeAutoHidePanel           xtpPaneTypeAutoHidePanel

// property grid defines
#define CXTPItemConstraints         CXTPPropertyGridItemConstraints

#define pgiHasEdit                  xtpGridItemHasEdit
#define pgiHasExpandButton          xtpGridItemHasExpandButton
#define pgiHasComboButton           xtpGridItemHasComboButton

#define Categorized                 xtpGridSortCategorized
#define Alphabetical                xtpGridSortAlphabetical
#define NoSort                      xtpGridSortNoSort

// tab manager
#define XTP_TABCOLOR_BLUE           xtpTabColorBlue
#define XTP_TABCOLOR_YELLOW         xtpTabColorYellow
#define XTP_TABCOLOR_GREEN          xtpTabColorGreen
#define XTP_TABCOLOR_RED            xtpTabColorRed
#define XTP_TABCOLOR_PURPLE         xtpTabColorPurple
#define XTP_TABCOLOR_CYAN           xtpTabColorCyan
#define XTP_TABCOLOR_ORANGE         xtpTabColorOrange
#define XTP_TABCOLOR_MAGENTA        xtpTabColorMagenta
#define XTP_TABMANAGER_BUTTON_LEFT  xtpTabNavigateButtonLeft
#define XTP_TABMANAGER_BUTTON_RIGHT xtpTabNavigateButtonRight
#define XTP_TABMANAGER_BUTTON_CLOSE xtpTabNavigateButtonClose

// CommandBars
#define XTP_COMMANDBARS_OPTIONS     CXTPCommandBarsOptions
#define XTP_GRADIENT_COLOR          CXTPPaintManagerColorGradient
#define CXTStatusBar                CXTPStatusBar
#define CXTIconPane                 CXTPStatusBarIconPane
#define CXTLogoPane                 CXTPStatusBarLogoPane

//}}AFX_CODEJOCK_PRIVATE

//////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTPDEPRECIATED_H__)
