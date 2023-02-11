#include <string>
#include <windows.h>
#ifdef VC_CL_COMPILER
#program comment (lib,"gdi32.lib")
#endif
bool run=true;
HINSTANCE hInstance = GetModuleHandle (0);
std::string title="Game2";
LPCSTR Icon= IDI_APPLICATION;
HWND hwnd;
WNDCLASSEX wc;
LPCSTR Cursor= IDC_ARROW;
class ball{
public:
    int x=320;
    int y=320;
    int r=5;
    int sx=0;
    int sy=0;
    paint(HDC hdc){
        HBRUSH hBrush1 =  CreateSolidBrush(RGB(255,255,255));
        SelectObject(hdc, hBrush1);
        Ellipse(hdc,x+r,y+r,x-r,y-r);
        DeleteObject(hBrush1);
    }
};
class broad{
public:
    int x=285;
    int y=325;
    int l=70;
    int h=7;
    paint(HDC hdc){
        RECT rect;
        rect.left=x;
        rect.top=y;
        rect.right=x+l;
        rect.bottom=y+h;
        HBRUSH hBrush1 =  CreateSolidBrush(RGB(255,255,255));
        FillRect(hdc,&rect,hBrush1);
        DeleteObject(hBrush1);
    }
};
ball Ball;
broad Broad;
void paint(HWND hwnd){
     PAINTSTRUCT ps;
     HDC hdc = BeginPaint(hwnd, &ps);
     Ball.paint(hdc);
     Broad.paint(hdc);
     EndPaint(hwnd, &ps);
}
int time=0;
int score=0;
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_PAINT: {
		    Ball.x-=Ball.sx;
		    Ball.y-=Ball.sy;
		    if (time%70==0)
                Ball.sy-=1;
		    if (((Ball.x+Ball.r<= Broad.x+Broad.l && Ball.x-Ball.r>= Broad.x)
                &&(Ball.y+Ball.r>=Broad.y&&Ball.y-Ball.r<=Broad.y+Broad.h)){
                Ball.sx=-Ball.sy;//+int((Ball.x-Broad.x)/4)
                Ball.sy=-Ball.sx;
            }
            if(Ball.x-Ball.r<=0||Ball.x+Ball.r>=640||Ball.y-Ball.r>=0){
                Ball.sx=-Ball.sy;
                Ball.sy=-Ball.sx;
            }
		    paint(hwnd);
			break;
		}
		/* Upon destruction, tell the main thread to stop */
		case WM_KEYDOWN:{
            switch(wParam){
                case VK_RIGHT:{
                    if (Broad.x+80<640){
                        Broad.x+=10;
                    }
                    break;
                }case VK_LEFT:{
                    if (Broad.x-10>=0){
                        Broad.x-=10;
                    }
                    break;
                }
            }
            break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}

		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow){


    memset(&wc,0,sizeof(wc));
    wc.cbSize		 = sizeof(WNDCLASSEX);
    wc.lpfnWndProc	 = (WNDPROC)WndProc; /* This is where we will send messages to */
    wc.hInstance	 = hInstance;
    wc.hCursor		 = LoadCursor(hInstance,Cursor);

    /* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
    wc.hbrBackground = CreateSolidBrush(0x000000);
    wc.lpszClassName = "WindowClass";
    wc.hIcon		 = LoadIcon(0, Icon); /* Load a standard icon */
    wc.hIconSm		 = LoadIcon(0, Cursor); /* use the name "A" to use the project icon */

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return -1;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass",title.c_str(),WS_VISIBLE|WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, /* x */
        CW_USEDEFAULT, /* y */
        640, /* width */
        480, /* height */
        NULL,NULL,hInstance,NULL);

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return -2;
    }
    MSG msg;
    while(run) {
        if(GetMessage(&msg, NULL, 0, 0)>0){
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }else{
            run=false;
        }
        RECT rectw;
        rectw.top=0;
        rectw.left=0;
        rectw.right=640;
        rectw.bottom=480;
        InvalidateRect(hwnd,&rectw,true);
        Sleep(3);
        time+=3;
	}

    return 0;
}
