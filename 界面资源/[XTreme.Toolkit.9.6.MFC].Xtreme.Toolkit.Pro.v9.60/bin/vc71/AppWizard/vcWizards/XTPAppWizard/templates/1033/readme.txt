================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : [!output PROJECT_NAME] Project Overview
===============================================================================

The application wizard has created this [!output PROJECT_NAME] application for 
you.  This application not only demonstrates the basics of using the Microsoft 
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your [!output PROJECT_NAME] application.

[!output PROJECT_NAME].vcproj
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

[!output APP_HEADER]
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    [!output APP_CLASS] application class.

[!output APP_IMPL]
    This is the main application source file that contains the application
    class [!output APP_CLASS].

[!output PROJECT_NAME].rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in [!output LCID].

res\[!output SAFE_PROJECT_NAME].ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file [!output PROJECT_NAME].rc.

res\[!output SAFE_PROJECT_NAME].rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]
[!output PROJECT_NAME].reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
[!if APP_TYPE_DLG]
    file to go along with your application.
[!else]
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.
[!endif]

[!if AUTOMATION]
[!output PROJECT_NAME].idl
    This file contains the Interface Description Language source code for the
    type library of your application.
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

For the main frame window:
[!if PROJECT_STYLE_EXPLORER]
    Windows Explorer Style: The project will include a Windows Explorer-like
    interface, with two frames.
[!else]
    The project includes a standard MFC interface.
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    These files contain the frame class [!output MAIN_FRAME_CLASS], which is derived from
[!if APP_TYPE_MDI]
    CMDIFrameWnd and controls all MDI frame features.
[!else]
    CFrameWnd and controls all SDI frame features.
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    These files contain the left frame class [!output TREE_VIEW_CLASS], which is derived from
    CTreeView.
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the [!output MAIN_FRAME_CLASS]
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in [!output PROJECT_NAME].rc to add
    toolbar buttons.
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    This bitmap file is used to create tiled images for the toolbar
    when your server application is in-place activated inside another
    container. This toolbar is constructed in the [!output INPLACE_FRAME_CLASS]
    class. This bitmap is similar to the bitmap in res\Toolbar.bmp
    except that it has many nonserver commands removed.

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

For the child frame window:

ChildFrm.h, ChildFrm.cpp
    These files define and implement the [!output CHILD_FRAME_CLASS] class, which
    supports the child windows in an MDI application.

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
The application wizard creates one document type and one view:

[!output DOC_HEADER], [!output DOC_IMPL] - the document
    These files contain your [!output DOC_CLASS] class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via [!output DOC_CLASS]::Serialize).
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    The Document will have the following strings:
        File extension:      [!output FILE_EXTENSION]
        File type ID:        [!output FILE_TYPE_ID]
        Main frame caption:  [!output MAIN_FRAME_CAPTION]
        Doc type name:       [!output DOC_TYPE_NAME]
        Filter name:         [!output FILTER_NAME]
        File new short name: [!output FILE_NEW_NAME_SHORT]
        File type long name: [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
The application wizard creates one view:
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] - the view of the document
    These files contain your [!output VIEW_CLASS] class.
[!if !DB_VIEW_NO_FILE]
    [!output VIEW_CLASS] objects are used to view [!output DOC_CLASS] objects.
[!endif]

[!if APP_TYPE_MDI]
res\[!output SAFE_PROJECT_NAME]Doc.ico
    This is an icon file, which is used as the icon for MDI child windows
    for the [!output DOC_CLASS] class.  This icon is included by the main
    resource file [!output PROJECT_NAME].rc.
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

Database support:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    These files contain your [!output ROWSET_CLASS] class.  This class is used to access
    the data source you selected in the wizard.
[!if DB_VIEW_NO_FILE]
    No serialization support will be added.
[!else]
    Serialization support has been added.
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

The application wizard has also created classes specific to OLE

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    These files contain your [!output CONTAINER_ITEM_CLASS] class. This class is used to 
    manipulate OLE objects.  OLE objects are usually displayed by your
    [!output VIEW_CLASS] class and serialized as part of your [!output DOC_CLASS] class.
[!if ACTIVE_DOC_CONTAINER]
    The program includes support to contain Active documents within its frame.
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    These files contain your [!output SERVER_ITEM_CLASS]. This class is used to
    connect your [!output DOC_CLASS] class to the OLE system, and optionally
    provide links to your document.
[!if ACTIVE_DOC_SERVER]
    The project has support to create and manage Active documents.
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    These files contain your [!output INPLACE_FRAME_CLASS]. This class is derived
    from COleIPFrameWnd and controls all frame features during in-place activation.
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    The project has support for Compound files. The Compound-file format stores a document 
    that contains one or more Automation objects to one file and still allows access to 
    the it for the individual objects.
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
The application wizard creates one dialog class and automation proxy class:
[!else]
The application wizard creates one dialog class:
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL] - the dialog
    These files contain your [!output DIALOG_CLASS] class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in [!output PROJECT_NAME].rc, which can be edited in Microsoft Visual C++.
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] - the automation object
    These files contain your [!output DIALOG_AUTO_PROXY_CLASS] class.  This class
    is called the Automation proxy class for your dialog, because it
    takes care of exposing the Automation methods and properties that
    Automation controllers can use to access your dialog.  These methods
    and properties are not exposed from the dialog class directly, because
    in the case of a modal dialog-based MFC application it is cleaner and
    easier to keep the Automation object separate from the user interface.
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

Help Support:

[!if HELPSTYLE_HTML]
hlp\[!output PROJECT_NAME].hhp
    This file is a help project file. It contains the data needed to
    compile the help files into a .chm file.

hlp\[!output PROJECT_NAME].hhc
    This file lists the contents of the help project.

hlp\[!output PROJECT_NAME].hhk
    This file contains an index of the help topics.

hlp\afxcore.htm
    This file contains the standard help topics for standard MFC
    commands and screen objects. Add your own help topics to this file.

[!if PRINTING]
hlp\afxprint.htm
    This file contains the help topics for the printing commands.

[!endif]
makehtmlhelp.bat
    This file is used by the build system to compile the help files.

hlp\Images\*.gif
    These are bitmap files required by the standard help file topics for
    Microsoft Foundation Class Library standard commands.

[!else]
hlp\[!output PROJECT_NAME].hpj
    This file is the help project file used by the help compiler to create
    your application's help file.

hlp\*.bmp
    These are bitmap files required by the standard HELP file topics for
    Microsoft Foundation Class Library standard commands.

hlp\*.rtf
    These files contain the standard help topics for standard MFC
    commands and screen objects.
[!endif]
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

Other Features:
[!if ACTIVEX_CONTROLS]

ActiveX Controls
    The application includes support to use ActiveX controls.
[!endif]
[!if PRINTING]

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

Database support
    The application wizard has added the basic level of database support for your program. 
    Only the needed files have been included.
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

Split Window
    The application wizard has added support for splitter windows for your application documents.
[!endif]
[!if MAPI]

MAPI support
    The generated project contains the code needed to create, manipulate, transfer and
    store mail messages.
[!endif]
[!if SOCKETS]

Windows Sockets
    The application has support for establishing communications over TCP/IP networks.
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named [!output PROJECT_NAME].pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	Application manifest files are used by Windows XP to describe an applications 
	dependency on specific versions of Side-by-Side assemblies. The loader uses this 
	information to load the appropriate assembly from the assembly cache or private 
	from the application. The Application manifest  maybe included for redistribution 
	as an external .manifest file that is installed in the same folder as the application 
	executable or it may be included in the executable in the form of a resource. 
[!endif]
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

If your application uses MFC in a shared DLL, and your application is in a 
language other than the operating system's current language, you will need 
to copy the corresponding localized resources MFC70XXX.DLL from the Microsoft
Visual C++ CD-ROM under the Win\System directory to your computer's system or 
system32 directory, and rename it to be MFCLOC.DLL.  ("XXX" stands for the 
language abbreviation.  For example, MFC70DEU.DLL contains resources 
translated to German.)  If you don't do this, some of the UI elements of 
your application will remain in the language of the operating system.
[!endif]

/////////////////////////////////////////////////////////////////////////////
