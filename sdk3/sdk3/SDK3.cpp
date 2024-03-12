
#include <stdio.h>
#include <TCHAR.h>
#include <windows.h>

/*
*
* 鼠标消息
* 键盘消息(hotkey热键)
* 快捷键消息
* 菜单消息
*
*
* 窗口消息
* 控件消息
* 自定义消息
*
*
* 客户区域(Client Aera)
* 非客户区域(Not Client Aera) NC
*
*/

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

LRESULT OnMove(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    int xPos = (int)(short)LOWORD(lParam);
    int yPos = (int)(short)HIWORD(lParam);
    TCHAR szBuf[MAXBYTE];
    wsprintf(szBuf, _T("xPos:%d yPos:%d"), xPos, yPos);
    MessageBox(NULL, szBuf, _T("51asm"), MB_OK);

    return TRUE;
}

//鼠标消息
LRESULT OnLButtonDown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int xPos = (int)(short)LOWORD(lParam);
    int yPos = (int)(short)HIWORD(lParam);
    TCHAR szBuf[MAXBYTE];
    wsprintf(szBuf, _T("[51asm] WM_LBUTTONDOWN  xPos:%d yPos:%d\n"), xPos, yPos);
    OutputDebugString(szBuf);
    //MessageBox(NULL, szBuf, _T("51asm"), MB_OK);
    return FALSE;
}

LRESULT OnLButtonUp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int xPos = (int)(short)LOWORD(lParam);
    int yPos = (int)(short)HIWORD(lParam);
    TCHAR szBuf[MAXBYTE];
    wsprintf(szBuf, _T("[51asm] File:%s Line:%d OnLButtonUp hwnd=%p uMsh=%p wParam=%p lParam:%p"
        " WM_LBUTTONUP  xPos:%d yPos:%d\n"), __FILE__, __LINE__, hwnd, uMsg, wParam, lParam, xPos, yPos);
    OutputDebugString(szBuf);
    //MessageBox(NULL, szBuf, _T("51asm"), MB_OK);
    return FALSE;
}

LRESULT OnLButtonDoubleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    int xPos = (int)(short)LOWORD(lParam);
    int yPos = (int)(short)HIWORD(lParam);
    TCHAR szBuf[MAXBYTE];
    wsprintf(szBuf, _T("[51asm] File:%s Line:%d OnLButtonDoubleClick hwnd=%p uMsh=%p wParam=%p lParam:%p"
        " WM_LBUTTONDBLCLK  xPos:%d yPos:%d\n"), __FILE__, __LINE__, hwnd, uMsg, wParam, lParam, xPos, yPos);
    OutputDebugString(szBuf);
    //MessageBox(NULL, szBuf, _T("51asm"), MB_OK);
    return FALSE;
}

//LRESULT OnMouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//    LRESULT lResult = FALSE;
//    switch (uMsg)
//    {
//    case WM_LBUTTONDOWN:
//        lResult = OnLButtonDown(hwnd, uMsg, wParam, lParam);
//        break;
//    }
//
//
//    return lResult;
//}

//键盘消息
LRESULT OnKeyDown(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //获取键盘状态
    TCHAR szBuf[MAXBYTE];

    if (wParam >= 'A' && wParam <= 'Z')
    {
        BYTE KeyState[256];
        if (!GetKeyboardState(KeyState))
            return TRUE;
        //键盘扫描码
        BYTE ScanCode = (UINT)lParam >> 16 & 0xff;

        WORD ch;

        ToAscii(wParam, ScanCode, KeyState, &ch, 0);
        //check


        wsprintf(szBuf, _T("[51asm] OnKeyDown %c\n"), ch);
    }
    else {
        wsprintf(szBuf, _T("[51asm] OnKeyDown VK:%x\n"), wParam);
    }

    OutputDebugString(szBuf);

    return TRUE;
}

LRESULT OnKeyUp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //获取键盘状态
    TCHAR szBuf[MAXBYTE];

    if (wParam >= 'A' && wParam <= 'Z')
    {
        BYTE KeyState[256];
        if (!GetKeyboardState(KeyState))
            return TRUE;
        //键盘扫描码
        BYTE ScanCode = (UINT)lParam >> 16 & 0xff;

        WORD ch;

        ToAscii(wParam, ScanCode, KeyState, &ch, 0);
        //check


        wsprintf(szBuf, _T("[51asm] OnKeyUp %c\n"), ch);

    }
    else {
        wsprintf(szBuf, _T("[51asm] OnKeyUp VK:%x\n"), wParam);

    }
    OutputDebugString(szBuf);


    return TRUE;
}

//字符消息
LRESULT OnChar(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    TCHAR szBuf[MAXBYTE];
    wsprintf(szBuf, _T("[51asm] OnChar VK:%c\n"), wParam);
    OutputDebugString(szBuf);

    return TRUE;
}

//热键
LRESULT OnHotKey(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    MessageBox(NULL, _T("OnHotKey"), _T("51ASM"), MB_OK);

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
        //case WM_MOVE:
        //    lResult = OnMove(hwnd, uMsg, wParam, lParam);
        //    break;
    case WM_LBUTTONDOWN:
        lResult = OnLButtonDown(hwnd, uMsg, wParam, lParam);
        break;
    case WM_LBUTTONUP:
        lResult = OnLButtonUp(hwnd, uMsg, wParam, lParam);
        break;
    case WM_LBUTTONDBLCLK:
        lResult = OnLButtonDoubleClick(hwnd, uMsg, wParam, lParam);
        break;
    case WM_KEYDOWN:
        lResult = OnKeyDown(hwnd, uMsg, wParam, lParam);
        break;
    case WM_KEYUP:
        lResult = OnKeyUp(hwnd, uMsg, wParam, lParam);
        break;
    case WM_CHAR:
        lResult = OnChar(hwnd, uMsg, wParam, lParam);
        break;
    case WM_HOTKEY:
        lResult = OnHotKey(hwnd, uMsg, wParam, lParam);
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
    //int nResult = MessageBox(NULL, TEXT("Hello World"), _T("51asm"), MB_OK);
    //if (nResult == 0)
    //{
    //    ShowErrorMsg();
    //    return 0;
    //}

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

    //3.显示更新窗口
    ShowWindow(hWnd, SW_SHOWNORMAL);

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