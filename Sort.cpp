#include <algorithm>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <random>
#include <type_traits>
using namespace std;

template <typename T>
int Partition_classic(T arr[], int begin, int end)
{
    int i = begin, j = end + 1;
    T pivot = arr[begin];

    while (true)
    {
        while (arr[++i] < pivot && i < end)
            ;
        while (arr[--j] > pivot)
            ;

        if (i >= j)
            break;

        swap(arr[i], arr[j]);
    }

    arr[begin] = arr[j];
    arr[j] = pivot;

    return j;
}

template <typename IterT>
class SherwoodMethod
{
  public:
    IterT operator()(IterT i, IterT j, int stepLength)
    {
        rnd.seed(3);
        int IterDistance = distance(i, j + 1);
        uniform_int_distribution<int> dist(0, IterDistance);
        int pivot = dist(rnd);
        while (pivot % stepLength != 0)
            pivot = dist(rnd);
        return i + pivot;
    }

    default_random_engine rnd;
};

template <typename IterT>
class ClassicMethod
{
  public:
    IterT operator()(IterT i, IterT j, int stepLength)
    {
        return i;
    }
};

template <typename IterT>
class LessIter
{
  public:
    LessIter()
    {
    }
    LessIter(int *count)
    {
        counter = count;
    }
    bool operator()(IterT i, IterT j)
    {
        (*counter)++;
        return *i < *j;
    }
    int GetCount() { return *counter; }
    int *counter;
};

template <typename IterT>
class GreaterIter
{
  public:
    GreaterIter()
    {
    }
    GreaterIter(int *count)
    {
        counter = count;
    }
    bool operator()(IterT i, IterT j)
    {
        (*counter)++;
        return *i > *j;
    }
    int GetCount() { return *counter; }
    int *counter;
};

template <typename IterT, typename ComparetMethod = LessIter<IterT>, typename DecidePivotMethod = ClassicMethod<IterT>>
IterT Partition(IterT begin, IterT end, int stepLength, int divide, DecidePivotMethod pick = DecidePivotMethod(), ComparetMethod compare = ComparetMethod{})
{
    auto i = begin, j = end + stepLength;

    auto pivot = pick(begin, end, stepLength);
    // auto pivotVal = *pivot;
    while (*pivot % divide != 0)
    {
        pivot = pick(begin, end, stepLength);
    }

    if (pivot != begin)
    {
        swap(*pivot, *begin);
        pivot = begin;
    }

    i += stepLength;
    j -= stepLength;
    while (true)
    {
        while (compare(i, pivot) && i < end && (*i) % divide == 0)
            // ;
            i += stepLength;
        while (compare(pivot, j) && (*j) % divide == 0)
            // ;
            j -= stepLength;

        if (i >= j)
            break;
        swap(*i, *j);
    }

    swap(*begin, *j);
    return j;
}

template <typename IterT, typename ComparetMethod = LessIter<IterT>, typename DecidePivotMethod = ClassicMethod<IterT>>
inline void QuickSort(IterT begin, IterT end, int stepLength = 1, int divide = 1, DecidePivotMethod method = DecidePivotMethod(), ComparetMethod compare = ComparetMethod{})
{
    if ((begin == end) || (end - stepLength) <= begin)
        return;

    while ((*begin) % divide != 0)
        begin += stepLength;
    while ((*end) % divide != 0)
        end -= stepLength;

    auto pivot = Partition(begin, end, stepLength, divide, method, compare);
    QuickSort(begin, pivot, stepLength, divide, method, compare);
    QuickSort(pivot + stepLength, end, stepLength, divide, method, compare);
}
int main()
{
    int counter = 0;
    int counter1 = 0;

    SherwoodMethod<vector<int>::iterator> sherwood;
    ClassicMethod<vector<int>::iterator> classicmd;
    LessIter<vector<int>::iterator> lessClassic(&counter1);
    LessIter<vector<int>::iterator> lessSherwood(&counter);
    vector<int> testset{8, 2, 6, 11, 3, 5, 7, 1, 9, 15};

    vector<int> testset1(testset.size());
    copy(testset.begin(), testset.end(), testset1.begin());
    vector<int> testset2(testset.size());
    copy(testset.begin(), testset.end(), testset2.begin());
    vector<int> testset3(testset.size());
    copy(testset.begin(), testset.end(), testset3.begin());

    // vector<int> testset1{8, 18, 2, 16, 6};

    // QuickSort(testset.begin(), testset.end() - 1, 2, sherwood);
    // QuickSort(testset.begin() + 1, testset.end() - 2, 2, sherwood, great);
    QuickSort(testset.begin(), testset.end() - 1, 1, 1, classicmd, lessClassic);
    cout << "Num of Comparison using the certain calssical quick sort method: " << lessClassic.GetCount() << "\n";
    cout << "Result of the certain calssical quick sort method"
         << "\n";
    for (int i = 0; i < testset.size(); i++)
    {
        cout << testset[i] << "  ";
    }

    cout << "\n";
    cout << "---------------------------------------------------------------------------";
    cout << "\n";
    cout << "---------------------------------------------------------------------------";
    cout << "\n";

    QuickSort(testset1.begin(), testset1.end() - 1, 1, 1, sherwood, lessSherwood);
    cout << "Num of Comparison using the sherwood quick sort method: " << lessSherwood.GetCount() << "\n";
    cout << "Result of the sherwood quick sort method"
         << "\n";
    for (int i = 0; i < testset1.size(); i++)
    {
        cout << testset1[i] << "  ";
    }
    cout << "\n";
    cout << "---------------------------------------------------------------------------";
    cout << "\n";
    cout << "---------------------------------------------------------------------------";
    cout << "\n";

    GreaterIter<vector<int>::iterator> great(&counter);

    QuickSort(testset2.begin(), testset2.end() - 2, 2, 1, classicmd, lessClassic);
    QuickSort(testset2.begin() + 1, testset2.end() - 1, 2, 1, classicmd, great);

    cout << "Result of the certain calssical quick sort method"
         << "\n";
    for (int i = 0; i < testset2.size(); i++)
    {
        cout << testset2[i] << "  ";
    }

    QuickSort(testset3.begin(), testset3.end() - 2, 2, 3, classicmd, great);
    cout << "Result of the certain calssical quick sort method and can be divided by 3: "
         << "\n";
    for (int i = 0; i < testset3.size(); i++)
    {
        cout << testset3[i] << "  ";
    }

    system("pause");
}