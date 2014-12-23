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

////////////////学习///////////////////////////////
void drawline_study(HDC, int ,int ,int ,int ,int );
void swap(int *x0,int *y0,int *x1,int *y1);
void drawvline_study(HDC, int,int,int,int);
void drawhline_study(HDC, int,int,int,int);

typedef struct Edge_Study{
	int maxy;
	int x;//x|miny
	float m; //1/k
}Edge_Study;
//初始化ET表
void initScanLineNewEdgeTable_Study(vector<vector<Edge_Study>>& ,Point* ,int,int,int);
void ScanLinePolygonFill(HDC,Point*,int,int);
void GetPolygonMinMax(Point*,int,int*,int*);
void HorizonEdgeFill(HDC,Point*,int,int);
void ProcessScanLineFill(HDC,vector<vector<Edge_Study>>&,int,int,int);
void InsertNetListToAet(vector<Edge_Study>,vector<Edge_Study>&);
void FillAETScanLine(HDC,vector<Edge_Study>, int, int);
void RemoveNonActiveEdgeFromAet(vector<Edge_Study>,int y);
void UpdateAndResortAet(vector<Edge_Study>);
void Sort(vector<Edge_Study>& );

void print(vector<Edge_Study> edge);
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

	vector<Edge_Study> edge,aet;
	Edge_Study edge1,edge2,edge3,edge4;
	edge1.x = 3;
	edge2.x = 4;
	edge3.x = 5;
	edge4.x = 1;
	edge.push_back(edge1);
	edge.push_back(edge2);
	edge.push_back(edge3);
	edge.push_back(edge4);
	print(edge);
	//InsertNetListToAet(edge,aet);
	Sort(edge);
	print(edge);
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
		case ID_MENU_ITEM_DRAW_LINE:
			{
				if(!hdc)
				hdc=::BeginPaint(hwnd, &ps);
				else hdc=::GetDC(hwnd);

			int x0 = 50,y0 = 300, x1 = x0 + 300, y1 = y0 + 100; //k=(0,1]

			 // x1 = x0, y1 = y0 + 300; //竖线
			 // x1 = x0+300, y1 = y0; //水平线
			//  x1 = x0+300, y1 = y0+350; //k>1
			//  x1 = x0+300, y1 = y0-150; //k=[-1,0)
			 x1 = x0+300, y1 = y0-500; //k<-1
			drawline_study(hdc,x0,y0,x1,y1,0x00000000);
			}
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
/**
	   3  |  2
		  |
	4     |     1 
-----------------------
	5	  |		8
		  |
	   6  |  7
*/
////////////////学习///////////////////////////////
void drawline_study(HDC hdc, int x0,int y0,int x1,int y1,int color)
{
	if( x0 > x1)               /* 保证x0<x1,即x0为x1左边的点。*/
    {
        swap(&x0,&y0,&x1,&y1);  
    }
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = (dy << 1) - dx;
	int d1 = (dy - dx) << 1;
	int d2 = dy << 1;
	int x=x0,y=y0;
	string str = "start";
	::TextOutA(hdc,x,y,str.c_str(),str.length());


	if(dx == 0)  /* 斜率为无穷大，画直竖线 */
    {
        drawvline_study(hdc,x0,y0,y1,color);
        return;
    }

	if(dy == 0)  /* 斜率为0，画直横线 */
    {
        drawhline_study(hdc,x0,y0,x1,color);
        return;
    }

	
	//1.  k=(0,1] 
	if((dx >= dy) && dy > 0)
	{
		d = (dy<<1) - dx;
		d1 = (dy - dx) << 1;
		d2 = dy << 1;
		for(x = x0;x < x1;x++)
		{
			::SetPixel(hdc,x,y,color);
			if(d > 0)
			{
			   ++y;
			   d += d1;
			}
			else
			{
			   d += d2;
			}
		}
		return;
	}

	//2. k >1 
	if( (dy > dx ) && dy >0 )
	{
		d = dy - (dx<<1);
		d1 =  (dy - dx) <<1;
		d2 =  dx << 1;
		for(y = y0;y < y1;y++)
		{
			::SetPixel(hdc,x,y,color);
			if(d < 0)
			{
			   ++x;
			   d += d1;
			}
			else
			{
			   d += -d2;
			}
		}
		return;
	}
	//8.
	if( ( dx>=abs(dy) ) && ( dy<0 ))           /* k=[-1,0) */
    {
		// d = (dy<<1) + dx;
		  d = (dy << 1) + dx;           /* d = 2a-b */
		  d1 = (dy + dx)<<1;
		  d2 =  dy << 1;
		 for(x = x0;x<x1;x++)
		 {
			::SetPixel(hdc,x,y,color);
			if(d<0)
			{
				--y;
				d += d1;
			}
			else
			{
				d += d2;
			}
		 }
	}
	//7.
	if( (abs(dy)>dx)&&(dy<0))           /* k<-1 */
    {
		  d = dy + (dx << 1);          /* d = a - 2b */
		  d1 = ( dy + dx )<<1;
		  d2 = dx << 1;
		  for(y = y0;y>y1;y--)
		  {
			::SetPixel(hdc,x,y,color);
		    if(d > 0)
            {
                d += d1;   /* 选择SE点 */
                x++;
            }
            else
            {
                d += d2;          /* 选择S点 */
            }
		  }
		  return;
	}
}

void swap(int *x0,int *y0,int *x1,int *y1)
{
   int x,y;
   x = *x0; *x0 = *x1; *x1 = x;
   y = *y0; *y0 = *y1; *y1 = y;
}

void drawvline_study(HDC hdc, int x0,int y0,int y1,int color)
{
	int x = x0;
	int y = y0;
	for(y = y0; y < y1; y++)
	{
		::SetPixel(hdc,x,y,color);
	}
}
void drawhline_study(HDC hdc, int x0,int y0,int x1,int color)
{
	int x = x0;
	int y = y0;
	for(x = x0; x < x1; x++)
	{
		::SetPixel(hdc,x,y,color);
	}
}
void ScanLinePolygonFill(HDC hdc,Point* pts,int SIZE_OF_PTS,int color)
{
	int min_y = 0;
    int max_y = 0;
	GetPolygonMinMax(pts,SIZE_OF_PTS,&min_y,&max_y);

	vector<vector<Edge_Study>> net(max_y - min_y + 1);

	initScanLineNewEdgeTable_Study(net,pts,SIZE_OF_PTS,min_y,max_y);

}
void GetPolygonMinMax(Point* pts,int SIZE_OF_PTS,int* min_y,int* max_y)
{
	int tmax_y = (pts + 0)->y;
	int tmin_y = (pts + 0)->y;
	for(int i=1;i<SIZE_OF_PTS;i++)
	{
		if((pts + i)->y > tmax_y)
		{
			tmax_y = (pts + i)->y;
		}
		if((pts + i)->y < tmin_y)
		{
			tmin_y = (pts + i)->y;
		}
	}
	*min_y = tmin_y;
	*max_y = tmax_y;
}

void initScanLineNewEdgeTable_Study(vector<vector<Edge_Study>>& net,Point* pts,int SIZE_OF_PTS,int ymin, int ymax)
{
	for(int i=0;i<SIZE_OF_PTS-1;i++)
	{
		Edge_Study model;
		Point p_start = *(pts + i);//起点
		Point p_end = *(pts + ((i + 1) % SIZE_OF_PTS) );//终点
		Point p_start_beside = *(pts + (i - 1 + SIZE_OF_PTS) % SIZE_OF_PTS );//起点邻点
		Point p_end_beside = *(pts + ((i + 2) % SIZE_OF_PTS) );//终点邻点

		if(p_end.y != p_start.y) //不处理水平线
		{
			model.m = double(p_end.x - p_start.x) / double(p_end.y - p_end.y);
			if(p_end.y > p_start.y) //终点的y值大于起点的y值
			{
				model.x = p_start.x; //取低点的x值
				if(p_end_beside.y >= p_end.y) //终点邻边的y值大于等于终点的y值(左顶点和右顶点)
				{
					model.maxy = p_end.y - 1;//平衡和交点奇偶个数
				}
				else
				{
					model.maxy = p_end.y;
				}
				net[p_start.y - ymin].push_back(model);//低点追加
			}
			else
			{
				model.x = p_end.x; //取低点的x值
				if(p_start_beside.y >= p_start.y)
				{
					model.maxy = p_start.y - 1;
				}
				else
				{
					model.maxy = p_start.y;
				}
				net[p_end.y - ymin].push_back(model); //低点追加
			}
		}
	}
}

void HorizonEdgeFill(HDC hdc,Point* pts,int SIZE_OF_PTS,int color)
{
	for(int i=0;i<SIZE_OF_PTS;i++)
	{
		Point p_start = *(pts + i);
		Point p_end = *(pts + ((i+1) % SIZE_OF_PTS) );
		if(p_start.y == p_end.y)
		{
			drawline_study(hdc,p_start.x,p_start.y,p_end.x,p_end.y,color);
		}
	}
}
void ProcessScanLineFill(HDC,vector<vector<Edge_Study>>& net,int ymin,int ymax,int color)
{
	vector<Edge_Study> AET;
	for(int y = ymin; y <= ymax; y++)
	{
		//InsertNetListToAet   //函数负责将扫描线对应的所有新边插入到aet中，
							   //插入操作到保证aet还是有序表

		//FillAetScanLine(aet, y, color);//填充

		//RemoveNonActiveEdgeFromAet(aet, y);//删除非活动边

		 //UpdateAndResortAet(aet);//更新活动边表中每项的xi值，并根据xi重新排序
	}
}
void InsertNetListToAet(vector<Edge_Study> edge,vector<Edge_Study>& AET)
{
	AET.insert(AET.begin(),edge[0]);

	for(int i=1;i<edge.size();i++)
	{
		int j=0;
		while(j<AET.size())
		{
			if(edge[i].x > AET[j].x)
			{
				j++;
			}
			else
			{
				break;
			}
		}
		AET.insert(AET.begin()+j,edge[i]);
	}
}

void FillAETScanLine(HDC hdc,vector<Edge_Study> AET, int y, int color)
{
	vector<Edge_Study>::iterator iaet;
	bool fill = false;
	for(iaet = AET.begin();iaet != AET.end()-1; iaet++)
	{
		fill = !fill;
		if(fill)
		{
			drawline_study(hdc,iaet->x,y ,(iaet+1)->x,y,color);
		}
	}
}

bool IsEdgeOutOfActive(Edge_Study e,int y)
{
	return (e.maxy == y);
}
void RemoveNonActiveEdgeFromAet(vector<Edge_Study> &AET,int y)
{
	vector<Edge_Study>::iterator iaet;
	for(iaet = AET.begin(); iaet != AET.end(); iaet++)
	{
		if(IsEdgeOutOfActive(*iaet,y))
		{
			AET.erase(iaet);
		}
	}
}

void UpdateAetEdgeInfo(Edge_Study& e)
{
	e.x += e.m;
}

void Sort(vector<Edge_Study>& AET)
{
	vector<Edge_Study> tmp;
	tmp.push_back(AET[0]);
	for(int i=1;i<AET.size();i++)
	{
		int j=0;
		while(j<tmp.size())
		{
			if(AET[i].x > tmp[j].x)
			{
				j++;
			}
			else
			{
				break;
			}
		}
		tmp.insert(tmp.begin() + j,AET[i]);
	}
	//AET.clear();
	AET = tmp;
}
void UpdateAndResortAet(vector<Edge_Study> &AET)
{
	//更新x
	vector<Edge_Study>::iterator iaet;
	for(iaet = AET.begin(); iaet != AET.end(); iaet++)
	{
		UpdateAetEdgeInfo(*iaet);
	}

}

void print(vector<Edge_Study> edge)
{
	vector<Edge_Study>::iterator iedge;
	Util::LOG("--------------------start--------------------------");
	for(iedge = edge.begin();iedge!=edge.end();iedge++)
	{
		Util::LOG("%d",iedge->x);
	}
	Util::LOG("---------------------end-------------------------");
}