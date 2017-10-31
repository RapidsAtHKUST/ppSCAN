///
// Created by yche on 10/31/17.
//

#include <string>
#include <iostream>

using namespace std;

int main() {
    string a = "0.3000";
    a.erase(a.find_last_not_of("0") + 1);
    cout << a << endl;
}

