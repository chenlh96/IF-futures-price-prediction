#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template <typename Tw, typename Tp>
class Knap
{
  public:
    Knap();

    friend Tp Knaspack(Tp *, Tw *, Tw, int);

  private:
    Tp Bound(int i);
    void BackTrack(int i);
    Tw c;
    int n;
    Tw *w;
    Tp *p;
    Tw cw;
    Tp cp;
    Tp bestp;
};

template <typename Tw, typename Tp>
void Knap<Tw, Tp>::BackTrack(int i)
{
    if (i > n)
    {
        bestp = cp;
        return;
    }
    if (cw + w[i] <= c)
    {
        cw += w[i];
        cp += p[i];
        BackTrack(i + 1);
        cw -= w[i];
        cp -= p[i];
    }
    if (Bound(i + 1) > bestp)
    {
        BackTrack(i + 1);
    }
}

template <typename Tw, typename Tp>
Tp Knap<Tw, Tp>::Bound(int i)
{
    Tw cleft = c - cw;
    Tp b = cp;

    while (i <= n && w[i] < cleft)
    {
        cleft -= w[i];
        p += p[i];
        i++;
    }

    if (i <= n)
        b += p[i] / w[i] * cleft;

    return b;
}

template <typename WeightT, typename PriceT>
class Item
{
  public:
    Item()
    {
        this->price = 0;
        this->ratio = 0;
        this->weight = 0;
    }
    Item(WeightT w, PriceT p) : weight{w}, price{p}
    {
        double r = 0;
        if (w != 0)
            r = 1.0 * p / w;
        this->ratio = r;
    }
    double GetRatio() { return ratio; }

    WeightT weight;
    PriceT price;

  private:
    double ratio;
};

template <typename WeightT, typename PriceT>
class RatioStandard
{
  public:
    bool operator()(Item<WeightT, PriceT> a, Item<WeightT, PriceT> b)
    {
        return a.GetRatio() > b.GetRatio();
    }
};

template <typename WeightT, typename PriceT, typename ContainerT = vector<Item<WeightT, PriceT>>>
class KNaspack
{
  public:
    KNaspack()
    {
    }

    KNaspack(WeightT weight[], PriceT price[], int length, double c)
    {
        for (int i = 0; i < length; i++)
        {
            this->Items.push_back(Item<WeightT, PriceT>(weight[i], price[i]));
        }
        this->MaxWeight = c;

        Init(length);
    }

    PriceT PriceUpperBound(int i);
    void BackTrack(int i);
    void GetSolution();
    void Init(int length);

    void ShowCurrentNode(int i);
    void ShowSortResult();
    PriceT GetBestPrice() { return BestPrice; }

  private:
    ContainerT Items;
    WeightT MaxWeight;
    WeightT CurrentWeight;
    PriceT CurrentPrice, BestPrice;
    vector<int> CurrentChoice, FinalChoice;

    int counter;

    RatioStandard<WeightT, PriceT> RatioStd;
};

template <typename WeightT, typename PriceT, typename ContainerT>
void KNaspack<WeightT, PriceT, ContainerT>::ShowCurrentNode(int depth)
{
    auto i = CurrentChoice.begin();
    cout << "Node Choice: ";
    while (i != CurrentChoice.begin() + depth)
    {
        cout << *i << " ";
        i++;
    }
    cout << "\n";
}

template <typename WeightT, typename PriceT, typename ContainerT>
void KNaspack<WeightT, PriceT, ContainerT>::ShowSortResult()
{
    cout << "Result: "
         << "\n";
    cout << "Weight  Price"
         << "\n";
    auto i = Items.begin();
    while (i != Items.end())
    {
        cout << i->weight << "  " << i->price << "\n";
        i++;
    }
    cout << "\n";
}

template <typename WeightT, typename PriceT, typename ContainerT>
void KNaspack<WeightT, PriceT, ContainerT>::Init(int length)
{
    this->counter = 0;
    this->CurrentChoice.resize(length);
    this->FinalChoice.resize(length);
    this->CurrentWeight = 0;
    this->CurrentPrice = 0;
    this->BestPrice = 0;

    for (int i = 0; i < length; i++)
    {
        CurrentChoice[i] = 0;
        FinalChoice[i] = 0;
    }
}

template <typename WeightT, typename PriceT, typename ContainerT>
PriceT KNaspack<WeightT, PriceT, ContainerT>::PriceUpperBound(int i)
{
    WeightT leftSpace = MaxWeight - CurrentWeight;
    PriceT bound = CurrentPrice;

    auto itemIter = Items.begin() + i;

    while (itemIter != Items.end() && itemIter->weight < leftSpace)
    {
        leftSpace -= itemIter->weight;
        bound += itemIter->price;
        itemIter++;
    }

    if (itemIter != Items.end())
        bound += itemIter->price / itemIter->weight * leftSpace;

    return bound;
}

template <typename WeightT, typename PriceT, typename ContainerT>
void KNaspack<WeightT, PriceT, ContainerT>::BackTrack(int i)
{
    ShowCurrentNode(i);
    counter++;
    auto itemIter = Items.begin() + i;
    if (itemIter == Items.end())
    {
        if (BestPrice < CurrentPrice)
        {
            BestPrice = CurrentPrice;
            copy(CurrentChoice.begin(), CurrentChoice.end(), FinalChoice.begin());
        }
        return;
    }
    if (CurrentWeight + itemIter->weight <= MaxWeight)
    {
        CurrentWeight += itemIter->weight;
        CurrentPrice += itemIter->price;
        CurrentChoice[i] = true;

        BackTrack(i + 1);

        CurrentWeight -= itemIter->weight;
        CurrentPrice -= itemIter->price;
        CurrentChoice[i] = -1;
    }
    if (PriceUpperBound(i + 1) > BestPrice)
    {
        CurrentChoice[i] = false;
        BackTrack(i + 1);
    }
}

template <typename WeightT, typename PriceT, typename ContainerT>
void KNaspack<WeightT, PriceT, ContainerT>::GetSolution()
{
    sort(Items.begin(), Items.end(), RatioStd);
    ShowSortResult();
    BackTrack(0);
}

using WeightT = int;
using PriceT = int;
int main()
{
    int length = 8;
    WeightT weightArr[8]{8, 7, 6, 2, 10, 11, 15, 12};
    PriceT priceArr[8]{10, 6, 8, 12, 5, 9, 20, 30};
    WeightT maxWeight = 40;

    KNaspack<WeightT, PriceT> knap(weightArr, priceArr, length, maxWeight);
    knap.GetSolution();
    cout << "Max Price: " << knap.GetBestPrice() << endl;

    system("pause");
}