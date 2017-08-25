#include <bits/stdc++.h>

using namespace std;

struct node {
    int grid[3][3];
    int id, lev;
    node(){
        id = 0, lev = 0;
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                grid[i][j] = 0;
    }
    node(const node& x){
        id = x.id;
        lev = x.lev;
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                grid[i][j] = x.grid[i][j];
    }
    void print()
    {
        puts("");
        for(int i=0;i<3;i++){
            printf("%c|%c|%c\n",(grid[i][0]==0)?'-':(grid[i][0]==1)?'X':'O',
                                (grid[i][1]==0)?'-':(grid[i][1]==1)?'X':'O',
                                (grid[i][2]==0)?'-':(grid[i][2]==1)?'X':'O');
        }
        puts("");
    }
};
vector<vector<node> > graph;
map<int, int> result;
int MaxId = 0;

int finished(node u)
{
    for(int i=0;i<3;i++){
        if(u.grid[i][0] && u.grid[i][0] == u.grid[i][1] && u.grid[i][0] == u.grid[i][2])
            return 1 + (u.grid[i][0] == 1);
        if(u.grid[0][i] && u.grid[0][i] == u.grid[1][i] && u.grid[0][i] == u.grid[2][i])
            return 1 + (u.grid[0][i] == 1);
    }
    if(u.grid[0][0] && u.grid[0][0] == u.grid[1][1] && u.grid[0][0] == u.grid[2][2])
        return 1 + (u.grid[0][0] == 1);
    if(u.grid[0][2] && u.grid[0][2] == u.grid[1][1] && u.grid[0][2] == u.grid[2][0])
        return 1 + (u.grid[0][2] == 1);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(!u.grid[i][j]) return 0;
        }
    }
    return -1;
}

int fitness(node u)
{
    for(int i=0;i<3;i++){
        if(u.grid[i][0] == u.grid[i][1] && u.grid[i][0] == u.grid[i][2])
            return 1 - 2*(u.grid[i][0] == 1);
        if(u.grid[0][i] == u.grid[1][i] && u.grid[0][i] == u.grid[2][i])
            return 1 - 2*(u.grid[0][i] == 1);
    }
    if(u.grid[0][0] == u.grid[1][1] && u.grid[0][0] == u.grid[2][2])
        return 1 - 2*(u.grid[0][0] == 1);
    if(u.grid[0][2] == u.grid[1][1] && u.grid[0][2] == u.grid[2][0])
        return 1 - 2*(u.grid[0][2] == 1);
    return 0;
}

int searchId(node x, node cur = node())
{
//    if(cur.id == x.id) return cur.id;

    for(int i=0;i<(int)graph[cur.id].size();i++){
        node v = graph[cur.id][i];
        bool same = true;
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(v.grid[i][j] != x.grid[i][j]){
                    same = false;
                    break;
                }
            }
        }
        if(same) return v.id;
        int tmp = searchId(x, v);
        if(tmp != -1) return tmp;
    }
    return -1;
}

pair<int,int> findResult(node x, bool Max)
{
//    int id = searchId(x);
    int id = x.id;
    int ret = 1 - 2*(Max == 1);
    int idx = 0;
    if(!graph[id].size()){
        ret = result[id];
    }
    for(int i=0; i<(int)graph[id].size();i++){
        node v = graph[id][i];
        if(Max){
            int tmp = findResult(v,!Max).first;
            if(tmp > ret) ret = tmp, idx = i;
        } else {
            int tmp = findResult(v,!Max).first;
            if(tmp < ret) ret = tmp, idx = i;
        }
    }
    return {ret,idx};
}


int main()
{
    node root;
    root.id=0;
    root.lev=0;
    graph.resize(1);
    queue<node> Q;
    Q.push(root);
    bool turn = true;
    while(!Q.empty())
    {
        int sz = Q.size();
        while(sz--)
        {
            node u = Q.front();
            Q.pop();
            if(finished(u) == -1){
                int ret = fitness(u);
                result[u.id] = ret;
                continue;
            }
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(!u.grid[i][j]){
                        node tmp(u);
                        tmp.grid[i][j] = 1 + turn;
                        tmp.id = ++MaxId;
                        tmp.lev = u.lev+1;
                        graph.push_back(vector<node>());
                        graph[u.id].push_back(tmp);
                        Q.push(tmp);
                    }
                }
            }
        }
        turn = !turn;
    }
    cout << graph.size() << " " << graph[0].size() << " " << graph[1].size() << endl;
    cout << "Begin playing\n";
    node game;
    game.print();
    int winner = 0;
    while(!winner)
    {
        cout << "Enter x, y position\n";
        int x,y;
        bool valid = false;
        while(!valid){
            cin >> x >> y;
            if(x>=3 || x<0 || y>=3 || y<0 || game.grid[x][y] != 0){
                puts("Invalid position, try again");
            } else
                valid = true;
        }
//        node next = game;
        game.grid[x][y] = 2;
        game.id = searchId(game);
        assert(game.id >= 0);
        game.print();
        winner = finished(game);
        if(winner) break;
        auto bot = findResult(game, true);
        game = graph[game.id][bot.second];
        game.print();
        winner = finished(game);
    }
    cout << "Game finished\n";
    if(winner == 1) puts("You won");
    else if (winner == 2) puts("You lost");
    else puts("Draw");
    return 0;
}
