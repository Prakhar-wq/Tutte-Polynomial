#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> pairs;
void tuttePolynomial(vector<vector<int>> &graph,vector<vector<int>> &ans,int bridge,int loop );
int iteration =0;
void dfsBridges(vector<vector<int>> &graph,int u,vector<bool> &vis,vector<int> &parent,vector<int> &disc,vector<int> &low,set<pairs> &bridges){

    static int time=0;
    vis[u]=true;

    disc[u]=low[u]=++time;

    for(int i=0;i<graph[u].size();i++){
        if(graph[u][i]==1){
            int v=i;
            if(!vis[v]){
            parent[v]=u;
            dfsBridges(graph,v,vis,parent,disc,low,bridges);

            low[u]=min(low[u],low[v]);

                if(low[v] > disc[u]){
                // cout<<u<<" "<<v<<endl;
                    bridges.insert({u,v});
                }
            }
            else if(v!=parent[u])
                low[u]=min(low[u],disc[v]);
        }

    }

}
set<pairs> findBridges(vector<vector<int>> &graph){

    int ver=graph.size();

    vector<int> parent(ver,-1),disc(ver,0),low(ver,0);
    vector<bool> vis(ver+1,false);

    set<pairs> ans;

    for(int i=0;i<ver;i++)
        if(!vis[i])
            dfsBridges(graph,i,vis,parent,disc,low,ans);

    // for(auto i : ans)
    //     cout<<i.first<<" "<<i.second<<endl;
    return ans;
}
void removeBridgeEdges(vector<vector<int>> &graph,set<pairs> &bridges){

    for(auto i:bridges){
        graph[i.first][i.second]=0;
        graph[i.second][i.first]=0;
    }

}
int detectAndRemoveLoops(vector<vector<int>> &graph){

    int loops=0;
    for(int i=0;i<graph.size();i++)
        if(graph[i][i]){
            loops++;
            graph[i][i]--;
            cout<<i<<endl;
        }
    return loops;
}
void deleteEdge(vector<vector<int>> graph,int i,int j,vector<vector<int>> &ans,int bridge,int loop ){
    graph[i][j]--;
    graph[j][i]--;
    tuttePolynomial(graph,ans,bridge,loop);
}
void contractEdge(vector<vector<int>> graph,int i,int j,vector<vector<int>> &ans,int bridge,int loop ){

    //We'll contract to smaller edge always
    graph[i][j]--;
    graph[j][i]--;
    for(int k=0;k<graph[j].size();k++){
        if(graph[j][k]&&k!=i){
            graph[j][k]--;
            graph[k][j]--;
            graph[i][k]++;
            graph[k][i]++;
        }
        else if(graph[j][k]&&k==i){
            graph[j][k]--;
            graph[k][j]--;
            graph[i][k]++;
        }
    }
    tuttePolynomial(graph,ans,bridge,loop);

}
void tuttePolynomial(vector<vector<int>> &graph,vector<vector<int>> &ans,int bridge,int loop ){
    cout<<iteration++<<endl;
    for(int i=0;i<graph.size();i++){
        for(int j=0;j<graph[i].size();j++){
            cout<<graph[i][j]<<" ";
        }
        cout<<endl;
    }

    
    //TuttePolynomial depends on certain factors:
    // If edge u & v is a bridge : T(G,e) = x*T(G/e)
    // If edge u & v is a loop   : T(G,e) = y*T(G\e)
    // Else T(G,e)=T(G/e)+T(G\e)
    pair<int,int> p;
    // int loop=0,bridge=0;
    //Find All the Bridges first.
    set<pairs> bridges;

    bridges=findBridges(graph); // O(V+E)
    bridge+=bridges.size();
    // cout<<"Haha"<<iteration++<<" "<<bridge<<" "<<loop<<endl;

    //Remove all the edges which are bridge edges
    removeBridgeEdges(graph,bridges);
    
    //Detect and Remove all the edges which are loops
    loop+=detectAndRemoveLoops(graph);
    // cout<<"loop"<<loop<<endl;
    // p={bridge,loop};//X^iy^j

    //If there are i bridges and j loops then its x^iy^j

    //Check whether the edges are bridge or loop
    int f=0;

    for(int i=0;i<graph.size();i++){
        for(int j=0;j<graph.size();j++){
            if(graph[i][j]>0){
                //Delete the edge
                // graph[i][j]--;
                // graph[j][i]--;
                deleteEdge(graph,i,j,ans,bridge,loop);
                // tuttePolynomial(graph,ans,bridge,loop);
                //Contract an edge
                contractEdge(graph,i,j,ans,bridge,loop);
                f=1;
                break;
            }    
        }
        if(f)
            break;        
    }
    if(bridge||loop)
        ans[bridge][loop]=1;

}
int main(){

    int vertex,edge;
    cin>>vertex;

    // vector<int> vertices(vertex);
    // vector<vector<int>> graph(vertex);
    vector<vector<int>> graph(vertex,vector<int>(vertex,0));
    // vector<pair<int,int>> edges;
    int u,v;
    set<pair<int,int>> s;
    // for(int i=0;i<edge;i++){
    //     cin>>u>>v;
    //     edges.push_back({u,v});
    //     graph[u].push_back(v);
    //     graph[v].push_back(u);
    //     g[u][v]=g[v][u]=1;
    //     // s.insert({u,v});
    //     // cout<<i<<endl;
    // }
    for(int i=0;i<vertex;i++)
        for(int j=0;j<vertex;j++)
            cin>>graph[i][j];
    
    vector<vector<int>> ans(vertex,vector<int>(vertex,0));

    // printSet(s);

    tuttePolynomial(graph,ans,0,0);
//    s= findBridges(graph);
//    removeBridgeEdges(graph,s);
    for(int i=0;i<vertex;i++){
        for(int j=0;j<vertex;j++){
            cout<<ans[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Completed"<<endl;
}