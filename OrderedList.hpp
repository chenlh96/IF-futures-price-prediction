#include <random>
using namespace std;

template <typename T>
class OrderedList
{
  public:
    OrderedList();
    OrderedList(T small, T large, int maxl);
    OrderedList(OrderedList &&) = default;
    OrderedList(const OrderedList &) = default;
    OrderedList &operator=(OrderedList &&) = default;
    OrderedList &operator=(const OrderedList &) = default;
    ~OrderedList();

    bool Search(T x, int &index);
    int SearchLast(void);
    void Insert(T k);
    void Delete(T k);
    void Output();
    T GetElement(int index);

  private:
    int n;
    int max_length;
    T *val;
    int *link;
    T small;
    T tail_key;
    default_random_engine rnd;
};

template <typename T>
OrderedList<T>::OrderedList(T small, T large, int maxl)
{
    max_length = maxl;
    val = new T[max_length + 1];
    link = new int[max_length + 1];
    tail_key = large;
    n = 0;
    link[0] = 0;
    val[0] = 0;
    this->small = small;
}

template <typename T>
OrderedList<T>::~OrderedList()
{
    delete val;
    delete link;
}

template <typename T>
bool OrderedList<T>::Search(T x, int &index)
{
    index = 0;
    T max = small;
    int m = floor(sqrt(double(n)));
    uniform_int_distribution<int> dist(0, n - 1);
    for (int i = 1; i <= m; i++)
    {
        int j = dist(rnd) + 1;
        T y = val[j];
        if ((max < y) && (y < x))
        {
            max = y;
            index = j;
        }
    }
    while (val[link[index]] < x)
        index = link[index];

    return val[link[index]] == x;
}

template <typename T>
void OrderedList<T>::Insert(T k)
{
    if ((n == max_length) || (k >= tail_key))
        return;
    int index;
    if (!search(k, index))
    {
        val[++n] = k;
        link[n] = link[index];
        link[index] = n;
    }
}

template <typename T>
int OrderedList<T>::SearchLast()
{
    int index = 0;
    T x = val[n];
    T max = small;
    int m = floor(sqrt(double(n)));

    uniform_int_distribution<int> dist(0, n - 1);

    for (int i = 1; i <= m; i++)
    {
        int j = dist(rnd) + 1;
        T y = val[j];
        if ((max < y) && (y < x))
        {
            max = y;
            index = j;
        }
    }

    while (link[index] != n)
        index = link[index];

    return index;
}

template <typename T>
void OrderedList<T>::Delete(T k)
{
    if ((n == 0) || k > +tail_key)
        return;

    int index;

    if (Search(k, index))
    {
        int p = link[index];
        if (p == n)
            link[index] = link[p];
        else
        {
            if (link[p] != n)
            {
                int q = SearchLast();
                link[q] = p;
                link[index] = link[p];
            }
            val[p] = val[n];
            link[p] = link[n];
        }
    }
}

template<typename T>
T OrderedList<T>::GetElement(int idx)
{
    T elm;
    int i = 0;
    int index = link[0];
    int nxtIdx;
    while (i != idx + 1)
    {
        nxtIdx = index;
        elm = val[nxtIdx];
        index = link[nxtIdx];
        i++;
    }

    return elm;
}