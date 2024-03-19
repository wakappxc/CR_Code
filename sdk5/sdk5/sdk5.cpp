
#include <stdio.h>
#include <TCHAR.h>
#include <windows.h>
#include <string>
//using namespace std;

#ifdef _DEBUG
#define MYOUTPUT OutputDebugStringA
#else 
#define MYOUTPUT
#endif // DEBUG

#define IDM_OPEN 102
#define IDM_SAVE 103
#define IDM_EXIT 104
#define IDB_BUTTON1 105
#define IDB_BUTTON2 106
#define IDE_EDIT1 107
#define IDL_LISTBOX 107


#define MY_MSG WM_USER+1

HINSTANCE g_hInstance = NULL;




/*
* GDI API:
* 设备上下文（Device Context DC）
* 句柄（Handle HDC）
* 无效区域 ==> WM_PAINT
* 快捷键消息
* 菜单消息
* 定时器消息
*
* 命令消息：WM_COMMAND
* 1)菜单消息
* 2)快捷键消息
* 3)控件消息
*
*
* 高级控件不走WM_COMMAND 
*
* 控件消息(自定义消息)
*
*
* 资源
*
*/
std::string g_Text;
TEXTMETRIC  g_tm; //字体信息
void ShowErrorMsg()
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0,
        NULL
    );
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
    LocalFree(lpMsgBuf);
}

LRESULT OnCreate(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //初始化
    OutputDebugString(_T("[51asm] WM_CREATE\n"));

    //创建一个定时器
    SetTimer(hwnd, 1, 100, nullptr);

    return TRUE;
}

LRESULT OnClose(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //MessageBox(NULL, _T("OnClose"), _T("51asm"), MB_OK);
    DestroyWindow(hwnd);
    //PostQuitMessage(0);
    //PostMessage(hwnd,WM_QUIT, wParam,lParam);
    OutputDebugString(_T("[51asm] WM_CLOSE\n"));
    return FALSE;
}

LRESULT OnDestory(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //反初始化  释放资源
    //MessageBox(NULL, _T("OnDestory"), _T("51asm"), MB_OK);
    OutputDebugString(_T("[51asm] WM_DESTORY\n"));
    PostMessage(hwnd, WM_QUIT, wParam, lParam);

    //销毁定时器
    KillTimer(hwnd, 1);
    return TRUE;
}

LRESULT OnKeydown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    OutputDebugString(_T("[51asm] OnKeydown\n"));
    return TRUE;
}

LRESULT OnKeyup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    OutputDebugString(_T("[51asm] OnKeyup\n"));
    return TRUE;
}

//定时器消息
LRESULT OnTimer(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (wParam == 1) {
        HWND hDesktop = GetDesktopWindow();
        HDC hdc = GetDC(hDesktop);
        TextOut(hdc, 0, 0, "51asm", 5);
        ReleaseDC(hDesktop, hdc);
        //CloseHandle(hDesktop);
        MYOUTPUT("[51asm] OnTimer ID =1 \n");
    }
    else if (wParam == 2) {
        MYOUTPUT("[51asm] OnTimer ID =2 \n");
    }

    return TRUE;
}

//命令消息
LRESULT OnCommand(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(_T("[51asm] OnCommand\n"));
    WORD wID = LOWORD(wParam);
    WORD wNotifyCode = HIWORD(wParam);
    HWND hColtrol = (HWND)lParam;
    if (hColtrol != NULL) {//控件
        if (wID == IDB_BUTTON1) {
            if (wNotifyCode == BN_CLICKED) {
                LRESULT Result = SendMessage(hColtrol, BM_GETCHECK, 0, 0);
                if (BST_CHECKED == Result) {
                    SendMessage(hColtrol, BM_SETCHECK, BST_UNCHECKED, NULL);
                    MessageBox(NULL, "未婚", "婚姻状况", MB_OK);
                }
                else if (BST_UNCHECKED == Result) {
                    SendMessage(hColtrol, BM_SETCHECK, BST_CHECKED, NULL);
                    MessageBox(NULL, "已婚", "婚姻状况", MB_OK);
                }

            }
        }
        else if (wID == IDB_BUTTON2) {
            if (wNotifyCode == BN_CLICKED) {
                HWND hEdit = GetDlgItem(hwnd, IDE_EDIT1);
                char szText[] = { "" };
                //建议使用SendMessage  因为如果下面LPARAM是局部变量的话，Post传到消息队列，而局部变量已经清理的话会导致程序崩溃
                SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)szText);//直接调用窗口过程函数，直到过程函数返回
                SendMessage(hEdit, WM_PASTE, 0, 0);
            }
        }
        else if (wID == IDL_LISTBOX && wNotifyCode == LBN_SELCHANGE) {
            int nIndex = SendMessage(hColtrol, LB_GETCURSEL, 0, 0);
            if (nIndex != -1) {
                int nLen = SendMessage(hColtrol, LB_GETTEXTLEN, nIndex, 0);
                LPVOID lpBuff = malloc(nLen + 1);
                SendMessage(hColtrol, LB_GETTEXT, nIndex, (LPARAM)lpBuff);
                MessageBoxA(hwnd, (char*)lpBuff, "51asm", MB_OK);
                free(lpBuff);
            }
        }

        return TRUE;
    }

    switch (wID) {
    case IDM_OPEN:
        MessageBox(NULL, "打开", "51asm", MB_OK);
        break;
    case IDM_SAVE:
        MessageBox(NULL, "保存", "51asm", MB_OK);
        break;
    case IDM_EXIT:
        SendMessage(hwnd, MY_MSG, NULL, NULL);//直接发消息到过程函数
        //exit(0);
        //PostQuitMessage(0);
        PostMessage(hwnd, WM_QUIT, 0, 0);
        break;
    }

    //if (wNotifyCode == 1) {//快捷键

    //}
    //else if (wNotifyCode == 0) {//菜单
    //    switch (wID)
    //    {
    //    case IDM_OPEN:
    //        MessageBox(NULL, "打开", "51asm", MB_OK);
    //        break;
    //    case IDM_EXIT:
    //        PostQuitMessage(0);
    //        break;
    //    }
    //}
    //else if(lParam != NULL){//控件

    //}

    return TRUE;
}

LRESULT OnNotify(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    NMHDR* pnmh = (LPNMHDR)lParam;
    if (wParam == IDL_LISTBOX) {
    int nIndex = SendMessage(pnmh->hwndFrom, LB_GETCURSEL, 0, 0);
    if (nIndex != -1){
        int nLen = SendMessage(pnmh->hwndFrom, LB_GETTEXTLEN, nIndex, 0);
        LPVOID lpBuff = malloc(nLen + 1);
        SendMessage(pnmh->hwndFrom, LB_GETTEXT, nIndex, (LPARAM)lpBuff);
        MessageBoxA(hwnd, (char*)lpBuff, "51asm", MB_OK);
        free(lpBuff);
    }
    }
    OutputDebugString(_T("[51asm] OnNotify\n"));
    return TRUE;
}

//5.消息处理
LRESULT(CALLBACK WindowProc)(HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam) {
    LRESULT lResult = FALSE;
    switch (uMsg)
    {
    case WM_NOTIFY:
        lResult = OnNotify(hwnd, uMsg, wParam, lParam);
        break;

    case MY_MSG:
        MessageBox(NULL, "我的消息", "51asm", MB_OK);
        //SendMessage(hwnd, MY_MSG, NULL, NULL);
        break;
    case WM_CREATE:
        lResult = OnCreate(hwnd, uMsg, wParam, lParam);
        break;
    case WM_CLOSE:
        lResult = OnClose(hwnd, uMsg, wParam, lParam);
        break;
    case WM_DESTROY:
        lResult = OnDestory(hwnd, uMsg, wParam, lParam);
        break;
    case WM_COMMAND://绘制消息
        lResult = OnCommand(hwnd, uMsg, wParam, lParam);
        break;
    case WM_KEYDOWN:
        lResult = OnKeydown(hwnd, uMsg, wParam, lParam);
        break;
    case WM_KEYUP:
        lResult = OnKeyup(hwnd, uMsg, wParam, lParam);
        break;
    case WM_TIMER:
        lResult = OnTimer(hwnd, uMsg, wParam, lParam);
        break;
    }


    if (!lResult) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);//默认窗口过程处理函数 
    }
    return lResult;
}


BOOL InitControl(HWND hWnd) {
    HWND hButton1 = CreateWindowEx(0,
        "BUTTON",
        "已婚",
        WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
        0,
        0,
        100,
        50,
        hWnd,
        (HMENU)IDB_BUTTON1,
        g_hInstance,
        NULL
    );

    HWND hButton2 = CreateWindowEx(0,
        "BUTTON",
        "粘贴",
        WS_CHILD | WS_VISIBLE,
        100,
        0,
        100,
        50,
        hWnd,
        (HMENU)IDB_BUTTON2,
        g_hInstance,
        NULL
    );

    HWND hEdit = CreateWindowEx(0,
        "Edit",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL |
        ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL,
        0,
        60,
        200,
        100,
        hWnd,
        (HMENU)IDE_EDIT1,
        g_hInstance,
        NULL
    );

    HWND hListBox = CreateWindowEx(0,
        "ListBox",
        "编程语言列表",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_HASSTRINGS | LBS_NOTIFY,
        0,
        160,
        200,
        100,
        hWnd,
        (HMENU)IDL_LISTBOX,
        g_hInstance,
        NULL
    );
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"C语言");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"C++");
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)"C数据结构");
    return TRUE;
}


/*
*
* 图形界面 ==》 窗口
*/

int (WINAPI _tWinMain)(HINSTANCE hInstance,//应用程序实例句柄
    HINSTANCE hprevInstance,//保留
    TCHAR* lpCmdLine, //命令行参数 指针
    int nCmdShow)//窗口显示方式
{
    g_hInstance = hInstance;

    //申请局部堆地址空间
    //HLOCAL hMem = LocalAlloc(LHND,100); //LocalLock LocalFree
    //LPVOID lpMemory = LocalLock(hMem);
    //LocalFree(hMem);

    //申请全局堆地址空间
    //GlobalAlloc(LHND,100);

    //申请堆地址空间 malloc
    ACCEL* pAccelNews = (ACCEL*)HeapAlloc(GetProcessHeap(), 0, sizeof(ACCEL) * 2);
    if (pAccelNews == nullptr)
    {
        ShowErrorMsg();
        return 0;
    }

    pAccelNews[0].fVirt = FALT | FCONTROL | FVIRTKEY;
    pAccelNews[0].key = 'A';
    pAccelNews[0].cmd = WM_COMMAND;

    pAccelNews[1].fVirt = FALT | FCONTROL | FVIRTKEY;
    pAccelNews[1].key = 'B';
    pAccelNews[1].cmd = WM_COMMAND;

    //申请堆地址空间 可以指定内存属性
    //VirtualAlloc

    //创建快捷键表 
    HACCEL hAccel = CreateAcceleratorTable(pAccelNews, 2);
    if (hAccel == NULL)
    {
        ShowErrorMsg();
        return 0;
    }



    //1.注册窗口
    TCHAR szWndClassName[] = { _T("MYCLASSNAME") };

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;//窗口类型  垂直和水平
    wc.lpfnWndProc = WindowProc;//窗口过程函数（窗口回调函数-》处理消息）
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_HAND);//图标
    wc.hCursor = LoadCursor(NULL, IDC_HELP);//光标
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));//窗口背景颜色刷子
    wc.lpszMenuName = NULL;//菜单名称
    wc.lpszClassName = szWndClassName;//窗口类名

    if (RegisterClassEx(&wc) == 0) {
        ShowErrorMsg();
        return 0;
    }

    //2.创建窗口
    TCHAR szWndName[] = { _T("51asm") };
    //如果WindowProc返回为FALSE那么hWnd一定为0
    HWND hWnd = CreateWindowEx(0,
        szWndClassName,
        szWndClassName,
        WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (hWnd == NULL)
    {
        ShowErrorMsg();
        return 0;
    }

    HMENU hMenu = CreateMenu();
    //弹出菜单
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu, "文件(&F)");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hMenu, "编辑(&E)");

    SetMenu(hWnd,  // handle to window  
        hMenu // handle to menu
    );

    //添加子菜单
    HMENU hSubMenu = GetSubMenu(hMenu, 0);
    AppendMenu(hSubMenu, MF_STRING, IDM_OPEN, "打开(&O)");
    AppendMenu(hSubMenu, MF_STRING, IDM_SAVE, "保存(&S)");
    AppendMenu(hSubMenu, MF_STRING, IDM_EXIT, "退出(&E)");


    //创建子窗口(控件)
    InitControl(hWnd);

    //操作控件(发消息)


    //控件被操作会向父窗口发送通告消息BN_XXXX(Button Notify)



    //3.显示更新窗口
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    SetClassLong(hWnd, //window handle
        GCL_HCURSOR, //change cursor 使用X64此处宏定义会取消 也会失效
        (LONG)LoadCursor(NULL, IDC_CROSS));//new cursor

    //4.消息循环（消息队列）
    BOOL bRet;
    MSG msg;
    while ((bRet = GetMessage(&msg, hWnd, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            break;
        }
        else
        {
            //转换快捷键消息 WM_COMMAND
            if (!TranslateAccelerator(hWnd, hAccel, &msg))
            {
                TranslateMessage(&msg);//转换键盘消息为字符消息
                DispatchMessage(&msg);//派发消息
            }

        }
    }

    //删除快捷键表
    DestroyAcceleratorTable(hAccel);
    HeapFree(GetProcessHeap(), 0, pAccelNews);

    return msg.wParam;
}