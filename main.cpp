#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<vector>
#include<time.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstdlib>
#include<ctime>

using namespace std;
#define maxv 2000

int vertexnum = 0, edgenum = 0 ;

struct Vertex {
    
    vector<int> neighbor;   //�ڵ㼯
    int degree =0;          //����
    bool deleted = false;
    int coreness = 0;
    int peakness = 0;
};

struct Graph {
    int n = 0;
    int m = 0;
    Vertex node[maxv];
};


void get_peak( Graph &g )
{
    /* Batagelj and Zaversnik Algorithm */
    int n = g.n;
    vector<int> vertex(n, 0);  // �ź����Ķ���
    vector<int> deg(n, 0);     // ��ʼ�涥���degree�����涥���coreness
    vector<int> bin(n, 0);     // bin sort
    vector<int> pos(n, 0);     // ����bin���㶥�������vertex�����λ�� 
    vector<int> renum(n, 0);
    vector<int> val(vertexnum + 1, 0);    // �ҵ�renum[i]==valʱ��i����Ϊ���Ǽ���ʹ����ԭͼg��neighbor��
                                          // ����ʱ�����ҳ���neighbor��id����ԭid��������Ҫ��val����i��

    
    int j = 0;
    for (int i = 1; i <= vertexnum; i++)  //Ϊɾ����ʣ�µĶ������±��(0��n)
    {
        if (g.node[i].deleted == true) continue;
        renum[j++] = i;
        val[i] = j - 1;
    }

    /* ���Ե�
    for (int i = 0; i < n; i++)
    {
        cout << i << ' ' << renum[i] << endl;
    }
    */

    /* Initialization */
    for (int i = 0; i < n; ++i) 
    {
        deg[i] = g.node[renum[i]].degree;
        bin[deg[i]]++;
    }
    int start = 0;
    for (int i = 0; i < n; ++i) 
    {
        int num = bin[i];
        bin[i] = start;
        start += num;
    }
    // Bin sort vertices by their degree
    for (int i = 0; i < n; ++i) 
    {
        pos[i] = bin[deg[i]];
        vertex[pos[i]] = i;
        bin[deg[i]]++;
    }
    // Recover bin[]
    for (int i = n - 1; i >= 1; --i) bin[i] = bin[i - 1];
    bin[0] = 0;

    /* Main Algorithm */
    for (int i = 0; i < n; ++i) 
    {
        int v = vertex[i];
        for (auto u : g.node[renum[v]].neighbor) 
        {
            u = val[u];             
            if (deg[u] > deg[v]) 
            {
                int du = deg[u], pu = pos[u];
                int pw = bin[du];
                int w = vertex[pw];
                if (u != w) {
                    pos[u] = pw;
                    pos[w] = pu;
                    vertex[pu] = w;
                    vertex[pw] = u;
                }
                bin[du]++;
                deg[u]--;
            }
        }
    }
    
    /*
    for (int i = 0; i < n; ++i)
    {
        g.node[i].coreness = deg[i];
    }
    */

    /* �ҵ���ǰcoreness���Ķ��㼯������cn��ֵ��pn��ɾ����Щ���� */
    int maxcore = deg[vertex[n - 1]];
    int maxi = n - 1;
    for (int i = maxi; i >= bin[maxcore]; i--)
    {
        int v = vertex[i];  //vΪÿ����Ҫɾ���Ķ���
        g.node[renum[v]].peakness = deg[v];
        g.node[renum[v]].deleted = true;
        for (auto u : g.node[renum[v]].neighbor)
        {
            if (g.node[u].deleted == true) continue;
            g.node[u].degree--;
            int j = 0;
            for (; j < g.node[u].neighbor.size(); j++)
            {
                if (g.node[u].neighbor[j] == renum[v]) break;
            }
            g.node[u].neighbor[j] = g.node[u].neighbor.back();
            g.node[u].neighbor.pop_back();
        }
        g.n--;
         cout << renum[v] << ' ' << g.node[renum[v]].peakness << '\n' << endl;
    }
}




int main(int argc, char* argv[])
{
    Graph g;

    
    /* Open the input file */
    ifstream inputFile("datasets.txt");
    if (!inputFile) {
        cerr << "Input file could not be opened!" << endl;
        exit(EXIT_FAILURE);
    }

    
    inputFile >> vertexnum >> edgenum;
    g.n = vertexnum;
    g.m = edgenum;

    for (int i = 1; i <= edgenum; i++)  //ע�����ݼ���1��ʼ���
    {
        int u, v;
        inputFile >> u >> v;

        g.node[u].neighbor.push_back(v);
        g.node[v].neighbor.push_back(u);
        g.node[u].degree++;
        g.node[v].degree++;

    }

    inputFile.close();


    /* k-peak decomposition */
    while (g.n > 0)
    {
        get_peak(g);
    }
        

    /* Output the Result */
    ofstream outputFile("result.txt");
    if (!outputFile) {
        cerr << "Output file could not be opened!" << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i <= vertexnum; i++) {
        outputFile << i << ' ' << g.node[i].peakness << '\n';
    }
    outputFile.close();

    return 0;
}