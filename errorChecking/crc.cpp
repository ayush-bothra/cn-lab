#include <iostream>
#include <cstdint>
#include <bitset>

uint32_t crc(uint32_t x, uint32_t p) {
    int deg_x;
    int deg_p;
    int shift;

    for (int i = 31; i >= 0; i--) {
        if (p & (1 << i)) {
            deg_p = i;
            break;
        }
    }
    x = (x << deg_p);

    for (int i = 31; i >= 0; i--) {
        if (x & (1 << i)) {
            deg_x = i;
            break;
        }
    }

    while (deg_x > deg_p) {
        shift = deg_x - deg_p;
        x ^= (p << shift); 

        for (int i = 31; i >= 0; i--) {
            if (x & (1 << i)) {
                deg_x = i;
                break;
            }
        }
    }

    return x;
}

int main() {
    std::cout << "Testing CRC using the example 1011010\n";
    uint32_t x = 0b1011010;
    uint32_t p = 0b1011;

    uint32_t n_x = crc(x, p);
    std::cout <<  std::bitset<32>(n_x) << '\n';
    return 0;
}