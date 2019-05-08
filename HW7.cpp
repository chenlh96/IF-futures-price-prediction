#include "OrderedList.hpp"
#include "iostream"
#include "math.h"
#include <iomanip>
#include <random>
using namespace std;

// 7-3, 7-5, 7-6, 7-8, 7-17, 7-19

void GetRandomNum(int m, int n)
{
    if (m > n)
        return;
    default_random_engine gen;
    uniform_int_distribution<int> dist(0, n);
    int num[n];
    for (int i = 0; i < n; i++)
    {
        num[i] = i;
    }
    int ran = -1;
    int tmp;
    for (int i = 0; i < m; i++)
    {
        ran = dist(gen);
        tmp = num[i];
        num[i] = num[ran];
        num[ran] = tmp;
        uniform_int_distribution<int> dist(i + 1, n);
        cout << num[i] << " ";
    }
    cout << endl;
}

// Birthdaty test
double BirthdayProb(int numOfPeople, int numOfDays, int numOfIteration)
{
    default_random_engine gen;
    uniform_int_distribution<int> dist(0, numOfDays - 1);

    int days[numOfDays];
    for (int j = 0; j < numOfDays; j++)
        days[j] = 0;

    int success = 0;
    int rmNum = -1;
    for (int i = 0; i < numOfIteration; i++)
    {
        for (int j = 0; j < numOfDays; j++)
        {
            days[j] = 0;
        }

        for (int j = 0; j < numOfPeople; j++)
        {
            rmNum = dist(gen);
            days[rmNum]++;
            if (days[rmNum] > 1)
            {
                success++;
                break;
            }
        }
    }

    return 1 - (double)success / numOfIteration;
}

template <typename T>
T *OrderedListSort(T arr[], int n)
{
    T min = arr[0];
    T max = arr[0];
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < min)
            min = arr[i];
        if (arr[i] > max)
            max = arr[i];
    }

    OrderedList<T> SortUtil(min, max, n);
    for (int i = 0; i < n; i++)
    {
        SortUtil.Insert(arr[i]);
    }
    for (int i = 0; i < n; i++)
    {
        arr[i] = SortUtil.GetElement(i);
    }

    return arr;
}

template <typename T>
bool MC(T x);
template <typename T>
void Judge(T x);

template <typename T>
void LV(T x) {
    bool t = MC(x);
    while (!Judge(x))  t = MC(x);
}

template <typename T>
bool MC(T x) {
    LV(x);
    if (Judge(x)) return false;
    else return true;
}

int main()
{
    cout << fixed << setprecision(6) << BirthdayProb(25, 365, 100000) << endl;
    system("pause");
}
