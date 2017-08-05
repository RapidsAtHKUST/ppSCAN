//
// Created by yche on 8/4/17.
//
#include <iostream>

using namespace std;

int main() {
    cout << sizeof(int) << endl;

    int a = 20;
    int b = 66;

    // before swapping
    std::cout << a  << ' ' << b  << '\n';

    // swap
    std::swap(a,b);

    // after swapping
    std::cout << a << ' ' << b << '\n';
}