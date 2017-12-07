#include <iostream>
#include <fstream>
#include<string>
#define MAXN 10000
using namespace std;

int mk[MAXN];                          //mk[i]=0表示未访问过，为1表示访问过
int nx=MAXN, ny=MAXN;                  //X，Y集合中顶点的个数
int g[MAXN][MAXN];                     //邻接矩阵，g[i][j]为1，表示Xi,Yj有边相连
int cx[MAXN],cy[MAXN];                 //cx[i]最终求得的最大匹配中与Xi匹配的Y顶点，cy[i]同理
int pred[MAXN];                        //用来记录交错轨的，同时也用来记录Y集合中的顶点是否被遍历过
int queue[MAXN];                       //实现BFS搜索时，用到的队列（用数组模拟）

int read()                             //读取数据，数组存储
{
    ifstream cin("ER_data.txt",ios::in);
	//ifstream cin("test.txt",ios::in);
    for(int i=0;i<MAXN;i++)
        for(int j=0;j<MAXN;j++)
            g[i][j]=0;
    int x,y;
    while(cin>>x>>y)
        g[x][y]=1;
    cin.close();
    return 0;
}

int path (int u)
{
    for (int v=0; v <ny; v++ )   //考虑所有Yi顶点v
    {
        if (g[u][v]&&!mk[v])     //v与u邻接，且没有访问过
        {
            mk[v]=1;             //访问v
            //如果v没有匹配；或者v已经匹配了，但从cy[v]出发可以找到一条增广路
            //注意，前一个条件成立，则不会递归调用
            if (cy[v]==-1||path ( cy[v]) )
            {
                cx[u]=v;
                cy[v]=u;
                return 1;
            }
        }
    }
    return 0;                    //如果不存在从u出发的增广路
}

int DMaxMatch()                  //DFS算法
{
    int res=0;                   //所求得的最大匹配
    memset (cx,0xff,sizeof(cx)); //从0匹配开始增广，将cx和cy各元素初始化为-1
    memset (cy,0xff,sizeof(cy));
    for (int i=0;i<=nx;i++)
    {
        if (cx[i]==-1)            //从每个未盖点出发进行寻找增广路径
        {
            memset( mk,0,sizeof(mk));
            res+=path(i);         //每找到一条增广路，匹配数加1
        }
    }
    return res;
}

int BMaxMatch()                         //BFS算法
{
    int i,j,y;
    int cur,tail;                       //表示队列头和尾位置的下标
    int res = 0;		                //所求得的最大匹配
    memset (cx,0xff,sizeof(cx));        //初始化所有点为未匹配的状态
    memset (cy,0xff,sizeof(cy));
    for ( i=0;i< nx;i++ )
    {
        if ( cx[i]!= -1)                //对X集合中的每个未盖点i进行BFS找交错轨
            continue;
        for (j=0;j< ny;j++)
            pred[j]=-2;                 //-2表示初始值
        cur=tail=0;                     //初始化队列
        for ( j=0;j<ny;j++ )            //把i的邻接顶点都入队列
        {
            if ( g[i][j] )
            {
                pred [j]=-1; queue[tail++]=j;  //-1表示遍历到，是邻接顶点
            }
        }
        while ( cur <tail )              //BFS
        {
            y=queue[cur];
            if (cy[y]==-1) break;        //找到一个未匹配的顶点，则找到了一条增广轨
            cur++;                       //y已经被匹配给cy[y]了，从cy[y]出发，将它的邻接顶点入队列 
            for ( j = 0; j<ny; j++)
            {
                if (pred [j]==-2&&g[cy[y]][j])
                {
                    pred [j] =y;
                    queue [tail++]=j;
                    
                }
            }
        }
        if ( cur==tail ) continue;        //没有找到交错轨
        while (pred[y]>-1)                //更改交错轨上的匹配状态
        {
            cx[ cy[pred[y]]]=y;
            cy[y]=cy[pred[y]];
            y=pred[y];
        }
        cy[y]=i; cx[i]=y; 
        res++;                            //匹配数加1
    }
    return res;
}

int main(){
    read();                               //读数据
    int max1,max2; 
    max1 = DMaxMatch();                   //DFS找的最大匹配
    max2 = BMaxMatch();                   //BFS找的最大匹配
	
    cout<<max1<<endl<<max2<<endl;
	cin.get();
    return 0;
}