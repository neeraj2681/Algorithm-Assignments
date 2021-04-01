#include<cstdio>
#include <iostream>
#include <cmath>
#include <complex>
#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1);
typedef complex<double> base;

/*
 * check whether to apply FFT or Inverse FFT
 */
void fft (vector<base> & arr, bool invert) {
    int n = (int) arr.size();
    if (n == 1)  return;

    vector<base> arr_even (n/2);  //to store the even indexed terms
    vector<base> arr_odd (n/2); // to store the odd indexed terms
    for (int i=0, j=0; i<n; i+=2, ++j) {
        arr_even[j] = arr[i];
        arr_odd[j] = arr[i+1];
    }
    fft (arr_even, invert);     //applying fft on even array
    fft (arr_odd, invert);      //applying fft on odd array

    double ang = 2*PI/n * (invert ? -1 : 1);    // this angle is w(k,n)=e^(2*pi*k/n) where w is nth root of unity
    base w (1),  wn (cos(ang), sin(ang));       //e^(ix)=cos(x)+isin(x)

    for (int i=0; i<n/2; ++i) {
        arr[i] = arr_even[i] + w * arr_odd[i];
        arr[i+n/2] = arr_even[i] - w * arr_odd[i];
        if (invert)   //apply inverse FFT if true
            arr[i] /= 2,  arr[i+n/2] /= 2;
        w *= wn;
    }
}


/*
 * Multiply two polynomials passed as vectors
 */
void multiply_polynomials (const vector<int> & A, const vector<int> & B, vector<int> & result) {
    vector<base> fa (A.begin(), A.end());
    vector<base> fb (B.begin(), B.end());

    size_t n = 1;
    //adjust the degree size of polynomial by increasing n
    while (n < max (A.size(), B.size()))  n <<= 1;
    n <<= 1;
    fa.resize (n);
    fb.resize (n);

    //Doubling the degree bound of polynomial
    //Adding n higher order zero coefficient and applying fft to evaluate at double points
    fft (fa, false);
    fft (fb, false);
    for (size_t i=0; i<n; ++i)
        fa[i] *= fb[i];

    fft (fa, true);  //applying inverse FFT

    result.resize (n);
    for (size_t i=0; i<n; ++i)
        result[i] = int (fa[i].real());
}

int main(){

    int deg1, deg2;
    cout<<"Enter the degree of the first polynomial: ";
    cin>>deg1;
    int x;
    vector<int> A;
    printf("Enter the coefficients of the polynomial(space separated integers in ascending order of degree): ");
    for (int i = 0; i <= deg1; i++) {
        cin>>x;
        A.push_back(x);
    }
    cout<< "Enter the degree of the second polynomial: ";
    cin>>deg2;
    vector<int> B;
    printf("Enter the coefficients of the polynomial(space separated integers in ascending order of degree): ");
    for (int i = 0; i <= deg2; i++) {
        cin>>x;
        B.push_back(x);
    }

    vector <int> C; // to store the output polynomial coefficients
    multiply_polynomials(A, B, C);

    for (int i = 0; i <= deg1 + deg2; i++)
    {
        cout << C[i] << " ";
    }
    printf("\n");
}