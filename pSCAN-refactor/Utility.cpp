#include "Utility.h"
#include <iostream>

FILE *open_file(const char *file_name, const char *mode) {
    FILE *f = fopen(file_name, mode);
    if (f == nullptr) {
        cout << "Can not open file: " << file_name << endl;
        exit(1);
    }

    return f;
}

pair<int, int> get_eps(const char *eps_s) {
    int i = 0, eps_numerator = 0, eps_denominator = 1;
    while (eps_s[i] != '\0' && eps_s[i] != '.') {
        eps_numerator = eps_numerator * 10 + (eps_s[i] - '0');
        ++i;
    }

    if (eps_s[i] == '.') {
        ++i;
        while (eps_s[i] != '\0') {
            eps_numerator = eps_numerator * 10 + (eps_s[i] - '0');
            eps_denominator *= 10;
            ++i;
        }
    }

    return make_pair(eps_numerator * eps_numerator, eps_denominator * eps_denominator);
}

