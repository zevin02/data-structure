#include "Graph2.hpp"
void test1()
{
    Matrix::Graph<char, int, INT32_MAX, true> g("0123", 4); // 有相图
    g.AddEdge('0', '1', 1);
    g.AddEdge('0', '3', 4);
    g.AddEdge('1', '3', 2);
    g.AddEdge('1', '2', 9);
    g.AddEdge('2', '3', 8);
    g.AddEdge('2', '1', 5);
    g.AddEdge('2', '0', 3);
    g.AddEdge('3', '2', 6);
    g.Print();
}
void test2()
{
    string a[] = {"张三", "李四", "王五", "赵六"};
    Table::Graph<string, int> g1(a, 4);
    g1.AddEdge("张三", "李四", 100);
    g1.AddEdge("张三", "王五", 200);
    g1.AddEdge("王五", "赵六", 30);
    g1.Print();
}
void testbfs()
{
    string a[] = {"张三", "李四", "王五", "赵六", "周七"};
    Matrix::Graph<string, int> g1(a, sizeof(a) / sizeof(string));
    g1.AddEdge("张三", "李四", 100);
    g1.AddEdge("张三", "王五", 200);
    g1.AddEdge("王五", "赵六", 30);
    g1.AddEdge("王五", "周七", 30);
    // g1.Print();

    g1.BFS("张三");

    g1.DFS("张三");
}
void TestGraphMinTree()
{
    const char *str = "abcdefghi";
    Matrix::Graph<char, int> g(str, strlen(str));
    g.AddEdge('a', 'b', 4);
    g.AddEdge('a', 'h', 8);
    // g.AddEdge('a', 'h', 9);
    g.AddEdge('b', 'c', 8);
    g.AddEdge('b', 'h', 11);
    g.AddEdge('c', 'i', 2);
    g.AddEdge('c', 'f', 4);
    g.AddEdge('c', 'd', 7);
    g.AddEdge('d', 'f', 14);
    g.AddEdge('d', 'e', 9);
    g.AddEdge('e', 'f', 10);
    g.AddEdge('f', 'g', 2);
    g.AddEdge('g', 'h', 1);
    g.AddEdge('g', 'i', 6);
    g.AddEdge('h', 'i', 7);

    // Matrix::Graph<char, int> kminTree;
    // cout << "Kruskal:" << g.Kruskal(kminTree) << endl;
    // kminTree.Print();
    cout << endl;
    Matrix::Graph<char, int> kminTree;
    cout << "Prim:" << g.Prim(kminTree, 'a') << endl;
    kminTree.Print();
}

void testDijkstra()
{
    const char *str = "sytx";
    Matrix::Graph<char, int, INT32_MAX, true> g(str, strlen(str));
    g.AddEdge('s', 't', 10);
    g.AddEdge('s', 'y', 5);
    g.AddEdge('t', 'y', -7);
    g.AddEdge('y', 'x', 3);
    vector<int> dist;
    vector<int> parentPath;
    g.Dijkstra('s', dist, parentPath);
    // g.PrintShortPath('s', dist, parentPath);
}

int main()
{
    // test1();
    // test2();
    // testbfs();
    // TestGraphMinTree();
    testDijkstra();
    return 0;
}