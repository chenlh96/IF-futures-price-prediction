#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

// Birthdaty test
double BirthdayProb(int numOfPairs, int numOfPeople, int numOfDays, int numOfIteration)
{
    default_random_engine gen;
    gen.seed((int)chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, numOfDays - 1);

    int days[numOfDays];
    int success = 0;
    int rmNum = -1;
    int sumOfPairs = 0;

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
        }

        for (int j = 0; j < numOfDays; j++)
        {
            if (days[j] > 1)
            {
                sumOfPairs += (days[j] * (days[j] - 1) / 2);
                if (sumOfPairs >= numOfPairs)
                {
                    success++;
                    break;
                }
            }
        }

        sumOfPairs = 0;
    }

    return 1 - (double)success / numOfIteration;
}

template <typename ContainerT>
inline void CalStatistics(ContainerT data, int length)
{
    double *statistics = new double[3];

    for (int i = 0; i < length; i++)
        statistics[0] += data[i];

    statistics[0] /= length;

    for (int i = 0; i < length; i++)
        statistics[1] += (data[i] - statistics[0]) * (data[i] - statistics[0]);

    statistics[1] /= length;
    statistics[2] = sqrt(statistics[1]);

    cout << "mean: " << fixed << setprecision(8) << statistics[0] << endl;
    cout << "variance: " << fixed << setprecision(8) << statistics[1] << endl;
    cout << "Standrad Derivaiton: " << fixed << setprecision(8) << statistics[2] << endl;
}

int main()
{
    vector<double> prob1;
    vector<double> prob2;

    for (int i = 0; i < 300; i++)
    {
        prob1.push_back(BirthdayProb(1, 25, 365, 10000));
        prob2.push_back(BirthdayProb(25, 25, 365, 10000));
    }
    cout << "Result for number of pair 1: " << endl;
    CalStatistics<vector<double>>(prob1, 300);
    
    cout << "Result for number of pair 25: " << endl;
    CalStatistics<vector<double>>(prob2, 300);

    system("pause");
}