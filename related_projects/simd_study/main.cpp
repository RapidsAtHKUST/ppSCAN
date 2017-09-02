
#include <boost/simd/pack.hpp>
#include <iostream>

namespace bs = boost::simd;

int main() {
    bs::pack<float, 4> p{1.f, 2.f, 3.f, 4.f};
    std::cout << p + 10 * p << "\n";

    return 0;
}
