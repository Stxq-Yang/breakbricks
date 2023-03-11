#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#ifdef VC_CL_COMPILER
#pragma comment (lib,"gdi32.lib")
#endif
bool run=true;
bool init=false;
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
    void paint(HDC hdc){
        HBRUSH hBrush1 =  CreateSolidBrush(RGB(255,255,255));
        SelectObject(hdc, hBrush1);
        Ellipse(hdc,x+r,y+r,x-r,y-r);
        DeleteObject(hBrush1);
    }
};
class broad{
public:
    int x=285;
    int y=323;
    int l=70;
    int h=7;
    int xa=0;
    int ya=0;
    void paint(HDC hdc){
        x+=xa;
        y+=ya;
        RECT rect;
        rect.left=x;
        rect.top=y;
        rect.right=x+l;
        rect.bottom=y+h;
        HBRUSH hBrush1 =  CreateSolidBrush(RGB(255,255,255));
        FillRect(hdc,&rect,hBrush1);
        DeleteObject(hBrush1);
        xa=0;
        ya=0;
    }
};
class brick{
public:
    RECT rect;
    void paint(HDC hdc){
        HBRUSH hBrush1 =  CreateSolidBrush(RGB(255,255,0));
        FillRect(hdc,&rect,hBrush1);
        DeleteObject(hBrush1);
    }
};
bool operator==(brick a,const brick b){
    return a.rect.top==b.rect.top&&a.rect.left==b.rect.left&&a.rect.right==b.rect.right&&a.rect.bottom==b.rect.bottom;
}
ball Ball;
broad Broad;
HBITMAP hBackbuffer;
HDC hBackbufferDC;
bool start=false;
unsigned long long tick=0;
int score=0;
std::vector <RECT> rects;
std::vector <brick> Bricks;
unsigned long long sseed(){
    timeb t;
	ftime(&t);
	return t.time*1000+t.millitm;
}
void initgames(){
    Broad.x=285;
    Broad.y=323;
    Broad.l=70;
    Broad.h=7;
    Broad.xa=0;
    Broad.ya=0;
    Ball.x=320;
    Ball.y=320;
    Ball.r=5;
    Ball.sx=0;
    Ball.sy=0;
    score=0;
    Bricks.clear();
    std::vector <RECT>rectsc(rects);
    srand(sseed());
    for (unsigned int i=0;i<=rand()%rects.size();i++){
        srand(sseed());

        int j=rand()%rectsc.size();
        brick Brick;
        Brick.rect=rects[j];
        rectsc.erase(rectsc.begin()+j);
        Bricks.push_back(Brick);
    }
    start=true;

}
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		/* Upon destruction, tell the main thread to stop */
		case WM_KEYDOWN:{
            switch(wParam){
                case VK_RIGHT:{
                    if (Broad.x+80<640&&start){
                        Broad.xa+=10;
                    }else if(start){
                        Broad.xa+=640-70-(Broad.x+Broad.xa);
                    }
                    break;
                }case VK_LEFT:{
                    if (Broad.x-10>=0&&start){
                        Broad.xa-=10;
                    }
                    break;
                }case VK_SPACE:{
                    if((!start)&&init){
                        CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)initgames,NULL,true,0);

                    }
                    break;
                }
            }
            break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			run=false;
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
    wc.hbrBackground = CreateSolidBrush(0xFFFFFF);
    wc.lpszClassName = "WindowClass";
    wc.hIcon		 = LoadIcon(0, Icon); /* Load a standard icon */
    wc.hIconSm		 = LoadIcon(0, Icon); /* use the name "A" to use the project icon */

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
    HDC hdc = GetDC(hwnd);
    hBackbuffer= CreateCompatibleBitmap(hdc, 640, 480);
    hBackbufferDC=CreateCompatibleDC(hdc);
    std::vector <int>xlis;
    for (int i=20;i<=640-20;i+=20){
        xlis.push_back(i);
    }
    std::vector <int>ylis;
    for (int i=10;i<=200-10;i+=10){
        ylis.push_back(i);
    }

    for (auto x:xlis){
        for (auto y:ylis){
            RECT rect;
            rect.top=y;
            rect.left=x;
            rect.bottom=y+10;
            rect.right=x+20;
            rects.push_back(rect);
        }
    }
    int FPS=0;
    init=true;
    unsigned long long lasttick=0;
    unsigned long long lasttime=time(0);
    while(run) {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }else{
            if (start){
                if (Ball.sx>=3)
                    Ball.sx=3;
                if (Ball.sy>=4)
                    Ball.sy=4;
                Ball.x-=Ball.sx;
                Ball.y-=Ball.sy;
                if ((Ball.x+Ball.r<= Broad.x+Broad.l && Ball.x-Ball.r>= Broad.x)
                    &&(Ball.y+Ball.r>=Broad.y&&Ball.y-Ball.r<=Broad.y+Broad.h)){
                    Ball.sx=-(2*Ball.x-2*Broad.x-Broad.l)/30;
                    Ball.sy=-Ball.sy+2 ;
                }
                if(Ball.x-Ball.r-Ball.sx<=0)
                    Ball.sx=-Ball.sx;
                if (Ball.x+Ball.r+Ball.sx>=640)
                    Ball.sx=3;
                if (Ball.y-Ball.r-Ball.sy<=0){
                    Ball.sy=-Ball.sy;
                }
                if (Ball.y+Ball.r>=480){
                    Broad.x=285;
                    Broad.y=3232 ;
                    Broad.l=70;
                    Broad.h=7;
                    Broad.xa=0;
                    Broad.ya=0;
                    Ball.x=320;
                    Ball.y=320;
                    Ball.r=5;
                    Ball.sx=0;
                    Ball.sy=0;
                    start=false;
                }
                if(Bricks.size()){
                    for (auto iter=Bricks.begin();iter!=Bricks.end();){
                        if(PtInRect(&iter->rect,{Ball.x+Ball.r,Ball.y+Ball.r})
                           ||PtInRect(&iter->rect,{Ball.x,Ball.y-Ball.r})
                           ||PtInRect(&iter->rect,{Ball.x-Ball.r,Ball.y})
                           ||PtInRect(&iter->rect,{Ball.x,Ball.y-Ball.r})){
                            iter=Bricks.erase(iter);
                            score+=1;
                        }else{
                            iter++;
                        }
                    }
                }else{
                    start=false;
                }



            }
            SelectObject(hBackbufferDC, hBackbuffer);
            std::wstring infos;
            infos=L"score:"+std::to_wstring(score)+L" FPS : "+std::to_wstring(FPS);
            HBRUSH Brush1 =  CreateSolidBrush(RGB(0,0,0));
            SelectObject(hBackbufferDC, Brush1);
            Rectangle(hBackbufferDC,0, 0, 640, 480);
            DeleteObject(Brush1);
            Ball.paint(hBackbufferDC);
            Broad.paint(hBackbufferDC);
            HFONT font=CreateFontW(
                            13,                                    //   字体的高度
                            0,                                          //   字体的宽度
                            0,                                          //  nEscapement
                            0,                                          //  nOrientation
                            FW_NORMAL,                                  //   nWeight
                            FALSE,                                      //   bItalic
                            FALSE,                                      //   bUnderline
                            0,                                                   //   cStrikeOut
                            ANSI_CHARSET,                             //   nCharSet
                            OUT_DEFAULT_PRECIS,                 //   nOutPrecision
                            CLIP_DEFAULT_PRECIS,               //   nClipPrecision
                            DEFAULT_QUALITY,                       //   nQuality
                            DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily
                            L"宋体");
            SelectObject(hBackbufferDC,font);
            SetTextColor(hBackbufferDC,RGB(255,255,255));
            SetBkMode(hBackbufferDC,TRANSPARENT);
            TextOutW(hBackbufferDC,20,400,infos.c_str(),infos.length());
            DeleteObject(font);
            for (auto i:Bricks){
                i.paint(hBackbufferDC);
            }

            BitBlt(hdc, 0, 0, 640, 480,
                    hBackbufferDC, 0, 0,
                    SRCCOPY);
            Sleep(10);
            unsigned long long nowtime=time(0);
            if(nowtime-lasttime>=1){
                FPS=tick-lasttick;
                lasttick=tick;
                lasttime=nowtime;
            }
            tick+=1;
        }
	}
    DeleteObject(hBackbuffer);
    ReleaseDC(hwnd, hdc); //retrieved device contexts are just released
	DeleteDC(hBackbufferDC);
    return 0;
}
#else
#error This application only can run and compile in windows
#endif
