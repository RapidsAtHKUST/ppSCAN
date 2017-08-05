//
// Created by yche on 8/4/17.
//
#include <iostream>
#include <vector>

#include "pretty_print.h"

using namespace std;

void play_swap() {
    cout << sizeof(int) << endl;

    int a = 20;
    int b = 66;

    // before swapping
    std::cout << a << ' ' << b << '\n';

    // swap
    std::swap(a, b);

    // after swapping
    std::cout << a << ' ' << b << '\n';
}

void play_epsa_and_epsb() {
    const char *eps_s = string("0.13").c_str();
    int i = 0, eps_a = 0, eps_b = 1;
    while (eps_s[i] != '\0' && eps_s[i] != '.') {
        eps_a = eps_a * 10 + (eps_s[i] - '0');
        ++i;
    }

    if (eps_s[i] == '.') {
        ++i;
        while (eps_s[i] != '\0') {
            eps_a = eps_a * 10 + (eps_s[i] - '0');
            eps_b *= 10;
            ++i;
        }
    }

    cout << eps_a << " " << eps_b << endl;
    // result: 13 100

    if (eps_a > eps_b || eps_b > 100 || eps_a <= 0) {
        cout << "??? Wrong eps format: " << eps_a << "," << eps_b << "," << eps_s << "\n";
        exit(1);
    }
}

void play_increment() {
    auto my_vec = vector<int>{1, 2, 3, 4};
    cout << my_vec << endl;
    auto i = 1;
//    my_vec[++i] = 3;
    my_vec[i++] = 3;
    cout << my_vec << endl;
}

int main() {
//    play_epsa_and_epsb();
    play_increment();
}