#include <iostream>
#include <fstream>
#include <set>
#include <queue>
extern "C"{
#include "math.h"
#include "stdlib.h"
#include "time.h"
#include "limits.h"
}


#define MAP_X 100
#define MAP_Y 100
#define MAX_DISTANCE 45
#define N 260
using namespace std;

struct Node;
class Point{
	public:
		Point();
		void Spotting(int v,int w,int i);
		void Linking(Point *v);
		bool Distance(int i,int j,Point V,int n);
		int Show_id(){
			return id;
		}
		void exam(){
			cout<<id<<":("<<x<<","<<y<<")\n";
		}
		
		int id;
		int x;
		int y;
		int top;
		Point *collection[10000]; //fulled??
		Node *row;
};
struct Node{
	int distance;
	int parent;
	Point *map;
	bool done;
};
bool Pass(Point *v,int n){
	set<int> s;
	s.insert(0);
	queue<Point *> next;
	for(int i=0;i<v->top;i++){
		next.push(v->collection[i]);
	}
	while(!next.empty()){
		if(s.find(next.front()->id)==s.end()){
			s.insert(next.front()->id);
			if(s.size()==n){
				return true;
			}
			for(int i=0;i<next.front()->top;i++){
				next.push(next.front()->collection[i]);
			}
		}
		next.pop();
	}
	return false;
}

int array_dijkstra(Point *spot,int n,int s,int t){
	int indext=0,ans=0;
	Node table[n];
	for(int i=0;i<spot[s].top;i++){
		table[indext].distance=sqrt((spot[s].x-spot[s].collection[i]->x)*(spot[s].x-spot[s].collection[i]->x)+(spot[s].y-spot[s].collection[i]->y)*(spot[s].y-spot[s].collection[i]->y));
		table[indext].parent=s;
		table[indext].map=spot[s].collection[i];
		table[indext].done=false;
		spot[s].collection[i]->row=&table[indext++];
	}
	for(int i=0;i<n;i++){
		if(i!=s&&spot[i].row==NULL){
			table[indext].distance=INT_MAX;
			table[indext].parent=-1;
			table[indext].map=&spot[i];
			table[indext].done=false;
			spot[i].row=&table[indext++];
		}
	}
	for(int i=0;i<indext;i++){
		int min=INT_MAX,min_indext=0;
		for(int j=0;j<indext;j++){
			if(table[j].done==false&&table[j].distance<min){
				min=table[j].distance;
				min_indext=j;
			}
		}
		int ID=table[min_indext].map->id;
		for(int j=0;j<spot[ID].top;j++){
			if(spot[ID].collection[j]!=&spot[ID]&&spot[ID].collection[j]->id!=s){
				int path=sqrt((spot[ID].x - spot[ID].collection[j]->x)*(spot[ID].x - spot[ID].collection[j]->x)+(spot[ID].y - spot[ID].collection[j]->y)*(spot[ID].y - spot[ID].collection[j]->y));
				if(min+path < spot[ID].collection[j]->row->distance){
					spot[ID].collection[j]->row->distance=min+path;
					spot[ID].collection[j]->row->parent=ID;
				}
			}
		}
		table[min_indext].done=true;
	}
	ans=spot[t].row->distance;
	for(int i=0;i<n;i++){
		spot[i].row=NULL;
	}
	return ans;
}
void swap(Node &a,Node &b){
	Node *temp2=a.map->row;
	a.map->row=b.map->row;
	b.map->row=temp2;
	Node temp=a;
	a=b;
	b=temp;
}
int heapify(Node *table,int i,int size){
	int change=0,ans=0;
	do{
		int temp=i;
		change=0;
		if(2*i+1<size&&table[2*i+1].distance<table[i].distance){
			temp=2*i+1;
		}
		if(2*i+2<size&&table[2*i+2].distance<table[temp].distance){
			temp=2*i+2;
		}
		if(temp!=i){
			swap(table[i],table[temp]);
			change=1;
			i=temp;
		}
		ans=i;
	}while(change==1);
	return ans;
}
void upward(Node *table,int i){
	int change=0;
	do{
		int temp=(i-1)/2;
		if(i<=0){
			break;
		}
		change=0;
		if(table[i].distance<table[(i-1)/2].distance){
			temp=(i-1)/2;
			swap(table[i],table[temp]);
			change=1;
			i=temp;
		}
	}while(change==1);
}
int heap_dijkstra(Point *spot,int n,int s,int t){
	int indext=0,ans=0;
	Node table[n];
	for(int i=0;i<spot[s].top;i++){
		table[indext].distance=sqrt((spot[s].x-spot[s].collection[i]->x)*(spot[s].x-spot[s].collection[i]->x)+(spot[s].y-spot[s].collection[i]->y)*(spot[s].y-spot[s].collection[i]->y));
		table[indext].parent=s;
		table[indext].map=spot[s].collection[i];
		table[indext].done=false;
		spot[s].collection[i]->row=&table[indext];
		upward(table,indext++);
	}
	for(int i=0;i<n;i++){
		if(i!=s&&spot[i].row==NULL){
			table[indext].distance=INT_MAX;
			table[indext].parent=-1;
			table[indext].map=&spot[i];
			table[indext].done=false;
			spot[i].row=&table[indext++];
		}
	}
	int size=indext;
	for(int i=0;i<indext;i++){
		int min=table[0].distance;
		swap(table[0],table[size-1]);
		heapify(table,0,size-1);
		int ID=table[--size].map->id;
		for(int j=0;j<spot[ID].top;j++){
			if(spot[ID].collection[j]!=&spot[ID]&&spot[ID].collection[j]->id!=s){
				int path=sqrt((spot[ID].x - spot[ID].collection[j]->x)*(spot[ID].x - spot[ID].collection[j]->x)+(spot[ID].y - spot[ID].collection[j]->y)*(spot[ID].y - spot[ID].collection[j]->y));
				if(min+path < spot[ID].collection[j]->row->distance){
					spot[ID].collection[j]->row->distance=min+path;
					spot[ID].collection[j]->row->parent=ID;
					upward(table,spot[ID].collection[j]->row-table);
				}
			}
		}
	}
	ans=spot[t].row->distance;
	for(int i=0;i<n;i++){
		spot[i].row=NULL;
	}
	return ans;

}
int main(){

	srand(time(NULL));
	for(int z=0;z<5;z++){
	for(int n=10;n<=N;n+=25){
		Point *spot=NULL;
		int m=0;
		//mapping
		do{
			int array_x[n]={0},array_y[n]={0};
			int indexx=0,indexy=0;
			int map[MAP_X][MAP_Y]={0};
			fstream file("Verticle.out",ios::out|ios::trunc);
			if(spot!=NULL){
				delete spot;
			}
			spot=new Point[n];
			for(int i=0;i<n;i++){
				int x=0,y=0;
				do{
					x=rand()%MAP_X;
					y=rand()%MAP_Y;
				}while(map[x][y]==1);
				spot[i].Spotting(x,y,i);
				map[x][y]=1;
				array_x[indexx++]=x;
				array_y[indexy++]=y;
				//		spot[i].exam();
			}
			file<<"x_array=["<<array_x[0];
			for(int i=1;i<indexx;i++){
				file<<","<<array_x[i];
			}
			file<<"]"<<endl;
			file<<"y_array=["<<array_y[0];
			for(int i=1;i<indexy;i++){
				file<<","<<array_y[i];
			}
			file<<"]"<<endl;
			file.close();
			file.open("Side.out",ios::out|ios::trunc);
			int first_point[n*n],second_point[n*n],indexf=0,indexs=0;
			m=0;
			for(int i=0;i<n;i++){
				for(int j=i+1;j<n;j++){
					if(spot[i].Distance(i,j,spot[j],n)){
						spot[i].Linking(&spot[j]);
						spot[j].Linking(&spot[i]);
						first_point[indexf++]=i;
						second_point[indexs++]=j;
						m++;
					}
				}
			}
			file<<"side_x=["<<first_point[0];
			for(int i=1;i<indexf;i++){
				file<<","<<first_point[i];
			}
			file<<"]"<<endl;
			file<<"side_y=["<<second_point[0];
			for(int i=1;i<indexs;i++){
				file<<","<<second_point[i];
			}
			file<<"]"<<endl;
			file.close();
		}while(!Pass(spot,n));

		//dijstra: time & average distance
		int a_length=0,h_length=0;

		clock_t before_a=clock();
		for(int i=0;i<n;i++){
			for(int j=i+1;j<n;j++){
				a_length+=array_dijkstra(spot,n,i,j);
			}
		}
		clock_t after_a=clock();
		cout<<"n="<<n<<",m="<<m<<endl;
		cout<<"average array length:"<<double(a_length/((n*(n-1))/2))<<"\tIt needs time:"<<double(after_a-before_a)/double(CLOCKS_PER_SEC)<<endl;

		clock_t before_h=clock();
		for(int i=0;i<n;i++){
			for(int j=i+1;j<n;j++){
				h_length+=heap_dijkstra(spot,n,i,j);
			}
		}
		clock_t after_h=clock();
		cout<<"average heap length:"<<double(h_length/((n*(n-1))/2))<<"\tIt needs time:"<<double(after_h-before_h)/double(CLOCKS_PER_SEC)<<endl<<endl;
		delete spot;
	}
	}
}


Point::Point(){
	x=0;
	y=0;
	top=0;
	row=NULL;
	for(int i=0;i<10000;i++){
		collection[i]=NULL;
	}
}
void Point::Spotting(int v,int w,int i){
	x=v;
	y=w;	
	id=i;
	row=NULL;
}
void Point::Linking(Point *v){
	if(top<10000){
		collection[top]=v;
		top++;
	}else cout<<"Fulled"<<endl;
}
bool Point::Distance(int i,int j,Point V,int n){
	int distance=sqrt((V.x-x)*(V.x-x)+(V.y-y)*(V.y-y));
	int Capital=0,Municipality=n*0.15,County=n*0.5;

	if(i==Capital&&j>0&&j<Municipality&&distance<MAX_DISTANCE){
		return true;
	}
	else if(i==Capital&&j>=Municipality&&j<County&&distance<MAX_DISTANCE*0.9){
		return true;	
	}
	else if(i==Capital&&j>=County&&distance<MAX_DISTANCE*0.8){
		return true;
	}
	else if(i>Capital&&i<Municipality&&j>Capital&&j<Municipality&&distance<MAX_DISTANCE*0.7){
		return true;
	}
	else if(i>Capital&&i<Municipality&&j>=Municipality&&j<County&&distance<MAX_DISTANCE*0.6){
		return true;
	}
	else if(i>Capital&&i<Municipality&&j>County&&distance<MAX_DISTANCE*0.5){
		return true;
	}
	else if(i>=Municipality&&i<County&&j>=Municipality&&j<County&&distance<MAX_DISTANCE*0.45){
		return true;
	}
	else if(i>=Municipality&&i<County&&j>County&&distance<MAX_DISTANCE*0.4){
		return true;
	}
	else if(i>County&&j>County&&distance<MAX_DISTANCE*0.35){
		return true;
	}else return false;

}
