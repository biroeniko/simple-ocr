/* MIT License

Copyright (c) 2018 Biro Eniko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>

using namespace std;

double a = 1.0;
double b = 1.0;
double c = 3.0;
//int sigma = 0.001;    // 10^(-3)
double sigma = 10.0;
// KNN
int K = 5;

// euclidean distance of two vectors
double euclideanDistance(vector<int> first, vector<int> second)
{
    double sum = 0;
    for (int i = 0; i < 65; i++)
        sum += pow((first[i] - second[i]),2);
    return sqrt(sum);
}

struct smallest
{
    double distance;
    int index;
};

bool compareByDistance(const smallest &a, const smallest &b)
{
    return a.distance < b.distance;
}

vector<int> vectorsMinus(vector<int> v1, vector<int> v2)
{
    vector<int> difference;
    difference.resize(v1.size());
    for (uint i = 0; i < v1.size(); i++)
    {
        difference[i] = v1[i] - v2[i];
    }
    return difference;
}

double euclideanNorm(vector<int> v)
{
    double sum = 0;
    for (int i = 0; i < 65; i++)
        sum += pow(v[i], 2);
    return sqrt(sum);
}

double kernelLinear(vector<int> x, vector<int> y)
{
    double sum = 0;
    for (uint i = 0; i < x.size(); i++)
        sum += x[i] * y[i];
    return sum;
}

double kernelPolinomial(vector<int> x, vector<int> y)
{
    double sum = 0;
    for (uint i = 0; i < x.size(); i++)
        sum += x[i] * y[i];
    return pow(a*sum+b, 2);
}

double kernelGauss(vector<int> x, vector<int> y)
{
    double sum = (-1.0/(2.0*pow(sigma,2)))*pow(euclideanNorm(vectorsMinus(x,y)),2);
    return exp(sum);
}

// whichKernel:
// 0 - linear
// 1 - polinomial
// 2 - Gauss
// ||x -y||^2 = <x-y, x-y> = K(x, x) + K(y, y) - 2*K(x, y) -> the distance is defined in function of the scalar product
double kernelDistance(int whichKernel, vector<int> x, vector<int> y)
{
    if (whichKernel == 0)
        return (kernelLinear(x, x) + kernelLinear(y, y) - 2*kernelLinear(x, y));
    else if (whichKernel == 1)
        return (kernelPolinomial(x, x) + kernelPolinomial(y, y) - 2*kernelPolinomial(x, y));
    else
    {
        return (kernelGauss(x, x) + kernelGauss(y, y) - 2*kernelGauss(x, y));
    }
}

void KNN(int whichKernel, vector<vector<int>>& training, vector<vector<int>>& testing, int factor)
{
    int totalOK = 0;
    int howManyToBeGuessed[10] = {0};
    int howManyAreGuessed[10] = {0};

    // TEST and TRAINING data
    // for every testing data...
    for (uint i = 0; i < testing.size(); i++)
    {
        vector<smallest> closest;
        // with every training data
        for (uint j = 0; j < training.size()/factor; j++)
        {
            // we put in the first K
            if (j < K)
            {
                smallest K;
                K.distance = kernelDistance(whichKernel, testing[i], training[j]);
                K.index = j;
                closest.push_back(K);

                // sort the kNN vector
                sort(closest.begin(), closest.end(), compareByDistance);
            }
            else
            {
                double distance = kernelDistance(whichKernel, testing[i], training[j]);
                if (distance < closest[K-1].distance)
                {
                    closest[K-1].distance = distance;
                    closest[K-1].index = j;
                    sort(closest.begin(), closest.end(), compareByDistance);
                }
            }
        }

        // from the closest K which number occurs the most?
        vector<int> digits;
        digits.resize(10);
        for (int i = 0; i < K; i++)
            digits[training[closest[i].index][64]]++;

        int max = 0;
        int maxindex;
        for (int i = 0; i < 10; i++)
        {
            if (digits[i] > max)
            {
                max = digits[i];
                maxindex = i;
            }
        }
        int result = maxindex;

        cout << "The " << i << "th is guessed as " << result << " and is " << testing[i][64];
        if (result == testing[i][64])
        {
            totalOK++;
            howManyAreGuessed[testing[i][64]]++;
            cout << endl;
        }
        else
        {
            cout << " wrong" << endl;
        }
        howManyToBeGuessed[testing[i][64]]++;
    }


    // TRAINING and TRAINING data
    int totalOK2 = 0;
    int howManyToBeGuessed2[10] = {0};
    int howManyAreGuessed2[10] = {0};

    // for every training data...
    for (uint i = 0; i < training.size(); i++)
    {
        vector<smallest> closest;
        // with every training data
        for (uint j = 0; j < training.size()/factor; j++)
        {
            // we put in the first K
            if (j < K)
            {
                smallest K;
                K.distance = kernelDistance(whichKernel, training[i], training[j]);
                K.index = j;
                closest.push_back(K);

                // sort the kNN vector
                sort(closest.begin(), closest.end(), compareByDistance);
            }
            else
            {
                double distance = kernelDistance(whichKernel, training[i], training[j]);
                if (distance < closest[K-1].distance)
                {
                    closest[K-1].distance = distance;
                    closest[K-1].index = j;
                    sort(closest.begin(), closest.end(), compareByDistance);
                }
            }
        }

        // from the closest K which number occurs the most?
        vector<int> digits;
        digits.resize(10);
        for (int i = 0; i < K; i++)
            digits[training[closest[i].index][64]]++;

        int max = 0;
        int maxindex;
        for (int i = 0; i < 10; i++)
        {
            if (digits[i] > max)
            {
                max = digits[i];
                maxindex = i;
            }
        }
        int result = maxindex;

        cout << "The " << i << "th is guessed as " << result << " and is " << training[i][64];
        if (result == training[i][64])
        {
            totalOK2++;
            howManyAreGuessed2[training[i][64]]++;
            cout << endl;
        }
        else
        {
            cout << " wrong" << endl;
        }
        howManyToBeGuessed2[training[i][64]]++;
    }

    cout << "=====  KNN  =========" << endl;
    cout << "~~~~~Test error~~~~~~~~~" << endl;
    if (whichKernel == 0)
        cout << "Linear kernel" << endl;
    else if (whichKernel == 1)
        cout << "Polinomial kernel" << endl;
    else if (whichKernel == 2)
        cout << "Gauss kernel" << endl;

    cout << "Total: " << (1.0 - (double)totalOK/testing.size())*100.0 << endl;

    cout << "For every class:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << i << ": ";
        cout << (1.0 - (double)howManyAreGuessed[i]/howManyToBeGuessed[i])*100.0 << endl;
    }
    cout << endl;

    cout << "~~~~~Train error~~~~~~~~~" << endl;
    if (whichKernel == 0)
        cout << "Linear kernel" << endl;
    else if (whichKernel == 1)
        cout << "Polinomial kernel" << endl;
    else if (whichKernel == 2)
        cout << "Gauss kernel" << endl;

    cout << "Total: " << (1.0 - (double)totalOK2/training.size())*100.0 << endl;

    cout << "For every class:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << i << ": ";
        cout << (1.0 - (double)howManyAreGuessed2[i]/howManyToBeGuessed2[i])*100.0 << endl;
    }
    cout << endl;
}

void Centroid(int whichKernel, vector<vector<int>>& training, vector<vector<int>>& testing)
{
    // we make a super pattern for every class
    vector<vector<int>> totalTraining;
    totalTraining.resize(10);
    for (int i = 0; i < 10; i++)
        totalTraining[i].resize(64);

    int howManyThereIs[10] = {0};

    for (uint i = 0; i < training.size(); i++)
    {
        howManyThereIs[training[i][64]]++;
        for (uint j = 0; j < 64; j++)
            totalTraining[training[i][64]][j] += training[i][j];
    }

    for (uint i = 0; i < 10; i++)
    {
        for (uint j = 0; j < 64; j++)
        {
            totalTraining[i][j] /= howManyThereIs[i];
            totalTraining[i][j] = (int)totalTraining[i][j];
        }
    }

    int totalOK = 0;
    int howManyToBeGuessed[10] = {0};
    int howManyAreGuessed[10] = {0};


    // TEST data and super data 
    // for every testing data...
    for (uint i = 0; i < testing.size(); i++)
    {
        vector<smallest> distances;
        // with every SUPER data
        for (uint j = 0; j < 10; j++)
        {
            smallest K;
            K.distance = kernelDistance(whichKernel, testing[i], totalTraining[j]);
            K.index = j;
            distances.push_back(K);
        }
        sort(distances.begin(), distances.end(), compareByDistance);

        int result = distances[0].index;

        if (result == testing[i][64])
        {
            totalOK++;
            howManyAreGuessed[testing[i][64]]++;
        }
        howManyToBeGuessed[testing[i][64]]++;
    }

    int totalOK2 = 0;
    int howManyToBeGuessed2[10] = {0};
    int howManyAreGuessed2[10] = {0};

    // TRAINING data and SUPER data
    // for every testing data...
    for (uint i = 0; i < training.size(); i++)
    {
        vector<smallest> distances;
        // with every SUPER data
        for (uint j = 0; j < 10; j++)
        {
            smallest K;
            K.distance = kernelDistance(whichKernel, training[i], totalTraining[j]);
            K.index = j;
            distances.push_back(K);
        }
        sort(distances.begin(), distances.end(), compareByDistance);

        int result = distances[0].index;

        if (result == training[i][64])
        {
            totalOK2++;
            howManyAreGuessed2[training[i][64]]++;
        }
        howManyToBeGuessed2[training[i][64]]++;
    }

    cout << "=====  Centroid  =======" << endl;
    cout << "~~~~~Test error~~~~~~~~~" << endl;
    if (whichKernel == 0)
        cout << "Linear kernel" << endl;
    else if (whichKernel == 1)
        cout << "Polinomial kernel" << endl;
    else if (whichKernel == 2)
        cout << "Gauss kernel" << endl;

    cout << "Total: " << (1.0 - (double)totalOK/testing.size())*100.0 << endl;

    cout << "For every class:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << i << ": ";
        cout << (1.0 - (double)howManyAreGuessed[i]/howManyToBeGuessed[i])*100.0 << endl;
    }
    cout << endl;

    cout << "~~~~~Train error~~~~~~~~~" << endl;
    if (whichKernel == 0)
        cout << "Linear kernel" << endl;
    else if (whichKernel == 1)
        cout << "Polinomial kernel" << endl;
    else if (whichKernel == 2)
        cout << "Gauss kernel" << endl;

    cout << "Total: " << (1.0 - (double)totalOK2/training.size())*100.0 << endl;

    cout << "For every class:" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << i << ": ";
        cout << (1.0 - (double)howManyAreGuessed2[i]/howManyToBeGuessed2[i])*100.0 << endl;
    }
    cout << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        // 0 - linear kernel
        // 1 - polinomial kernel
        // 2 - Gauss kernel
        printf("usage: ocr 0|1|2\n0 - linear kernel\n1 - polinomial kernel\n2 - Gauss kernel\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    if (n != 0 && n != 1 && n != 2)
    {
        printf("the kernel number can only be 0, 1 or 2! \n0 - linear kernel\n1 - polinomial kernel\n2 - Gauss kernel\n");
        exit(1);
    }

    // read in the TRAIN and the TEST data
    vector<vector<int>> training;
    vector<vector<int>> testing;

    // TRAIN
    ifstream infile("optdigits.tra");
    string line;
    while (getline(infile, line))
    {
        vector<int> temp;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ','))
            temp.push_back(stoi(token));

        training.push_back(temp);
    }
    infile.close();

    // TEST
    ifstream infile2("optdigits.tes");
    while (getline(infile2, line))
    {
        vector<int> temp;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ','))
            temp.push_back(stoi(token));

        testing.push_back(temp);
    }

    // KNN
    int factor = 1;
    KNN(n, training, testing, factor);

    // CENTROID
    Centroid(n, training, testing);

    return 0;
}
