// (c) 2001 Microsoft Corporation

function OnFinish(selProj, selObj)
{
	try
	{		
		var bOLEDB = wizard.FindSymbol("OLEDB");
		if (bOLEDB)
		{
			var strRowsetClass = wizard.FindSymbol("ROWSET_CLASS");
			var bAttributed = wizard.FindSymbol("ATTRIBUTED");
			var strCode = wizard.GetConsumerClass(strRowsetClass, bAttributed);
			wizard.AddSymbol("ROWSET_CLASS_CODE", strCode);
		}
		else
		{
			var bODBC = wizard.FindSymbol("ODBC");
			if (bODBC)
			{
				var strRowsetClass = wizard.FindSymbol("ROWSET_CLASS");
				var bSnapshot = wizard.FindSymbol("SNAPSHOT");
				var bBindAllColumns = wizard.FindSymbol("BIND_ALL_COLUMNS");
				var strCodeDecl = wizard.GetODBCConsumerClassDecl(bBindAllColumns, bSnapshot, strRowsetClass);
				var strCodeImpl = wizard.GetODBCConsumerClassImpl();
				wizard.AddSymbol("ROWSET_CLASS_ODBC_DECL", strCodeDecl);
				wizard.AddSymbol("ROWSET_CLASS_ODBC_IMPL", strCodeImpl);
			}
		}

		var strProjectPath = wizard.FindSymbol("PROJECT_PATH");
		var strProjectName = wizard.FindSymbol("PROJECT_NAME");

		selProj = CreateProject(strProjectName, strProjectPath);

		AddCommonConfig(selProj, strProjectName);
		AddSpecificConfig(selProj, strProjectName, strProjectPath);

		SetFilters(selProj);

		var bContextHelp = wizard.FindSymbol("CONTEXT_HELP");
		var bHTMLHelp = wizard.FindSymbol("HELPSTYLE_HTML");
		var L_strHelpFiles_Text = "Help Files";
		if (bContextHelp)
		{
			if (bHTMLHelp)
			{
				var strHelpFilter = wizard.FindSymbol("HTMLHELP_FILTER");
                var L_strHTMLHelpFiles_Text = "HTML Help Files";
				var group = selProj.Object.AddFilter(L_strHTMLHelpFiles_Text);
				group.Filter = strHelpFilter;

				var strHelpFilter = wizard.FindSymbol("HTMLHELP_TOPICSFILTER");
                var L_strHelpTopics_Text = "HTML Help Topics";
				var group = selProj.Object.AddFilter(L_strHelpTopics_Text);
				group.Filter = strHelpFilter;
			}
			else
			{
				var strHelpFilter = wizard.FindSymbol("HELP_FILTER");
				var group = selProj.Object.AddFilter(L_strHelpFiles_Text);
				group.Filter = strHelpFilter;
			}
		}
		
		var strDocTypeName = wizard.FindSymbol("DOC_TYPE_NAME"); 		
		if (strDocTypeName != "" )
		{
			wizard.AddSymbol("SAFE_DOC_TYPE_NAME", CreateSafeName(strDocTypeName));
		}
		
		selProj.Object.keyword = "MFCProj";

		var strCodePage = wizard.FindSymbol("CODE_PAGE");
		if (strCodePage == "1252")
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "8");
		else
			wizard.AddSymbol("ABOUTBOX_FONT_SIZE", "9");

		var InfFile = CreateInfFile();

		AddFilesToProject(selProj, strProjectName, InfFile);
		SetCommonPchSettings(selProj);

		if (bContextHelp && !bHTMLHelp)
			AddHelpImages(selProj, L_strHelpFiles_Text);

		InfFile.Delete();
		selProj.Object.Save();

		if (wizard.FindSymbol("APP_TYPE_DLG"))
		{
			var strDialogId = "IDD_" + wizard.FindSymbol("UPPER_CASE_SAFE_PROJECT_NAME") + "_DIALOG";
			var oResHelper = wizard.ResourceHelper;
			oResHelper.OpenResourceFile(strProjectPath + "\\" + strProjectName + ".rc");
			oResHelper.OpenResourceInEditor("DIALOG", strDialogId);
		}
		if (bContextHelp)
			AddHelpBuildSteps(selProj.Object, strProjectName, bHTMLHelp);
	}
	catch(e)
	{
		if (e.description.length != 0)
			SetErrorInfo(e);
		return e.number
	}
}

function SetFileProperties(projfile, strName)
{
	return false;
}

function DoOpenFile(strTarget)
{
	return false;
}

function AddSpecificConfig(proj, strProjectName, strProjectPath)
{
	try
	{
		var bAutomation = wizard.FindSymbol("AUTOMATION");
		var bServer = wizard.FindSymbol("MINI_SERVER");
		var bDynamicMFC = wizard.FindSymbol("DYNAMIC_MFC");

		if (!bServer)
		{
			bServer = wizard.FindSymbol("FULL_SERVER");
			if (!bServer)
				bServer = wizard.FindSymbol("CONTAINER_SERVER");
		}

		var config = proj.Object.Configurations("Debug");
		config.CharacterSet = charSetMBCS;

		var CLTool = config.Tools("VCCLCompilerTool");

		if (bDynamicMFC)
		{
			config.UseOfMFC = useMfcDynamic;
			CLTool.RuntimeLibrary = rtMultiThreadedDebugDLL;
		}
		else
		{
			config.UseOfMFC = useMfcStatic;
			CLTool.RuntimeLibrary = rtMultiThreadedDebug;
		}
		var strDefines = GetPlatformDefine(config);
		strDefines += "_WINDOWS;_DEBUG";
		CLTool.PreprocessorDefinitions= strDefines;
		CLTool.TreatWChar_tAsBuiltInType = true;
		CLTool.DebugInformationFormat = debugEditAndContinue;

		var LinkTool = config.Tools("VCLinkerTool");
		LinkTool.LinkIncremental = linkIncrementalYes;
		LinkTool.GenerateDebugInformation = true;

		var bDBSupportHeaderOnly = wizard.FindSymbol("DB_SUPPORT_HEADER_ONLY");

		var bOLEDB = wizard.FindSymbol("OLEDB");
		var bSupportOLEDB = wizard.FindSymbol("DB_SUPPORT_OLEDB");
		if (bOLEDB || (bDBSupportHeaderOnly && bSupportOLEDB))
		{
			LinkTool.AdditionalDependencies = "msdasc.lib";
		}

		var bODBC = wizard.FindSymbol("ODBC");
		var bSupportODBC = wizard.FindSymbol("DB_SUPPORT_ODBC");
		if (bODBC || (bDBSupportHeaderOnly && bSupportODBC))
		{
			LinkTool.AdditionalDependencies = "odbc32.lib";
		}

		if (!bDynamicMFC)
		{
			LinkTool.IgnoreDefaultLibraryNames = "libcd.lib"
		}

		var MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = "_DEBUG";

		if (bAutomation)
			MidlTool.TypeLibraryName = "$(IntDir)/" + "$(ProjectName).tlb";

		var RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = rcEnglishUS;
		RCTool.PreprocessorDefinitions = "_DEBUG";
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";

		if (bServer || bAutomation)
		{
			var PostBuildTool = config.Tools("VCPostBuildEventTool");
			var L_PerformingRegistration1_Text = "Performing registration";
			PostBuildTool.Description = L_PerformingRegistration1_Text;
			PostBuildTool.CommandLine = "\"$(TargetPath)\" /RegServer";
		}

		config = proj.Object.Configurations("Release");
		config.CharacterSet = charSetMBCS;
		CLTool = config.Tools("VCCLCompilerTool");

		if (bDynamicMFC)
		{
			config.UseOfMFC = useMfcDynamic;
			CLTool.RuntimeLibrary = rtMultiThreadedDLL;
		}
		else
		{
			config.UseOfMFC = useMfcStatic;
			CLTool.RuntimeLibrary = rtMultiThreaded;
		}
		strDefines = GetPlatformDefine(config);
		strDefines += "_WINDOWS;NDEBUG";
		CLTool.PreprocessorDefinitions= strDefines;
		CLTool.InlineFunctionExpansion = expandOnlyInline;
		CLTool.MinimalRebuild = false;
		CLTool.TreatWChar_tAsBuiltInType = true;
		CLTool.DebugInformationFormat = debugEnabled;

		LinkTool = config.Tools("VCLinkerTool");
		LinkTool.GenerateDebugInformation = true;
		LinkTool.LinkIncremental = linkIncrementalNo;
		if (bOLEDB)
		{
			LinkTool.AdditionalDependencies = "msdasc.lib";
		}
		if (!bDynamicMFC)
		{
			LinkTool.IgnoreDefaultLibraryNames = "libc.lib"
		}


		MidlTool = config.Tools("VCMidlTool");
		MidlTool.MkTypLibCompatible = false;
		MidlTool.PreprocessorDefinitions = "NDEBUG";
		if (bAutomation)
			MidlTool.TypeLibraryName = "$(IntDir)/" + "$(ProjectName).tlb";

		RCTool = config.Tools("VCResourceCompilerTool");
		RCTool.Culture = rcEnglishUS;
		RCTool.PreprocessorDefinitions = "NDEBUG";
		RCTool.AdditionalIncludeDirectories = "$(IntDir)";

		if (bServer || bAutomation)
		{
			var PostBuildTool = config.Tools("VCPostBuildEventTool");
			var L_PerformingRegistration2_Text = "Performing registration";
			PostBuildTool.Description = L_PerformingRegistration2_Text;
			PostBuildTool.CommandLine = "\"$(TargetPath)\" /RegServer";
		}
	}
	catch(e)
	{
		throw e;
	}
}

function GetTargetName(strName, strProjectName, strResPath, strHelpPath)
{
	try
	{
		var strTarget = strName;
		var strSafeProjectName = wizard.FindSymbol("SAFE_PROJECT_NAME");
		if (strName.substr(0, 4) == "root")
		{
			var strlen = strName.length;
			if (strName == "root.ico" || strName == "root.rc2" || strName == "root.manifest")
			{
				strTarget = strResPath + "\\" + strProjectName + strName.substr(4, strlen - 4);
			}
			else if (strName == "root.hpj" || strName == "root.cnt")
			{
				strTarget = strProjectName + strName.substr(4, strlen - 4);
				strTarget = strHelpPath + "\\" + strTarget;
			}
			else if (strName == "root.hhc" || 
				strName == "root.hhk" || 
				strName == "root.hhp")
			{
				strTarget = strSafeProjectName + strName.substr(4, strlen - 4);
				strTarget = strHelpPath + "\\" + strTarget;
			}
			else
			{
				strTarget = strProjectName + strName.substr(4, strlen - 4);
			}
			return strTarget;
		}
		if (strName.substr(0, 7) == "dlgroot")
		{
			var strExtension = strName.substr(7, strName.length - 7);

			if (strName == "dlgroot.cnt")
			{
				strTarget = strHelpPath + "\\" + strProjectName + strExtension;
			}
			else if (strName == "dlgroot.hhc")
			{
				strTarget = strHelpPath + "\\" + strSafeProjectName + strExtension;
			}
			else
			{
				strTarget = strProjectName + strExtension;
			}

			return strTarget;
		}

		switch (strName)
		{
			case "readme.txt":
				strTarget = "ReadMe.txt";
				break;
			case "all.rc":
			case "dlgall.rc":
				strTarget = strProjectName + ".rc";
				break;
			case "dlgres.h":
			case "resource.h":
				strTarget = "Resource.h";
				break;
			case "dialog.h":
				strTarget = wizard.FindSymbol("DIALOG_HEADER");
				break;
			case "dialog.cpp":
				strTarget = wizard.FindSymbol("DIALOG_IMPL");
				break;
			case "dlgproxy.h":
				strTarget = wizard.FindSymbol("DIALOG_AUTO_PROXY_HEADER");
				break;
			case "dlgproxy.cpp":
				strTarget = wizard.FindSymbol("DIALOG_AUTO_PROXY_IMPL");
				break;
			case "frame.h":
				strTarget = wizard.FindSymbol("MAIN_FRAME_HEADER");
				break;
			case "frame.cpp":
				strTarget = wizard.FindSymbol("MAIN_FRAME_IMPL");
				break;
			case "childfrm.h":
				strTarget = wizard.FindSymbol("CHILD_FRAME_HEADER");
				break;
			case "childfrm.cpp":
				strTarget = wizard.FindSymbol("CHILD_FRAME_IMPL");
				break;
			case "doc.h":
				strTarget = wizard.FindSymbol("DOC_HEADER");
				break;
			case "doc.cpp":
				strTarget = wizard.FindSymbol("DOC_IMPL");
				break;
			case "view.h":
				strTarget = wizard.FindSymbol("VIEW_HEADER");
				break;
			case "view.cpp":
				strTarget = wizard.FindSymbol("VIEW_IMPL");
				break;
			case "wndview.h":
				strTarget = wizard.FindSymbol("WND_VIEW_HEADER");
				break;
			case "wndview.cpp":
				strTarget = wizard.FindSymbol("WND_VIEW_IMPL");
				break;
			case "treeview.h":
				strTarget = wizard.FindSymbol("TREE_VIEW_HEADER");
				break;
			case "treeview.cpp":
				strTarget = wizard.FindSymbol("TREE_VIEW_IMPL");
				break;
			case "recset.h":
				strTarget = wizard.FindSymbol("ROWSET_HEADER");
				break;
			case "recset.cpp":
				strTarget = wizard.FindSymbol("ROWSET_IMPL");
				break;
			case "srvritem.h":
				strTarget = wizard.FindSymbol("SERVER_ITEM_HEADER");
				break;
			case "srvritem.cpp":
				strTarget = wizard.FindSymbol("SERVER_ITEM_IMPL");
				break;
			case "ipframe.h":
				strTarget = wizard.FindSymbol("INPLACE_FRAME_HEADER");
				break;
			case "ipframe.cpp":
				strTarget = wizard.FindSymbol("INPLACE_FRAME_IMPL");
				break;
			case "cntritem.h":
				strTarget = wizard.FindSymbol("CONTAINER_ITEM_HEADER");
				break;
			case "cntritem.cpp":
				strTarget = wizard.FindSymbol("CONTAINER_ITEM_IMPL");
				break;
			case "doc.ico":
				strTarget = strResPath + "\\" + strProjectName + "Doc.ico";
				break;
			case "paneicons.bmp":
				strTarget = strResPath + "\\" + strTarget;
				break;
			case "afxdlg.rtf":
				strTarget = strHelpPath + "\\" + strTarget;
				break;
			case "afxdlg.htm":
			case "afxcore.htm":
				strTarget = strHelpPath + "\\" + strTarget;
				break;
			case "HIDD_ROOT_DIALOG.htm":
			case "afxo0001.htm":
			case "afx_hidd_changeicon.htm":
			case "afx_hidd_color.htm":
			case "afx_hidd_convert.htm":
			case "afx_hidd_editlinks.htm":
			case "afx_hidd_fileopen.htm":
			case "afx_hidd_filesave.htm":
			case "afx_hidd_find.htm":
			case "afx_hidd_font.htm":
			case "afx_hidd_insertobject.htm":
			case "afx_hidd_newtypedlg.htm":
			case "afx_hidd_pastespecial.htm":
			case "afx_hidd_print.htm":
			case "afx_hidd_printdlg.htm":
			case "afx_hidd_printsetup.htm":
			case "afx_hidd_replace.htm":
			case "afx_hidp_default.htm":
			case "afx_hidw_dockbar_top.htm":
			case "afx_hidw_preview_bar.htm":
			case "afx_hidw_resize_bar.htm":
			case "afx_hidw_status_bar.htm":
			case "afx_hidw_toolbar.htm":
			case "hidr_doc1type.htm":
			case "hid_app_about.htm":
			case "hid_app_exit.htm":
			case "hid_context_help.htm":
			case "hid_edit_clear.htm":
			case "hid_edit_clear_all.htm":
			case "hid_edit_copy.htm":
			case "hid_edit_cut.htm":
			case "hid_edit_find.htm":
			case "hid_edit_paste.htm":
			case "hid_edit_paste_link.htm":
			case "hid_edit_redo.htm":
			case "hid_edit_repeat.htm":
			case "hid_edit_replace.htm":
			case "hid_edit_undo.htm":
			case "hid_file_close.htm":
			case "hid_file_mru_file1.htm":
			case "hid_file_new.htm":
			case "hid_file_open.htm":
			case "hid_file_page_setup.htm":
			case "hid_file_print.htm":
			case "hid_file_print_preview.htm":
			case "hid_file_print_setup.htm":
			case "hid_file_save.htm":
			case "hid_file_save_as.htm":
			case "hid_file_save_copy_as.htm":
			case "hid_file_send_mail.htm":
			case "hid_file_update.htm":
			case "hid_help_index.htm":
			case "hid_help_using.htm":
			case "hid_ht_caption.htm":
			case "hid_ht_nowhere.htm":
			case "hid_next_pane.htm":
			case "hid_ole_edit_links.htm":
			case "hid_ole_insert_new.htm":
			case "hid_ole_verb_1.htm":
			case "hid_prev_pane.htm":
			case "hid_record_first.htm":
			case "hid_record_last.htm":
			case "hid_record_next.htm":
			case "hid_record_prev.htm":
			case "hid_sc_close.htm":
			case "hid_sc_maximize.htm":
			case "hid_sc_minimize.htm":
			case "hid_sc_move.htm":
			case "hid_sc_nextwindow.htm":
			case "hid_sc_prevwindow.htm":
			case "hid_sc_restore.htm":
			case "hid_sc_size.htm":
			case "hid_sc_tasklist.htm":
			case "hid_view_ruler.htm":
			case "hid_view_status_bar.htm":
			case "hid_view_toolbar.htm":
			case "hid_window_all.htm":
			case "hid_window_arrange.htm":
			case "hid_window_cascade.htm":
			case "hid_window_new.htm":
			case "hid_window_split.htm":
			case "hid_window_tile.htm":
			case "hid_window_tile_horz.htm":
			case "hid_window_tile_vert.htm":
			case "main_index.htm":
			case "menu_edit.htm":
			case "menu_file.htm":
			case "menu_help.htm":
			case "menu_record.htm":
			case "menu_view.htm":
			case "menu_window.htm":
			case "scrollbars.htm":
				strTarget = strHelpPath + "\\" + strTarget;
				break;
			default:
				break;
		}

		var strTemp = GetAdditionalPath(strTarget, strResPath, strHelpPath);


		strTarget = strTemp;
		return strTarget; 
	}
	catch(e)
	{
		throw e;
	}
}

function GetAdditionalPath(strName, strResPath, strHelpPath)
{
	try
	{
		var strFullName = strName;
		switch(strName)
		{
			case "afxcore.rtf":
			case "afxprint.rtf":
			case "afxolecl.rtf":
			case "afxolesv.rtf":
			case "afxdb.rtf":
				strFullName = strHelpPath + "\\" + strName;
				break;
			case "Bullet.gif":
				strFullName = strHelpPath + "\\Images\\" + strName;
				break;
			case "tbdh_.bmp":
			case "tbd__.bmp":
			case "tbrh_.bmp":
			case "tbr__.bmp":
			case "tbah_.bmp":
			case "tba__.bmp":
			case "tbedh.bmp":
			case "tbed_.bmp":
			case "tbeah.bmp":
			case "tbea_.bmp":
			case "tbndm.bmp":
			case "tbndmh.bmp":
			case "tbnds.bmp":
			case "tbndsh.bmp":
				strFullName = strResPath + "\\Toolbar.bmp";
				break;
			case "tba_i.bmp":
			case "tbrhi.bmp":
			case "tbr_i.bmp":
			case "tbahi.bmp":
				strFullName = strResPath + "\\IToolbar.bmp";
				break;
			case "bullet.bmp":
				strFullName = "";  // Help image files - will be added to the project in AddHelpImages
				break;
			default:
				break;
		}
		return strFullName;
	}
	catch(e)
	{
		throw e;
	}
}

function AddHelpBuildSteps(projObj, strProjectName, bHTMLHelp)
{
	try
	{
		var fileExt;
		var outFileExt;
		var fileTool1 = "";
		var fileTool2 = "";
		var extraMakehmSwitch = "";
		var extraMakehmSwitch2 = "";
		var strCodeTool1 = new Array();
		var strCodeTool2 = new Array();
		var strHmFileName;
		var strHmFileNameNoQuotes;
		var strHmHeaderComment;
		var strSafeProjectName = wizard.FindSymbol("SAFE_PROJECT_NAME");

		if(bHTMLHelp)
		{
			fileExt = ".hhp";
			outFileExt = ".chm";
			strHmFileName = "\"hlp\\HTMLDefines.h\"";
			strHmFileNameNoQuotes = "hlp\\HTMLDefines.h";
			extraMakehmSwitch = "/h ";
			extraMakehmSwitch2 = "/a afxhh.h "
			var L_strCodeFragment1_Text = "Generated Help Map file.  Used by ";
			strHmHeaderComment = L_strCodeFragment1_Text + strSafeProjectName + ".HHP.";

			// put together the command line to apply to the .hhp file
			strCodeTool1[0] = "start /wait hhc \"hlp\\" + strSafeProjectName + ".hhp\"";
			strCodeTool1[1] = "if not exist \"hlp\\" + strSafeProjectName + ".chm\" goto :HelpError";
			strCodeTool1[2] = "copy \"hlp\\" + strSafeProjectName + ".chm\" \"$(OutDir)\\" + strProjectName + ".chm\"";
			strCodeTool1[3] = "goto :HelpDone";
			strCodeTool1[4] = ":HelpError";
			var strLine1 = "echo hlp\\" + strSafeProjectName + ".hhp(1) : error:";
			var L_strCodeFragment2_Text = "Problem encountered creating help file";
			strLine1 += L_strCodeFragment2_Text;
			strCodeTool1[5] = strLine1;
			strCodeTool1[6] = "echo.";
			strCodeTool1[7] = ":HelpDone";
			strCodeTool1[8] = "echo.";
			var idx;
			for (idx=0; idx<9; idx++)
				fileTool1 += strCodeTool1[idx] + "\r\n";
		}
		else
		{
			fileExt = ".hpj";
			outFileExt = ".hlp";
			strHmFileName = "\"hlp\\$(ProjectName).hm\"";
			strHmFileNameNoQuotes = "hlp\\$(ProjectName).hm";
			var L_strCodeFragment3_Text = "Generated Help Map file.  Used by $(ProjectName).hpj.";
			strHmHeaderComment = L_strCodeFragment3_Text;

			// put together the command line to apply to the .hpj file
			strCodeTool1[0]  = "start /wait hcw /C /E /M \"hlp\\$(ProjectName).hpj\"";
			strCodeTool1[1]  = "if errorlevel 1 goto :HelpError";
			strCodeTool1[2]  = "if not exist \"hlp\\$(ProjectName).hlp\" goto :HelpError";
			strCodeTool1[3]  = "if not exist \"hlp\\$(ProjectName).cnt\" goto :HelpError";
			strCodeTool1[4]  = "echo.";
			strCodeTool1[5]  = "copy \"hlp\\$(ProjectName).hlp\" \"$(OutDir)\"";
			strCodeTool1[6]  = "copy \"hlp\\$(ProjectName).cnt\" \"$(OutDir)\"";
			strCodeTool1[7]  = "goto :HelpDone";
			strCodeTool1[8]  = ":HelpError";
			var strLine1 = "echo hlp\\$(ProjectName).hpj(1) : error: ";
			var L_strWinCodeToolFragment1_Text = "Problem encountered creating help file";
			strLine1 += L_strWinCodeToolFragment1_Text;
			strCodeTool1[9]  = strLine1;
			strCodeTool1[10] = ":HelpDone";
			strCodeTool1[11] = "echo.";
			var idx;
			for (idx=0; idx<12; idx++)
				fileTool1 += strCodeTool1[idx] + "\r\n";
		}

		// put together the command line to apply to the resource.h file
		var strLine = "echo // ";
		strLine += strHmHeaderComment;
		strLine += " > " + strHmFileName;
		strCodeTool2[0]  = strLine;
		strCodeTool2[1]  = "echo. > " + strHmFileName;
		strLine = "echo // ";
		var L_strCodeFragment4_Text = "Commands (ID_* and IDM_*)";
		strLine += L_strCodeFragment4_Text;
		strLine += " >> " + strHmFileName;
		strCodeTool2[2]  = strLine;
		strCodeTool2[3]  = "makehm " + extraMakehmSwitch + "ID_,HID_,0x10000 IDM_,HIDM_,0x10000 \"$(InputFileName)\" >> " + strHmFileName;
		strCodeTool2[4]  = "echo. >> " + strHmFileName;
		strLine = "echo // ";
		var L_strCodeFragment5_Text = "Prompts (IDP_*)";
		strLine += L_strCodeFragment5_Text;
		strLine += " >> " + strHmFileName;
		strCodeTool2[5]  = strLine;
		strCodeTool2[6]  = "makehm " + extraMakehmSwitch + "IDP_,HIDP_,0x30000 \"$(InputFileName)\" >> " + strHmFileName;
		strCodeTool2[7]  = "echo. >> " + strHmFileName;
		strLine = "echo // ";
		var L_strCodeFragment6_Text = "Resources (IDR_*)";
		strLine += L_strCodeFragment6_Text;
		strLine += " >> " + strHmFileName;
		strCodeTool2[8]  = strLine;
		strCodeTool2[9]  = "makehm " + extraMakehmSwitch + "IDR_,HIDR_,0x20000 \"$(InputFileName)\" >> " + strHmFileName;
		strCodeTool2[10] = "echo. >> " + strHmFileName;
		strLine = "echo // ";
		var L_strCodeFragment7_Text = "Dialogs (IDD_*)";
		strLine += L_strCodeFragment7_Text;
		strLine += " >> " + strHmFileName;
		strCodeTool2[11] = strLine;
		strCodeTool2[12] = "makehm " + extraMakehmSwitch + "IDD_,HIDD_,0x20000 \"$(InputFileName)\" >> " + strHmFileName;
		strCodeTool2[13] = "echo. >> " + strHmFileName;
		strLine = "echo // ";
		var L_strCodeFragment8_Text = "Frame Controls (IDW_*)";
		strLine += L_strCodeFragment8_Text;
		strLine += " >> " + strHmFileName;
		strCodeTool2[14] = strLine;
		strCodeTool2[15] = "makehm " + extraMakehmSwitch + extraMakehmSwitch2 + "IDW_,HIDW_,0x50000 \"$(InputFileName)\" >> " + strHmFileName;
		for (idx=0; idx<16; idx++)
			fileTool2 += strCodeTool2[idx] + "\r\n";

		var oPch = projObj.Files("StdAfx.cpp");
		if (oPch != null)
		{
			for (var n=1; n<=oPch.FileConfigurations.Count; n++)
			{
				var config = oPch.FileConfigurations.Item(n)
				config.Tool.UsePrecompiledHeader = pchCreateUsingSpecific;
			}
		}
		
		var fileObj1;
		if(bHTMLHelp)
		{
			fileObj1 = projObj.Files(strSafeProjectName + fileExt);
		}
		else
		{
			fileObj1 = projObj.Files(strProjectName + fileExt);
		}
		var fileObj2 = projObj.Files("resource.h");
			
		if (fileObj1 != null)
		{
			for (var i=1; i<=fileObj1.FileConfigurations.Count; i++)
			{
				var config = fileObj1.FileConfigurations.Item(i);
				if (config != null)
				{
					var CustomBuildTool = config.Tool;	
					CustomBuildTool.Outputs = "$(OutDir)\\$(ProjectName)" + outFileExt;
					CustomBuildTool.AdditionalDependencies = strHmFileNameNoQuotes;
					CustomBuildTool.CommandLine = fileTool1;
					L_ToolDesc1_Text = "Making help file...";
					CustomBuildTool.Description = L_ToolDesc1_Text;
				}
			}
		}

		if (fileObj2 != null)
		{
			for (var i=1; i<=fileObj2.FileConfigurations.Count; i++)
			{
				var config = fileObj2.FileConfigurations.Item(i);
				if (config != null)
				{
					var CustomBuildTool = config.Tool;	
					CustomBuildTool.Outputs = strHmFileNameNoQuotes;
					CustomBuildTool.CommandLine = fileTool2;
					L_ToolDesc2_Text = "Generating map file for help compiler...";
					CustomBuildTool.Description = L_ToolDesc2_Text;
				}
			}
		}
	}
	catch(e)
	{
		throw e;
	}
}

function AddHelpImages(selProj, L_strHelpFiles_Text)
{
	try
	{
		var oHelpFolder = selProj.Object.Filters.Item(L_strHelpFiles_Text);
		strHelpPath = wizard.FindSymbol("HELP_PATH");
		oHelpFolder.AddFile(strHelpPath + "\\Images\\bullet.bmp");
	}
	catch(e)
	{
		throw e;
	}
}
