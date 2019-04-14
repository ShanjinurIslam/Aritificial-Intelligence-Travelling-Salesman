#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <fstream>
#include <time.h>
using namespace std ;

struct city{
    float x ;
    float y ;
    int id ;
};

struct savings{
    int i ;
    int j ;
    float val ;

    bool operator > (const savings& str) const
    {
        return (val > str.val);
    }
};

int bfs(vector<int> g[], int u,int n)
{
    int nodes = 0 ;
    queue<int> q;
    bool v[n];
    q.push(u);
    for(int i=0;i<n;i++){
        v[i] = false ;
    }

    v[u] = true ;

    while (!q.empty()) {

        int f = q.front();
        q.pop();
        nodes++ ;
        //cout << f << " ";
        for(int i=0;i<g[f].size();i++){
            if(!v[g[f][i]]){
                q.push(g[f][i]) ;
                v[g[f][i]] = true ;
            }
        }
    }

    return nodes;
}

class TSP{
private:
    vector<city> cities ;
    int id ;
public:
    TSP(){
        id = 0 ;
    }


    void addCity(int id,float x,float y){
        city c ;
        c.id = id-1 ;
        c.x = x ;
        c.y = y ;
        cities.push_back(c) ;
    }

    float calculateDistance(city c1,city c2){
        float x1 = c1.x ;
        float x2 = c2.x ;
        float y2 = c1.y ;
        float y1 = c2.y ;
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    vector<city> NearestNeighbour(int start_node){
        vector<city> path ;
        vector<city> unvisited = cities ;
        float min = 999999999  ;
        int next = -1 ;
        int index = -1 ;
        city current = cities[start_node] ;
        unvisited.erase(unvisited.begin()+start_node) ;
        path.push_back(current) ;
        while(!unvisited.empty()){
            for(int i=0;i<unvisited.size();i++){
                float distance = calculateDistance(unvisited[i],current) ;
                if(distance<min){
                    min = distance ;
                    next = unvisited[i].id ;
                    index = i ;
                }
            }
            city nbour = cities[next] ;
            unvisited.erase(unvisited.begin()+index) ;
            path.push_back(nbour) ;
            current = nbour ;
            min = 999999999  ;
        }
        path.push_back(path[0]) ;
        return path ;
    }

    void printGraph(vector<int> graph[],int n){
        int i = 0 ;
        for(int i=0;i<n;i++){
            cout<<"Node "<<i<<": " ;
            for(int j=0;j<graph[i].size();j++){
                cout<<graph[i][j]<<" " ;
            }
            if(i==n){
                break ;
            }
        }
    }

    int findElement(vector<int> graph,int item){
        for(int i=0;i<graph.size();i++){
            if(graph[i]==item){
                return i ;
            }
        }
        return -1 ;
    }

    vector<city> generatepath(vector<int> g[], int u,int n)
    {
        vector<city> path ;
        queue<int> q;
        bool v[n];
        q.push(u);
        for(int i=0;i<n;i++){
            v[i] = false ;
        }

        v[u] = true ;

        while (!q.empty()) {
            int f = q.front();
            q.pop();
            path.push_back(cities[f]) ;
            for(int i=0;i<g[f].size();i++){
                if(!v[g[f][i]]){
                    q.push(g[f][i]) ;
                    v[g[f][i]] = true ;
                }
            }
        }
        return path ;
    }

    int contains(vector<int> a,int x){
        for(int i=0;i<a.size();i++){
            if(a[i]==x){
                return 1 ;
            }
        }
        return 0 ;
    }

    int remove_item(vector<int> &items,int a){
        for(int i=0;i<items.size();i++){
            if(items[i]==a){
                items.erase(items.begin()+i) ;
                return 1 ;
            }
        }
        return 0 ;
    }

    int subtourCheck(vector<int> g[],int u,int sink,int center,int n){
        queue<int> q;
        bool v[n];
        q.push(u);
        for(int i=0;i<n;i++){
            v[i] = false ;
        }

        v[u] = true ;

        while (!q.empty()) {

            int f = q.front();
            q.pop();
            for(int i=0;i<g[f].size();i++){
                if(!v[g[f][i]] && g[f][i]!=center){
                    q.push(g[f][i]) ;
                    v[g[f][i]] = true ;
                    if(g[f][i]==sink){
                        return 1 ;
                    }
                }
            }
        }
        return 0 ;
    }

    vector<city> SavingsAlgorithm(int center,int source){
        vector<city> path ;
        int n = cities.size() ;
        vector<int> g[n] ;
        for(int i=0;i<cities.size();i++){
            if(i!=center){
                path.push_back(cities[center]) ;
                path.push_back(cities[i]) ;
                g[center].push_back(i) ;
                g[center].push_back(i) ;
                g[i].push_back(center) ;
                g[i].push_back(center) ;
            }
        }

        float costmatrix[n][n] ;

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                costmatrix[i][j] = 0;
            }
        }
        vector<savings> pairs ;
        for(int i=0;i<n;i++){
            for(int j=i+1;j<n;j++){
                if(i!=center && j!=center && i!=j){
                    costmatrix[i][j]  = calculateDistance(cities[center],cities[i]) + calculateDistance(cities[center],cities[j]) - calculateDistance(cities[i],cities[j]) ;
                    savings s ;
                    s.i = i ;
                    s.j = j ;
                    s.val = costmatrix[i][j] ;
                    pairs.push_back(s) ;
                }
            }
        }

        sort(pairs.begin(),pairs.end(),greater<savings>()) ;
        for(int i=0;i<pairs.size();i++){
            int a = pairs[i].i ;
            int b = pairs[i].j ;
            int flag1 =contains(g[a],center) ;
            int flag2 =contains(g[b],center) ;
            int flag3 =!subtourCheck(g,a,b,center,n);
            if(flag1&&flag2&&flag3){
                remove_item(g[center],a) ;
                remove_item(g[a],center) ;
                remove_item(g[b],center) ;
                remove_item(g[center],b) ;
                g[a].push_back(b) ;
                g[b].push_back(a) ;
            }
             ;

             ;
        }
        path = generatepath(g,source,n) ;
        return path ;
    }





    vector<city> TwoOpt(vector<city> path,int startNode){
        path.erase(path.begin()+(path.size()-1)) ;
        int size = (int) path.size() ;
        for(int i=0;i<size-2;i++){
            for(int j=i+2;j<size;j++){
                float i1 = calculateDistance(path[i],path[i+1]);
                float i2 = calculateDistance(path[j],path[j+1]);

                float d1 = calculateDistance(path[i],path[j]);
                float d2 = calculateDistance(path[i+1],path[j+1]);

                if((d1+d2)<(i1+i2)){
                    city tem = path[i+1] ;
                    path[i+1] = path[j] ;
                    path[j] = tem ;
                }
            }
        }

        path.push_back(path[0]) ;
        return path ;
    }

    float calculateCost(vector<city> path){
        float distance = 0 ;
        for(int i=0;i<path.size()-1;i++){
            distance += calculateDistance(path[i],path[i+1]) ;

        }
        return distance ;
    }

    int getCitySize(){
        return cities.size() ;
    }
};

class TSPRandomized{
private:
    vector<city> cities ;
    int id ;
public:
    TSPRandomized(){
        id = 0 ;
    }


    void addCity(int id,float x,float y){
        city c ;
        c.id = id-1 ;
        c.x = x ;
        c.y = y ;
        cities.push_back(c) ;
    }

    float calculateDistance(city c1,city c2){
        float x1 = c1.x ;
        float x2 = c2.x ;
        float y2 = c1.y ;
        float y1 = c2.y ;
        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    vector<city> NearestNeighbour(int start_node,int random_size){
        vector<city> path ;
        vector<city> unvisited = cities ;
        float min = 999999999  ;
        int next = -1 ;
        int index = -1 ;
        city current = cities[start_node] ;
        unvisited.erase(unvisited.begin()+start_node) ;
        path.push_back(current) ;
        while(!unvisited.empty()){
            vector< pair<int,float> > distances ;
            for(int i=0;i<unvisited.size();i++){
                float distance = calculateDistance(unvisited[i],current) ;
                distances.push_back(make_pair(unvisited[i].id,distance)) ;
            }
            sort(distances.begin(),distances.end(),[](auto &left,auto &right) {
                    return left.second < right.second ;
                 }) ;
            int nextloc = distances[(rand()%distances.size())%random_size].first ;
            city nbour = cities[nextloc] ;
            for(int i=0;i<unvisited.size();i++){
                if(unvisited[i].id==nbour.id){
                    index = i ;
                    break ;
                }
            }
            unvisited.erase(unvisited.begin()+index) ;
            path.push_back(nbour) ;
            current = nbour ;
        }
        path.push_back(path[0]) ;
        return path ;
    }

    void printGraph(vector<int> graph[],int n){
        int i = 0 ;
        for(int i=0;i<n;i++){
            cout<<"Node "<<i<<": " ;
            for(int j=0;j<graph[i].size();j++){
                cout<<graph[i][j]<<" " ;
            }
            if(i==n){
                break ;
            }
        }
    }

    int findElement(vector<int> graph,int item){
        for(int i=0;i<graph.size();i++){
            if(graph[i]==item){
                return i ;
            }
        }
        return -1 ;
    }

    vector<city> generatepath(vector<int> g[], int u,int n)
    {
        vector<city> path ;
        queue<int> q;
        bool v[n];
        q.push(u);
        for(int i=0;i<n;i++){
            v[i] = false ;
        }

        v[u] = true ;

        while (!q.empty()) {
            int f = q.front();
            q.pop();
            path.push_back(cities[f]) ;
            for(int i=0;i<g[f].size();i++){
                if(!v[g[f][i]]){
                    q.push(g[f][i]) ;
                    v[g[f][i]] = true ;
                }
            }
        }

        path.push_back(cities[u]) ;
        return path ;
    }

    int contains(vector<int> a,int x){
        for(int i=0;i<a.size();i++){
            if(a[i]==x){
                return 1 ;
            }
        }
        return 0 ;
    }

    int remove_item(vector<int> &items,int a){
        for(int i=0;i<items.size();i++){
            if(items[i]==a){
                items.erase(items.begin()+i) ;
                return 1 ;
            }
        }
        return 0 ;
    }

    int subtourCheck(vector<int> g[],int u,int sink,int center,int n){
        queue<int> q;
        bool v[n];
        q.push(u);
        for(int i=0;i<n;i++){
            v[i] = false ;
        }

        v[u] = true ;

        while (!q.empty()) {

            int f = q.front();
            q.pop();
            for(int i=0;i<g[f].size();i++){
                if(!v[g[f][i]] && g[f][i]!=center){
                    q.push(g[f][i]) ;
                    v[g[f][i]] = true ;
                    if(g[f][i]==sink){
                        return 1 ;
                    }
                }
            }
        }
        return 0 ;
    }

    vector<city> SavingsAlgorithm(int center,int source,int random_size){
        vector<city> path ;
        int n = cities.size() ;
        vector<int> g[n] ;
        for(int i=0;i<cities.size();i++){
            if(i!=center){
                path.push_back(cities[center]) ;
                path.push_back(cities[i]) ;
                g[center].push_back(i) ;
                g[center].push_back(i) ;
                g[i].push_back(center) ;
                g[i].push_back(center) ;
            }
        }

        float costmatrix[n][n] ;

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                costmatrix[i][j] = 0;
            }
        }
        vector<savings> pairs ;
        for(int i=0;i<n;i++){
            for(int j=i+1;j<n;j++){
                if(i!=center && j!=center && i!=j){
                    costmatrix[i][j]  = calculateDistance(cities[center],cities[i]) + calculateDistance(cities[center],cities[j]) - calculateDistance(cities[i],cities[j]) ;
                    savings s ;
                    s.i = i ;
                    s.j = j ;
                    s.val = costmatrix[i][j] ;
                    pairs.push_back(s) ;
                }
            }
        }
        sort(pairs.begin(),pairs.end(),greater<savings>()) ;
        while(pairs.size()>0){
            int index = (pairs.size()-1)%random_size ;
            int a = pairs[index].i ;
            int b = pairs[index].j ;
            int flag1 =contains(g[a],center) ;
            int flag2 =contains(g[b],center) ;
            int flag3 =!subtourCheck(g,a,b,center,n);
            if(flag1&&flag2&&flag3){
                remove_item(g[center],a) ;
                remove_item(g[a],center) ;
                remove_item(g[b],center) ;
                remove_item(g[center],b) ;
                g[a].push_back(b) ;
                g[b].push_back(a) ;
            }
            pairs.erase(pairs.begin()+index) ;
        }
        path = generatepath(g,source,n) ;
        return path ;
    }





    vector<city> TwoOptBest(vector<city> path){
        path.erase(path.begin()+(path.size()-1)) ;
        float cost = calculateCost(path) ;
        int i = 0 ;
        int size = (int) path.size() ;
        bool improved = true ;
        while(improved){
            improved = false ;
            for(int i=1;i<size-2;i++){
                for(int j=i+2;j<size;j++){
                    if(j-i==1) continue ;
                    float i1 = calculateDistance(path[i],path[i+1]);
                    float i2 = calculateDistance(path[j],path[j+1]);

                    float d1 = calculateDistance(path[i],path[j]);
                    float d2 = calculateDistance(path[i+1],path[j+1]);

                    if((d1+d2)<(i1+i2)){
                        city tem = path[i+1] ;
                        path[i+1] = path[j] ;
                        path[j] = tem ;
                    }
                }
            }
            int cost1 = calculateCost(path) ;
            if(cost1<cost){
                cost = cost1 ;
                improved = true ;
            }
        }
        path.push_back(path[0]) ;
        return path ;
    }

    vector<city> TwoOptFirst(vector<city> path){
        path.erase(path.begin()+(path.size()-1)) ;
        float cost = calculateCost(path) ;
        int i = 0 ;
        int size = (int) path.size() ;
        bool improved = true ;
        while(improved){
            improved = false ;
            for(int i=1;i<size-2;i++){
                for(int j=i+2;j<size;j++){
                    if(j-i==1) continue ;
                    float i1 = calculateDistance(path[i],path[i+1]);
                    float i2 = calculateDistance(path[j],path[j+1]);

                    float d1 = calculateDistance(path[i],path[j]);
                    float d2 = calculateDistance(path[i+1],path[j+1]);

                    if((d1+d2)<(i1+i2)){
                        city tem = path[i+1] ;
                        path[i+1] = path[j] ;
                        path[j] = tem ;
                        break ;
                    }
                }
            }
            int cost1 = calculateCost(path) ;
            if(cost1<cost){
                cost = cost1 ;
                improved = true ;
            }
        }
        path.push_back(path[0]) ;
        return path ;
    }

    float calculateCost(vector<city> path){
        float distance = 0 ;
        for(int i=0;i<path.size()-1;i++){
            distance += calculateDistance(path[i],path[i+1]) ;

        }
        return distance ;
    }

    int getCitySize(){
        return cities.size() ;
    }
};


int sh(TSP tsp,int cases,vector<city> &bestpath){
    srand(time(NULL)) ;
    vector<float> sh_results ;
    float avg = 0 ;
    float tem = 999999 ;
    int index = -1 ;
    vector<city> path ;
    for(int i=0;i<cases;i++){
        int source = rand()%tsp.getCitySize() ;
        path = tsp.SavingsAlgorithm(0,source) ;
        float val = tsp.calculateCost(path) ;
        if(val<tem){
            tem = val ;
            index = source ;
            bestpath = path ;
        }
        sh_results.push_back(val) ;
        avg += val ;
    }
    float sh_avg = (avg/cases) ;

    sort(sh_results.begin(),sh_results.end(),less<float>()) ;
    cout<<"Savings Results"<<endl ;
    cout<<"Number of cases : " ;
    cout<<cases<<endl;

    cout<<endl ;
    cout<<"Best Case: "<<sh_results[0]<<endl ;
    cout<<"Worst Case: "<<sh_results[cases-1]<<endl ;
    cout<<"Avrg Case: "<<sh_avg<<endl ;
    cout<<endl ;

    return index ;
}

vector<city> sh_random(TSPRandomized tsp,int cases,int source,int random_size){
    srand(time(NULL)) ;
    vector<float> sh_results ;
    float avg = 0 ;
    float tem = 999999 ;
    vector<city> path ;
    for(int i=0;i<cases;i++){
        vector<city> t =tsp.SavingsAlgorithm(0,source,rand()%random_size + 1) ;
        float val = tsp.calculateCost(t) ;
        if(val<tem){
            tem = val ;
            path = t ;
        }
        sh_results.push_back(val) ;
        avg += val ;
    }
    float sh_avg = (avg/cases) ;

    sort(sh_results.begin(),sh_results.end(),less<float>()) ;
    cout<<"Savings Results"<<endl ;
    cout<<"Number of cases : " ;
    cout<<cases<<endl;

    cout<<endl ;
    cout<<"Best Case: "<<sh_results[0]<<endl ;
    cout<<"Worst Case: "<<sh_results[cases-1]<<endl ;
    cout<<"Avrg Case: "<<sh_avg<<endl ;
    cout<<endl ;

    return path ;
}

int nnh(TSP tsp,int cases,vector<city> &bestpath){
    srand(time(NULL)) ;
    vector<float> nnh_results ;
    float tem = 999999 ;
    float avg = 0 ;
    int index = -1 ;
    for(int i=0;i<cases;i++){
        int source = rand()%tsp.getCitySize() ;
        vector<city> path = tsp.NearestNeighbour(source) ;
        float val = tsp.calculateCost(path) ;
        if(val<tem){
            tem = val ;
            bestpath = path ;
            index = source ;
        }
        nnh_results.push_back(val) ;
        avg += val ;
    }

    float nnh_avg = (avg/cases) ;

    sort(nnh_results.begin(),nnh_results.end(),less<float>()) ;
    cout<<"Nearest Neighbour Results"<<endl ;
    cout<<"Number of cases : " ;
    cout<<cases<<endl;

    cout<<endl ;
    cout<<"Best Case: "<<nnh_results[0]<<endl ;
    cout<<"Worst Case: "<<nnh_results[cases-1]<<endl ;
    cout<<"Avrg Case: "<<nnh_avg<<endl ;
    cout<<endl ;

    return index ;
}

vector<city> nnh_random(TSPRandomized tsp,int cases,int source,int random_size){

    vector<float> nnh_results ;
    float tem = 999999 ;
    float avg = 0 ;
    int index = -1 ;
    vector<city> path ;
    for(int i=0;i<cases;i++){
        vector<city> t = tsp.NearestNeighbour(source,random_size) ;
        float val = tsp.calculateCost(t) ;
        if(val<tem){
            tem = val ;
            path = t ;
        }
        nnh_results.push_back(val) ;
        avg += val ;
    }

    float nnh_avg = (avg/cases) ;

    sort(nnh_results.begin(),nnh_results.end(),less<float>()) ;
    cout<<"Nearest Neighbour Results"<<endl ;
    cout<<"Number of cases : " ;
    cout<<cases<<endl;

    cout<<endl ;
    cout<<"Best Case: "<<nnh_results[0]<<endl ;
    cout<<"Worst Case: "<<nnh_results[cases-1]<<endl ;
    cout<<"Avrg Case: "<<nnh_avg<<endl ;
    cout<<endl ;

    return path ;
}




int main() {
    TSP tsp1 ;
    TSPRandomized tspr1 ;
    ifstream infile1("pr76.tsp") ;
    cout<<"pr 76"<<endl ;
    float x,y ;
    int id ;
    while(infile1>>id>>x>>y){
        tsp1.addCity(id,x,y) ;
        tspr1.addCity(id,x,y) ;
    }

    int testcases = tsp1.getCitySize()/2 ;
    vector<city> nnh_greedy ;
    vector<city> sh_greedy ;
    int nnh_best = nnh(tsp1,testcases,nnh_greedy) ;
    int sh_best = sh(tsp1,testcases,sh_greedy);

    float avg_sh  = tspr1.calculateCost(tspr1.TwoOptBest(sh_greedy)) ;
    float avg_nnh = tspr1.calculateCost(tspr1.TwoOptBest(nnh_greedy)) ;
    float max1,max1_f ;
    float max2,max2_f ;
    float min1,min1_f ;
    float min2,min2_f ;

    max1 = avg_nnh ;
    max2 = avg_sh ;
    min1 = avg_nnh ;
    min2 = avg_sh ;

    for(int i=0;i<3;i++){
        vector<city> path1 = tspr1.TwoOptBest(nnh_random(tspr1,testcases,nnh_best,10)) ;
        float v1 = tspr1.calculateCost(path1) ;
        avg_nnh += v1 ;
        if(v1<min1){
            min1 = v1 ;
        }
        if(v1>max1){
            max1 = v1 ;
        }
        path1 =  tspr1.TwoOptBest(sh_random(tspr1,testcases,nnh_best,10));
        float v2 = tspr1.calculateCost(path1) ;
        avg_sh += tspr1.calculateCost(path1) ;
        if(v2<min2){
            min2 = v2 ;
        }
        if(v2>max2){
            max2 = v2 ;
        }
    }

    float avg_sh_f  = tspr1.calculateCost(tspr1.TwoOptFirst(sh_greedy)) ;
    float avg_nnh_f = tspr1.calculateCost(tspr1.TwoOptFirst(nnh_greedy)) ;

    max1_f = avg_nnh_f ;
    max2_f = avg_sh_f ;
    min1_f = avg_nnh_f ;
    min2_f = avg_sh_f ;

    for(int i=0;i<3;i++){
        vector<city> path1 = tspr1.TwoOptFirst(nnh_random(tspr1,testcases,nnh_best,10)) ;
        float v1 = tspr1.calculateCost(path1) ;
        avg_nnh_f += v1 ;
        if(v1<min1_f){
            min1_f = v1 ;
        }
        if(v1>max1_f){
            max1_f = v1 ;
        }
        path1 =  tspr1.TwoOptFirst(sh_random(tspr1,testcases,nnh_best,10));
        float v2 = tspr1.calculateCost(path1) ;
        avg_sh_f += tspr1.calculateCost(path1) ;
        if(v2<min2_f){
            min2_f = v2 ;
        }
        if(v2>max2_f){
            max2_f = v2 ;
        }
    }

    cout<<"2 Opt Best :: \n" ;
    cout<<"Nearest Neighbour Results"<<endl ;

    cout<<"Best Case: "<<min1<<endl ;
    cout<<"Worst Case: "<<max1<<endl ;
    cout<<"Avrg Case: "<<avg_nnh/4<<endl ;

    cout<<endl ;
    cout<<"Savings Results"<<endl ;

    cout<<endl ;
    cout<<"Best Case: "<<min2<<endl ;
    cout<<"Worst Case: "<<max2<<endl ;
    cout<<"Avrg Case: "<<avg_sh/4<<endl ;
    cout<<endl ;
    cout<<"2 Opt First :: \n" ;
    cout<<"Nearest Neighbour Results"<<endl ;

    cout<<"Best Case: "<<min1_f<<endl ;
    cout<<"Worst Case: "<<max1_f<<endl ;
    cout<<"Avrg Case: "<<avg_nnh_f/4<<endl ;

    cout<<endl ;
    cout<<"Savings Results"<<endl ;

    cout<<endl ;
    cout<<"Best Case: "<<min2_f<<endl ;
    cout<<"Worst Case: "<<max2_f<<endl ;
    cout<<"Avrg Case: "<<avg_sh_f/4<<endl ;

    return 0;
}
