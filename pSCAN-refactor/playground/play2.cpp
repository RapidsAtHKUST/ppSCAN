//
// Created by yche on 10/10/17.
//
#include <iostream>
#include <vector>
#include <algorithm>

#include "pretty_print.h"

using namespace std;

int main() {
    vector<int> my_vec = {0, 1, 2, 3, 4, 5};
    vector<int> my_vec2 = {4, 8, 0, 9, 5, 6};
    sort(begin(my_vec), end(my_vec), [&my_vec2](int u, int v) {
        return my_vec2[u] > my_vec2[v];
    });
    cout << my_vec << endl;
    vector<int> my_vec3;

    transform(begin(my_vec), end(my_vec), back_inserter(my_vec3), [&my_vec2](int u) -> int {
        return my_vec2[u];
    });
    cout << my_vec3 << endl;
}