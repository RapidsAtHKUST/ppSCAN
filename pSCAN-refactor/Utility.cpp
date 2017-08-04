#include "Utility.h"

FILE *open_file(const char *file_name, const char *mode) {
    FILE *f = fopen(file_name, mode);
    if (f == NULL) {
        printf("Can not open file: %s\n", file_name);
        exit(1);
    }

    return f;
}


pair<int, int> get_eps(const char *eps_s) {
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

    if (eps_a > eps_b || eps_b > 100 || eps_a <= 0) {
        cout << "??? Wrong eps format: " << eps_a << "," << eps_b << "," << eps_s << "\n";
        exit(1);
    }

    return make_pair(eps_a * eps_a, eps_b * eps_b);
}

