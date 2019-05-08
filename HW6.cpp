#include "HW.h"
#include <iostream>
#include <queue>
using namespace std;

// MaxLoading
template <class T>
void AddLiveNode(priority_queue<HeapNode<T>> &H, bbnode<T> *E, T wt, bool ch, int lev)
{
    bbnode<T> *b = new bbnode<T>;
    bool parent_path[lev] = E->LChild;
    parent_path[lev - 1] = ch;
    b->LChild = parent_path;
    HeapNode<T> N;
    N.uweight = wt;
    N.level = lev,
    N.ptr = b;
    H.push(N);
}

template <class T>
T MaxLoading(T w[], T c, int n, int bestx[])
{
    priority_queue<HeapNode<T>> H(1000);

    T r = new T[n + 1];
    r[n] = 0;
    for (int j = n - 1; j > 0; j++)
        r[j] = r[j + 1] + w[j + 1];

    int i = 1;
    bbnode<T> *E = 0;
    T Ew = 0;

    while (i != n + 1)
    {
        if (Ew + w[i] < c)
        {
            AddLiveNode(H, E, Ew + w[i] + r[i], true, i + 1);
        }
        AddLiveNode(H, E, Ew + r[i], false, i + 1);

        HeapNode<T> N = H.top();
        H.pop();
        i = N.level;
        E = N.ptr;
        Ew = N.uweight - r[i - 1];
    }

    return Ew;
}

// Knaspack
template <class Tw, class Tp>
bool operator<(HeapNode4nap<Tw, Tp> a, HeapNode4nap<Tw, Tp> b)
{
    return a.uprofit > b.uprofit;
}

template <class Tw, class Tp>
Tp Knap<Tw, Tp>::Bound(int i)
{
    Tw cleft = c - cw;
    Tp b = cp;
    while (i <= n && w[i] <= left)
    {
        cleft -= w[i];
        b += p[i];
        i++;
    }
    if (i <= n)
        b += p[i] / w[i] * cleft;
    return b;
}

template <class Tw, class Tp>
void Knap<Tw, Tp>::AddLiveNode(Tp up, Tp cp, Tw cw, bool ch, int lev, int *x)
{
    HeapNode4nap<Tw, Tp> N;
    N.x = new int[n + 1];
    for (int j = 0; j <= n; j++)
        N.x[j] = x[j];
    N.x[lev - 1] = ch;
    N.uprofit = up;
    N.profit = cp;
    N.weight = cw;
    N.level = lev;
    H->push(N);
}

template <class Tw, class Tp>
Tp Knap<Tw, Tp>::MsxKnaspack()
{
    H = new priority_queue<HeapNode4nap<Tw, Tp>>(1000);

    Tp bestp = 0;
    bestx = new int[n + 1];
    for (int k = 0; k <= n; k++)
        bestx[k] = 0;
    int i = 1;
    Tp up = Bound(1);
    while (i != n + 1)
    {
        Tw wt = cw + w[i];
        if (wt < c)
        {
            if (cp + p[i] > bestp)
                bestp = cp + p[i];
            AddLiveNode(up, cp + p[i], cw + w[i], true, i + 1, bestx);
            up = Bound(i + 1);
        }
        if (up >= bestp)
            AddLiveNode(up, cp, cw, false, i + 1, bestx);

        HeapNode4nap<Tw, Tp> N = H->top();
        H->pop();
        cw = N.weight;
        cp = N.profit;
        up = N.uprofit;
        i = N.level;
    }
}

template <class Tw, class Tp>
Tp Knaspack(Tp p[], Tw w[], Tw c, int n, int bestx[])
{
    Tw W = 0;
    Tp P = 0;

    NaspackObj *Q = new NaspackObj[n];
    for (int i = 0; i < n; i++)
    {
        Q[i - 1].ID = i;
        Q[i - 1].d = 1.0 * p[i] / w[i];
        P += p[i];
        W += w[i];
    }

    if (W <= c)
        return P;
    else
    {
        // Sort(Q, n);
    }

    Knap<Tw, Tp> K;
    K.p = new Tp[n + 1];
    K.w = new Tw[n + 1];
    for (int i = 1; i <= n; i++)
    {
        K.p[i] = p[Q[i - 1].ID];
        K.w[i] = w[Q[i - 1].ID];
    }
    K.cp = 0;
    K.cw = 0;
    K.c = c;
    K.n = n;

    Tp bestp = K.MsxKnaspack();

    return bestp;
}

bool Place(int ci, int cj, int *x)
{
    for (int i = 0; i < ci; i++)
    {
        if (cj == x[i] || abs(ci - i) == abs(cj - x[i]))
            return false;
    }
    return true;
}

int CalOccupy(int n, int i, int *x)
{
    int sum = 0;
    if (i + 1 != n)
    {
        for (int l = 0; l < n; l++)
        {
            if (!Place(i + 1, l, x))
                sum++;
        }
        return sum;
    }
    else
    {
        return 0;
    }
}

void NQueen(int n)
{
    priority_queue<QueenNode> MaxHeap;
    int occupy = 0;
    int locate[n];
    for (int i = 0; i < n; i++)
        locate[i] = -(128);

    qnode *p = 0;
    int i = 0;
    while (i != n)
    {
        for (int j = 0; j < n; j++)
        {
            if (Place(i, j, locate))
            {
                locate[i] = j;
                occupy = CalOccupy(n, i, locate);

                qnode *n = new qnode;
                n->col = j;
                n->parent = p;
                QueenNode current;
                current.ocupy = occupy;
                // current.x = copy;
                current.row = i + 1;
                current.ptr = n;
                MaxHeap.push(current);
            }
        }

        QueenNode piror = MaxHeap.top();
        MaxHeap.pop();
        occupy = piror.ocupy;
        i = piror.row;
        p = piror.ptr;
        for (int j = i; j > 0; j--)
        {
            locate[j - 1] = p->col;
            p = p->parent;
        }
        p = piror.ptr;

        if (MaxHeap.empty())
        {
            if (i < n)
                cout << "no solution" << endl;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j != locate[i])
                cout << "* ";
            else
                cout << locate[i] << " ";
        }
        cout << endl;
    }
}

int main()
{
    NQueen(10);
    getchar();
}