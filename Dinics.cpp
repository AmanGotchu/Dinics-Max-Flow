#include <vector>
#include <iostream>
#include <queue>

#define int long long

using namespace std;

vector<int> nextNodeIndex;
vector<int> level;
const int MAXVAL = 1000000000;

struct Edge{
    int nextNode; // the next indexed node
    int reverseIndex; // Gives us the index of the next nodes adjacency list which points our current node
    int flow; // How much flow has been passed in so far
    int cap; // Total cap of the edge
};

void addEdge(int prevNode, int nextNode, int weight, vector< vector<Edge> > &graph){
    Edge forward = {nextNode, graph[nextNode].size(), 0, weight};
    Edge backward = {prevNode, graph[prevNode].size(), 0, weight};

    graph[prevNode].push_back(forward);
    graph[nextNode].push_back(backward);
}

bool bfs(int srcNode, int destNode, vector< vector<Edge> > &graph){
    fill(level.begin(), level.end(), -1);

    queue<int> q;
    q.push(srcNode);
    level[srcNode] = 0;

    while(q.size() > 0){
        int currNode = q.front(); q.pop();

        for(int i = 0; i<graph[currNode].size(); i++){
            Edge currEdge = graph[currNode][i];
            int nextNode = currEdge.nextNode;

            if(level[nextNode] == -1 && currEdge.flow < currEdge.cap){
                level[nextNode] = level[currNode] + 1;
                q.push(nextNode);
            }
        }
    }

    return level[destNode] != -1;
}

int dfs(int currNode, int destNode, int currMinFlow, vector< vector<Edge> > &graph){
    if(currNode == destNode){
        return currMinFlow;
    }

    for(nextNodeIndex[currNode]; nextNodeIndex[currNode]<graph[currNode].size(); nextNodeIndex[currNode]++){
        Edge &e = graph[currNode][nextNodeIndex[currNode]];

        if(level[e.nextNode] == level[currNode] + 1 && e.flow < e.cap){
            int nextFlow = min(currMinFlow, e.cap - e.flow);
            int dfsVal = dfs(e.nextNode, destNode, nextFlow, graph);

            if(dfsVal > 0){
                e.flow += dfsVal;
                graph[e.nextNode][e.reverseIndex].flow -= dfsVal;
                return dfsVal;
            }
        }
    }

    return 0;
}

int maxFlow(int src, int dest, vector< vector<Edge> > &graph){
    int totalFlow = 0;

    while(bfs(src, dest, graph)){
        fill(nextNodeIndex.begin(), nextNodeIndex.end(), 0);
        int flow = dfs(src, dest, MAXVAL, graph);
        while(flow){
            totalFlow+=flow;
            flow = dfs(src, dest, MAXVAL, graph);
        }
    }

    return totalFlow;
}

signed main() {
    int numNodes, numEdges; cin >> numNodes >> numEdges;

    vector< vector<Edge> > graph(numNodes);
    level.resize(numNodes, 0);
    nextNodeIndex.resize(numNodes, 0);

    for(int i = 0; i<numEdges; i++){
        int a, b, w; cin >> a >> b >> w;
        a--;
        b--;
        addEdge(a, b, w, graph);
    }

    cout << maxFlow(0, numNodes-1, graph) << endl;
}
