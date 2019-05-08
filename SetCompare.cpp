#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

template <typename elmT, typename ContainerT = vector<elmT>>
class SetCompareUtil
{
  public:
    SetCompareUtil(ContainerT &S, ContainerT &T)
    {
        int size = S.size();
        SetS.resize(size);
        SetT.resize(size);
        
        copy(S.begin(), S.end(), this->SetS.begin());
        copy(T.begin(), T.end(), this->SetT.begin());
        this->dist = InitRandomUtil();
    }

    uniform_int_distribution<int> InitRandomUtil();
    bool CompareMC();
    void AddDiffElm(elmT elmVal);
    int RandomCompare(int numOfExeperience, int comparisionPerExep);
    ContainerT GetEqualResult();

  private:
    ContainerT SetS;
    ContainerT SetT;

    default_random_engine rnd;
    uniform_int_distribution<int> dist;
    vector<elmT> DiffElm;
};

template <typename elmT, typename ContainerT>
inline uniform_int_distribution<int> SetCompareUtil<elmT, ContainerT>::InitRandomUtil()
{
    DiffElm.clear();
    auto iterDistance = distance(this->SetS.begin(), this->SetS.end());
    this->rnd.seed((int)chrono::system_clock::now().time_since_epoch().count());
    return uniform_int_distribution<int>(0, iterDistance - 1);
}

template <typename elmT, typename ContainerT>
inline void SetCompareUtil<elmT, ContainerT>::AddDiffElm(elmT elmVal)
{
    if (this->DiffElm.begin() != this->DiffElm.end())
    {
        auto tmp = find(this->DiffElm.begin(), this->DiffElm.end(), elmVal);
        if (tmp == this->DiffElm.end())
            this->DiffElm.push_back(elmVal);
    }
    else
        this->DiffElm.push_back(elmVal);
}

template <typename elmT, typename ContainerT>
ContainerT SetCompareUtil<elmT, ContainerT>::GetEqualResult()
{
    return DiffElm;
}

template <typename elmT, typename ContainerT>
bool SetCompareUtil<elmT, ContainerT>::CompareMC()
{
    int pick = this->dist(this->rnd);
    auto resultIter = find(SetT.begin(), SetT.end(), SetS[pick]);
    if (resultIter == SetT.end())
    {
        AddDiffElm(SetS[pick]);
        return false;
    }

    pick = this->dist(this->rnd);
    resultIter = find(SetS.begin(), SetS.end(), SetT[pick]);
    if (resultIter == SetS.end())
    {
        AddDiffElm(SetT[pick]);
        return false;
    }

    return true;
}

template <typename elmT, typename ContainerT>
int SetCompareUtil<elmT, ContainerT>::RandomCompare(int numOfExeperience, int comparisionPerExep)
{
    this->dist = InitRandomUtil();
    int success = 0;
    bool isSameIn1Exep = true;

    for (int i = 0; i < numOfExeperience; i++)
    {
        isSameIn1Exep = true;
        for (int j = 0; j < comparisionPerExep; j++)
        {
            isSameIn1Exep = CompareMC();
            if (!isSameIn1Exep)
                break;
        }
        if (isSameIn1Exep)
            success++;
    }

    return success;
}

template <typename elmT, typename ContainerT = vector<elmT>>
struct TestResult
{
    int numOfExeperience;
    int success;
    bool isSame;
    ContainerT DiffElm;
};

int main()
{
    vector<vector<char>> SetS{
        vector<char>{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'Z', 'X', 'Y'},
        vector<char>{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'Z', 'X', 'Y'},
        vector<char>{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'Z', 'X', 'Y'}};

    vector<vector<char>> SetT{
        vector<char>{'A', 'C', 'D', 'E', 'F', 'G', 'B', 'H', 'I', 'X', 'Y', 'Z'},
        vector<char>{'A', 'C', 'D', 'E', 'F', 'G', 'B', 'H', 'K', 'X', 'Y', 'Z'},
        vector<char>{'A', 'C', 'D', 'E', 'F', 'G', 'B', 'J', 'K', 'X', 'Y', 'Z'}};

    vector<TestResult<char>> results;
    for (int i = 0; i < 3; i++)
    {
        SetCompareUtil<char> setCompare(SetS[i], SetT[i]);

        TestResult<char> tmp;        
        tmp.success = setCompare.RandomCompare(100, 15);
        tmp.DiffElm = setCompare.GetEqualResult();
        tmp.numOfExeperience = 100;
        tmp.isSame = (100 == tmp.success);
        results.push_back(tmp);
    }

    system("pause");
}