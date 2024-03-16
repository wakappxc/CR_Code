
#include <stdio.h>
#include <TCHAR.h>
#include <windows.h>
#include <string>
//using namespace std;

#define IDM_OPEN 102
#define IDM_SAVE 103
#define IDM_EXIT 104


/*
* GDI API:
* 设备上下文（Device Context DC）
* 句柄（Handle HDC）
*
* 快捷键消息
* 菜单消息
* 窗口消息
* 控件消息
* 自定义消息
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
    //MessageBox(NULL, _T("OnCreate"), _T("51asm"), MB_OK);
    OutputDebugString(_T("[51asm] WM_CREATE\n"));
    RegisterHotKey(hwnd, GlobalAddAtom(_T("666")), MOD_ALT, VK_F1);

    HDC hdc = GetDC(hwnd);
    SelectObject(hdc,
        GetStockObject(SYSTEM_FIXED_FONT));
    GetTextMetrics(hdc, &g_tm);
    ReleaseDC(hwnd, hdc);

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
    UnregisterHotKey(hwnd, GlobalAddAtom(_T("666")));
    OutputDebugString(_T("[51asm] WM_DESTORY\n"));
    PostMessage(hwnd, WM_QUIT, wParam, lParam);
    return TRUE;
}




//字符消息
LRESULT OnChar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    TCHAR szBuf[MAXBYTE];
    if ((char)wParam == '\r') {
        g_Text += (char)wParam;
        g_Text += '\n';
    }
    else if ((char)wParam == '\b') {
        g_Text.pop_back();
    }
    else {
        g_Text += (char)wParam;
    }

    wsprintf(szBuf, _T("[51asm] OnChar VK:%s\n"), g_Text.data());
    OutputDebugString(szBuf);

    //获取窗口HDC
    //HDC hdc = GetWindowDC(hwnd); //非客户区域

    HDC hdc = GetDC(hwnd);
    //TextOut(hdc, 0, 0, g_Text.data(), g_Text.length());
    //获取窗口客户区域大小


    //创建一个白色刷子
   // HBRUSH hBrushOld;
    //HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));//窗口背景颜色刷子


    //DC选择刷子
    //hBrushOld = (HBRUSH)SelectObject(hdc, hBrush);

    //绘制背景
    //FillRect(hdc, &rc, hBrush);

    //BeginPaint

    //绘制文本


    //还原默认刷子
    //SelectObject(hdc, hBrushOld);

    //释放刷子
    //DeleteObject(hBrush);

    //释放DC
    //ReleaseDC(hwnd, hdc);

    //ShowCaret(hwnd);
    //SetCaretPos(g_tm.tmAveCharWidth * g_Text.length(), 0);

    RECT rc;
    GetClientRect(hwnd, &rc);

    //设置无效区域
    InvalidateRect(hwnd, NULL, TRUE);

    //DrawText(hdc, g_Text.data(), g_Text.length(), &rc, DT_LEFT);
    ReleaseDC(hwnd, hdc);
    return TRUE;
}

LRESULT OnSetFocus(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(_T("[51asm] OnSetFocus\n"));

    //创建插入符
    CreateCaret(hwnd, (HBITMAP)NULL, 1, g_tm.tmHeight);
    SetCaretPos(0, 0);
    ShowCaret(hwnd);

    return TRUE;
}

LRESULT OnKillFocus(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(_T("[51asm] OnKillFocus\n"));

    DestroyCaret();

    return TRUE;
}

LRESULT OnPaint(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(_T("[51asm] OnPaint\n"));

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);


    DrawText(hdc, g_Text.data(), g_Text.length(), &rc, DT_LEFT);

    EndPaint(hwnd, &ps);//自动化将无效区域设为有效区域

    ReleaseDC(hwnd, hdc);

    return TRUE;
}

LRESULT OnEraseBackground(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(_T("[51asm] OnEraseBackground\n"));


    return FALSE;
}

//命令消息
LRESULT OnCommand(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    OutputDebugString(_T("[51asm] OnCommand\n"));
    WORD wID = LOWORD(wParam);

    switch (wID)
    {
    case IDM_OPEN:
        MessageBox(NULL, "打开", "51asm", MB_OK);
        break;
    case IDM_EXIT:
        PostQuitMessage(0);
        break;

    }


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

    case WM_CREATE:
        lResult = OnCreate(hwnd, uMsg, wParam, lParam);
        break;
    case WM_CLOSE:
        lResult = OnClose(hwnd, uMsg, wParam, lParam);
        break;
    case WM_DESTROY:
        lResult = OnDestory(hwnd, uMsg, wParam, lParam);
        break;
    case WM_CHAR:
        lResult = OnChar(hwnd, uMsg, wParam, lParam);
        break;
    case WM_SETFOCUS:
        lResult = OnSetFocus(hwnd, uMsg, wParam, lParam);
        break;
    case WM_KILLFOCUS:
        lResult = OnKillFocus(hwnd, uMsg, wParam, lParam);
        break;
    case WM_PAINT:
        lResult = OnPaint(hwnd, uMsg, wParam, lParam);
        break;
    case WM_ERASEBKGND:
        lResult = OnEraseBackground(hwnd, uMsg, wParam, lParam);
        break;
    case WM_COMMAND:
        lResult = OnCommand(hwnd, uMsg, wParam, lParam);
        break;
    }


    if (!lResult) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);//默认窗口过程处理函数 
    }
    return lResult;
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
    //发送消息
    //SendMessage();//直接调用窗口过程函数  同步
    //PostMessage();//投递消息到消息队列    异步
   // HWND hNotepad = FindWindow("Notepad", NULL);
   // if (hNotepad == NULL) {
   //     return FALSE;
   // }
   // //PostMessage(hNotepad, WM_QUIT, 0, NULL); //消息队列而下面不是
   // //SendMessage(hNotepad, WM_QUIT, 0, NULL);
   // //SetFocus(hNotepad);
   // HWND hEdit = GetWindow(
   //     hNotepad,  // handle to original window
   //     GW_CHILD  // relationship
   // );
   // //SendMessage(hEdit, WM_KEYDOWN, 'A', 0);
   // PostMessage(hEdit, WM_KEYDOWN, 'A', 0);
   // PostMessage(hEdit, WM_KEYDOWN, 'B', 0);
   //{PostMessage(hEdit, WM_KEYDOWN, 'C', 0);}
   // HDC hdc = GetDC(hEdit);
   // while (true)
   // {
   //     SetTextColor(hdc,RGB(255,0,0));
   //     TextOut(hdc, 0, 0, "SB", 2);
   // }
   // ReleaseDC(hEdit, hdc);



    //程序 ==》 实例化 ==》 进程 ==》 多个窗口 

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
            TranslateMessage(&msg);//转换键盘消息为字符消息
            DispatchMessage(&msg);//派发消息
        }
    }



    return 0;
}