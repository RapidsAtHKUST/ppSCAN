//
// Created by yche on 8/4/17.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

#include "pretty_print.h"

using namespace std;

void PlaySwap() {
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

void PlayEpsaAndEpsb() {
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

void PlaySelfIncrement() {
    auto my_vec = vector<int>{1, 2, 3, 4};
    cout << my_vec << endl;
    auto i = 1;
//    my_vec[++i] = 3;
    my_vec[i++] = 3;
    cout << my_vec << endl;
}


void PlayUnique() {
    vector<pair<int, int>> my_vec = {{1, 2},
                                     {1, 2},
                                     {1, 3},
                                     {4, 5},
                                     {3, 2},
                                     {3, 2}};
    sort(my_vec.begin(), my_vec.end());
    auto iter = unique(my_vec.begin(), my_vec.end());
    for_each(my_vec.begin(), iter, [](pair<int, int> ele) { cout << ele << "\n"; });
}


void PlaySetUnion() {
    vector<int> my_vec0 = {1, 4, 6};
    vector<int> my_vec1 = {1, 2, 3};
    vector<int> another;
    std::merge(my_vec0.begin(), my_vec0.end(), my_vec1.begin(), my_vec1.end(), back_inserter(another));
    cout << another << endl;
}

void PlaySort() {
    vector<pair<int, int>> lines;
    lines.emplace_back(2, 3);
    lines.emplace_back(2, 4);
    lines.emplace_back(1, 3);
    lines.emplace_back(1, 2);
    sort(lines.begin(), lines.end(), [](const pair<int, int> &left, const pair<int, int> &right) {
        if (left.first == right.first) {
            return left.second < right.second;
        }
        return left.first < right.first;
    });
    cout << lines << endl;
}

void PlayTime() {
    // http://en.cppreference.com/w/cpp/chrono/time_point
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
//    std::cout << "24 hours ago, the time was "
//              << std::put_time(std::localtime(&now_c), "%F %T") << '\n';

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::cout << "Hello World\n";
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Printing took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "us.\n";

    auto unix_timestamp = std::chrono::seconds(std::time(NULL));
    auto unix_timestamp_x_1000 = std::chrono::milliseconds(unix_timestamp).count();
    cout << "unix timestamp:" << unix_timestamp_x_1000 << endl;
}

int main() {
    PlaySwap();
    PlayEpsaAndEpsb();
    PlaySelfIncrement();
    PlayUnique();
    PlaySetUnion();
    PlaySort();
    PlayTime();
}