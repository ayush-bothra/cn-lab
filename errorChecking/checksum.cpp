#include <iostream>
const uint mask = 0xFFFF;

uint findChecksum(std::string bytes) {
    u_int16_t acc = 0;
    u_int32_t sum = 0;
    int i = 0;
    for (; i < bytes.size() - 1; i += 2) {
        acc = (static_cast<u_int16_t>(static_cast<u_int8_t>(bytes[i])) << 8) 
            | (static_cast<u_int16_t>(static_cast<u_int8_t>(bytes[i + 1])));
        sum += acc;

        if (sum > mask) {
        sum = (sum & mask) + (sum >> 16);
        }
    }

    if (i < bytes.size()) { // padding with 00 for odd size
        acc = static_cast<u_int16_t>(static_cast<u_int8_t>(bytes[i])) << 8
                | 0x00u;
        sum += acc;
        if (sum > mask) sum += (acc & mask) + (acc >> 16);
    }

    while (sum > mask) {
        sum = (sum & mask) + (sum >> 16);
    }
    return (~sum & mask);
}

int main() {
    std::cout << "testing checksum on 'Hello world'\n";
    std::string bytestream = "Hello world";
    uint checksum = 0x8E31;

    uint found_checksum = findChecksum(bytestream);
    bool same = (found_checksum == checksum);
    std::cout << found_checksum << ", " << checksum << '\n';
    std::cout << same << '\n';
    return 0;
}