#include <iostream>
#include <queue>
using namespace std;

template <class T>
class bbnode;

template <class T>
class HeapNode
{
public:
  HeapNode();
  HeapNode(HeapNode &&) = default;
  HeapNode(const HeapNode &) = default;
  HeapNode &operator=(HeapNode &&) = default;
  HeapNode &operator=(const HeapNode &) = default;
  ~HeapNode();

  friend void AddLiveNode(priority_queue<HeapNode<T>> &, bbnode<T> *, T, bool, int);
  friend void MaxLoading(T *, T, int, int *);

  friend bool operator<(HeapNode a, HeapNode b) { return a.uweight > b.uweight; }

private:
  bbnode<T> *ptr;
  T uweight;
  int level;
};
template <class T>
void AddLiveNode(priority_queue<HeapNode<T>> &, bbnode<T> *, T, bool, int);
template <class T>
void MaxLoading(T *, T, int, int *);

template <class T>
class bbnode
{
public:
  bbnode();
  bbnode(bbnode &&) = default;
  bbnode(const bbnode &) = default;
  bbnode &operator=(bbnode &&) = default;
  bbnode &operator=(const bbnode &) = default;
  ~bbnode();

  friend void AddLiveNode(priority_queue<HeapNode<int>> &, bbnode *, T, bool, int);
  friend void MaxLoading(T *, T, int, int *);

private:
  bool *LChild;
};

template <class T>
void AddLiveNode(priority_queue<HeapNode<int>> &, bbnode<T> *, T, bool, int);
template <class T>
void MaxLoading(T *, T, int, int *);

class NaspackObj
{
public:
  friend int Knaspack(int *, int *, int, int, int *);
  friend bool operator<(NaspackObj a, NaspackObj b) { return a.d > b.d; }

private:
  int ID;
  float d;
};

int Knaspack(int *, int *, int, int, int *);

template <class Tw, class Tp>
class Knap;

// class bbnode4nap
// {
//   friend Knap<int, int>;
//   friend int Knaspack(int *, int *, int, int, int *);

// private:
//   bbnode4nap *parent;
//   bool Lchild;
// };

template <class Tw, class Tp>
class HeapNode4nap
{
  friend Knap<Tw, Tp>;
  friend bool operator<(HeapNode4nap a, HeapNode4nap b);

private:
  Tp uprofit, profit;
  Tw weight;
  int level;
  int *x;
};

template <class Tw, class Tp>
class Knap
{
  friend Tp Knaspack(Tp *, Tw *, Tw, int, int *);

public:
  Tp MsxKnaspack();
  Tp Bound(int i);
  void AddLiveNode(Tp up, Tp cp, Tw cw, bool ch, int lev, int *x);

private:
  priority_queue<HeapNode4nap<Tp, Tw>> *H;
  Tw c;
  int n;
  Tw *w;
  Tp *p;
  Tw cw;
  Tp cp;
  int *bestx;
};

class qnode
{
public:
  int col;
  qnode *parent;
};

class QueenNode
{
  friend bool operator<(QueenNode a, QueenNode b)
  {
    if (a.row == b.row)
      return a.ocupy < b.ocupy;
    else
      return a.row > b.row;
  }

public:
  int ocupy;
  int row;
  qnode *ptr;
};

template <class T>
class Edge
{
public:
  friend bool operator<(Edge e1, Edge e2)
  {
    return e1.weight > e2.weight;
  }
  // Properties
  int u, v; // Nodes with this edge;
  T weight; // Weight of the edge

  Edge(int, int, T);
};

template <class T>
class Graph
{
public:
  Graph(int v, int e);

  void MinMAxWST();
  void AddEdge(int i, int j, T w);

private:
  int v, e;
  T **weight;
  int *belong;
  vector<Edge<T>> eVec;
  priority_queue<Edge<T>> eq;

  int FindIndex();
  void SortEdge();
  void UnionNode(int u, int v);
};

class RomeoAndJuliet
{
  int m, n;
  int **Maze;
  pair<int, int> Romeo, Juliet;
  int current_turn;
  int best_turn;
  int prior_direction;
  int current_direction;
  pair<int, int> *corrdinate;

public:
  RomeoAndJuliet(int m, int n, int **Maze, pair<int, int> startp, pair<int, int> endp);
  void NextStep(int i, int j);
  bool IsCoverAllRoom();
};

class Loading
{
public:
  void BackTrack(int i);
  float GetMax();
  Loading(int n, float *w, float c, float cw = 0, float best = 0);

private:
  int n;
  float *w, c, cw, bestw;
};