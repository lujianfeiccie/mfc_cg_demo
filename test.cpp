// test.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"
#include <vector>
#include <stack>
#include <map>
using namespace std;
typedef struct Point{
	int x;
	int y;
	Point(int a,int b){
		x=a;
		y=b;
	}
}Point;
typedef struct Edge{
	float v;
	float k;
	int maxy;
	Edge *next;
}Edge;

void drawline(HDC, int ,int ,int , int ,int );
void scan_seed_fill(HDC , int, int, int, int);
void drawmap(HDC, vector<Point>, int);
void full(HDC, vector<Point>, int);

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	char szClassName[]="MainClass";
	WNDCLASSEX wndclass;
    //用描述主窗口飞参数填充结构
	wndclass.cbSize=sizeof(wndclass);
	wndclass.style=CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc=MainWndProc;
	wndclass.cbClsExtra=0;
	wndclass.cbWndExtra=0;
	wndclass.hInstance=hInstance;
	wndclass.hIcon=::LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor=::LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground=(HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName=(LPSTR)IDR_TYPER;
	wndclass.lpszClassName=szClassName;
	wndclass.hIconSm=NULL;
	
	//注册主窗口
	::RegisterClassEx(&wndclass);
	
	//创建主窗口
	HWND hwnd=::CreateWindowEx(
		0,
		szClassName,
		"My First Window!",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
		
		);
	if(hwnd==NULL)
	{
		::MessageBox(NULL, "创建窗口出错!", "error", MB_OK);
		return -1;
	}
	//
	::ShowWindow(hwnd, nCmdShow);
	::UpdateWindow(hwnd);
	MSG msg;
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
	
}

LRESULT CALLBACK MainWndProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{

	static HDC hdc = NULL;
	PAINTSTRUCT ps;
	vector<Point>points;
	Point p[6]={
		Point(120,450),
		Point(120,200),
		Point(200,100),
		Point(510,300),
		Point(510,400),
		Point(250,300)
	};
	for(int i= 0;i < 6 ;i++){
		points.push_back(p[i]);
	}
	Point seed(200, 200);
	switch(message)
	{
	case WM_PAINT:
		{
			hdc=::BeginPaint(hwnd, &ps);
			return 0;
		}
	case WM_DESTROY:
		::PostQuitMessage(0);
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case ID_FILE_EXIT:
			::PostQuitMessage(0);
		case ID_SEED:
			if(!hdc)
				hdc=::BeginPaint(hwnd, &ps);
			else hdc=::GetDC(hwnd);
			//draw line
			drawmap(hdc,points,0x00ff0000);
			scan_seed_fill(hdc, seed.x, seed.y, 0x0000ff00, 0x00ff0000);
			::EndPaint(hwnd, &ps);
			return 0;
		case ID_SCAN:
			if(!hdc)
				hdc=::BeginPaint(hwnd, &ps);
			else hdc=::GetDC(hwnd);
			full(hdc,points,0x00ff0000);
			::EndPaint(hwnd, &ps);
			return 0;
		}
		return 0;
	}
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

////////////////////////////////////////////////////////////////
/*
* draw line
*/
void drawline(HDC hdc, int x1, int y1, int x2, int y2,int color)
{
	if (x1 > x2)
	{
		int k;
		k=x1;x1=x2;x2=k;
		k=y1;y1=y2;y2=k;
	}

	int dx, dy, x, y, e, i;
	float k;
	dx = x2 - x1;
	dy = y2 - y1;
	k = (float)dy / dx;
	e = -1 * dx;
	x = x1;
	y = y1;
	if(k<=1 && k>0){
		for(i = 0; i<=dx; i++)
		{
			::SetPixel(hdc, x, y, color);
			x = x + 1;
			e = e + 2 * dy;
			if (e >= 0){
				y = y + 1;
				e = e - 2 * dx;
			}
		}
	}else if(k>1){
		for(i = 0; i<=dy; i++)
		{
			::SetPixel(hdc, x, y, color);
			y = y + 1;
			e = e + 2 * dx;
			if (e >= 0){
				x = x + 1;
				e = e - 2 * dy;
			}
		}
	}
	else{
		dy = dy * (-1);
		y = y2;
		if(k>-0.5){
			for(i = 0; i<=dx; i++)
			{
				::SetPixel(hdc, x, y1 + y2- y, color);
				x = x + 1;
				e = e + 2 * dy;
				if (e >= 0){
					y = y + 1;
					e = e - 2 * dx;
				}
			}
		}
		else{
			for(i = 0; i<=dy; i++)
			{
				::SetPixel(hdc,x1 + x2 - x, y, color);
				y = y + 1;
				e = e + 2 * dx;
				if (e >= 0){
					x = x + 1;
					e = e - 2 * dy;
				}
			}
		}
		
	}
}
/*
*full map
*/

/////////////////////
void drawmap(HDC hdc, vector<Point>p, int color){
	for(vector<Point>::iterator i=p.begin(); i!=p.end()-1; i++){
		drawline(hdc, i->x, i->y, (i+1)->x, (i+1)->y, color);
	}
	drawline(hdc, p.begin()->x, p.begin()->y, (p.end()-1)->x, (p.end()-1)->y, color);
}
//////////////////////////////////种子填充//////////////////
void scan_seed_fill(HDC hdc, int a, int b, int color, int color2){
	int x=a,y=b,x0,xl,xr,flag,xnextspan;
	stack<Point> p;
	Point temp(0,0);
	p.push(Point(x,y));

	while(!p.empty()){
		temp=p.top();
		p.pop();
		x = temp.x;
		y = temp.y;
		::SetPixel(hdc, x, y, color);
		x0 = x+1;
		while((::GetPixel(hdc, x0, y))!=color2){
			::SetPixel(hdc, x0, y ,color);
			x0 = x0 + 1;
		}
		xr = x0 - 1;
		x0 = x - 1;
		while(::GetPixel(hdc ,x0, y)!=color2){
			::SetPixel(hdc, x0, y ,color);
			x0 = x0 - 1;
		}
		xl = x0 + 1;
		x0 = xl;
		y = y + 1;
		while(x0 <= xr){
			flag = 0;
			while((::GetPixel(hdc, x0, y)!=color2) && (::GetPixel(hdc, x0, y)!=color) && (x0 < xr)){
				if (flag == 0) flag = 1;
				x0 ++;
			}
			if(flag == 1){
				if((x0 == xr) && (::GetPixel(hdc, x0, y)!=color2) && (::GetPixel(hdc, x0, y)!=color))
					p.push(Point(x0, y));
				else
					p.push(Point(x0 -1, y));
				flag = 0;
			}
			xnextspan = x0;
			while(((::GetPixel(hdc, x0, y))== color2) || ((::GetPixel(hdc, x0, y))== color) && (x0<=xr))
				x0 ++;
			if(xnextspan == x0)
				x0 ++;
		}
	}


////////////////////////////
	x=a;y=b;
	p.push(Point(x,y));
	while(!p.empty()){
		temp=p.top();
		p.pop();
		x = temp.x;
		y = temp.y;
		::SetPixel(hdc, x, y, color);
		x0 = x+1;
		while((::GetPixel(hdc, x0, y))!=color2){
			::SetPixel(hdc, x0, y ,color);
			x0 = x0 + 1;
		}
		xr = x0 - 1;
		x0 = x - 1;
		while(::GetPixel(hdc ,x0, y)!=color2){
			::SetPixel(hdc, x0, y ,color);
			x0 = x0 - 1;
		}
		xl = x0 + 1;

		x0 = xl;
		y = y - 1;
		while(x0 <= xr){
			flag = 0;
			while((::GetPixel(hdc, x0, y)!=color2) && (::GetPixel(hdc, x0, y)!=color) && (x0 < xr)){
				if (flag == 0) flag = 1;
				x0 ++;
			}
			if(flag == 1){
				if((x0 == xr) && (::GetPixel(hdc, x0, y)!=color2) && (::GetPixel(hdc, x0, y)!=color))
					p.push(Point(x0, y));
				else
					p.push(Point(x0 -1, y));
				flag = 0;
			}
			xnextspan = x0;
			while(((::GetPixel(hdc, x0, y))== color2) || ((::GetPixel(hdc, x0, y))== color) && (x0<=xr))
				x0 ++;
			if(xnextspan == x0)
				x0 ++;
		}
	}
}
////////////////////////扫描线填充//////////////////////////////////////////
void initNET(map< int, Edge *> &NET, vector<Point> p){
	Edge *temp;
	map<int ,Edge *>::iterator iNET;
	int index;
	for(vector<Point>::iterator j=p.begin(); j!=p.end()-1 ; j++){
		temp = new Edge;		
		index = ( j->y > (j+1) -> y )? ( (j+1) -> y): (j->y);//index取两点最小Y值
		temp->v = ( j->y > (j+1) -> y )? ( (j+1) ->x ): (j->x);//v取两点低点x值
		temp->k = (j->x - (j+1)->x) / ( (j->y)*1.0 - (j+1)->y );//计算x和y的比值
		temp->maxy = ( j->y > (j+1) -> y )? (j->y ): ((j+1) -> y);//取两点高点y值
		temp->next = NULL;
		if( (iNET=NET.find(index)) == NET.end()){//map中不存在
			NET[index] = temp;//以两点最小Y值作为索引存放边对象
		}
		else{ //链接一个结点
			temp->next=iNET->second;
			NET[index] = temp;
		}
	}
	vector<Point>::iterator m = p.begin(),n = p.end()-1;
	temp = new Edge;
	index = ( m->y > n -> y )? ( n -> y): (m->y);
	temp->v = ( m->y > n -> y )? ( n ->x ): (m->x);
	temp->k = (m->x - n->x) / ( (m->y)*1.0 - n->y );
	temp->maxy = ( m->y > n -> y )? (m->y ): (n -> y);
	temp->next = NULL;
	if( (iNET=NET.find(index)) == NET.end()){
		NET[index] = temp;
	}
	else{
		temp->next=iNET->second;
		NET[index] = temp;
	}
	return;
}
int insertEdge(Edge *&head, Edge *&e){
	Edge *pre,*succ;
	pre = succ = head;
	if(head == NULL ){
		head = e; e->next = NULL;
	}
	else{
		while( succ != NULL && (succ->v < e->v)){
			pre=succ;succ=succ->next;
		}
		if( succ == NULL){
			pre->next=e;
			e->next=NULL;
		}
		else if((succ->v == e->v)&&( (e->v + e->k) > (succ->v + succ->k) )){
			e->next = succ->next;
			succ->next = e;
		}
		else if(succ == head){
			head = e;
			e->next = succ;
		}
		else{
			e->next = succ;
			pre ->next = e;
		}
	}
	return 1;
}
void insertAET(map<int , Edge *> &NET, Edge *&AET,int l){
	map<int ,Edge *>::iterator iNET;
	Edge *temp,*rec;
	if( (iNET=NET.find(l)) != NET.end()){
		temp = iNET->second;
		while(temp != NULL){
			rec=temp->next;
			insertEdge(AET, temp);
			temp = rec;
		}
		NET.erase(iNET);
	}

}
int delEdge(Edge *&head, int i){
	Edge *pre,*succ;
	pre = succ = head;
	if(head == NULL)return 0;
	while(succ != NULL){
		if( succ->maxy == i){
			if(succ==head) head=head->next;
			else {
				pre->next=succ->next;
				succ->next=NULL;
				succ = pre->next;
			}
		}else {
		pre=succ;
		succ=succ->next;
		}
	}
	return 1;
}
void drawpix(HDC hdc, int y, Edge * e, int color){
	Edge *temp=e;
	int x1,x2;
	while( temp!=NULL ){
		x1=(int)(temp->v + 0.5);
		if(temp->next==NULL)return;
		x2=(int)(temp->next->v + 0.5);
		for(int i=x1; i< x2; i++)::SetPixel(hdc, i, y, color);
		temp=temp->next->next;
	}
	return;
}
void increase(Edge *&head){
	Edge *temp=head;
	while(temp != NULL){
		temp->v = temp->v + temp->k;
		temp=temp->next;
	}
	return ;
}
/////////////////////
void full(HDC hdc, vector<Point>p, int color){
	drawmap(hdc, p, color);
	//***********INIT NET************//
	map< int, Edge *>NET;
	initNET(NET, p);
	//***********************************//
	//*******init ymin and AET***********//
	int ymin = NET.begin()->first;
	Edge *AET = NULL;
	//***********************************//
	//*************pack *****************//
	for(int l = ymin; l < 500; l++){
		//::SetPixel(hdc, 30, l, color);
		//add to AET
		insertAET(NET, AET, l);
		//del max
		delEdge(AET, l);
		//draw pix
		drawpix(hdc, l, AET, color);
		//increase
		increase(AET);
	}
	//***********************************//
	return;
}

