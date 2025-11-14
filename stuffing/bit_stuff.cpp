#include <iostream>

std::string bitStuff(std::string bits) {
    std::string result = "";
    int countOnes = 0;
    for (const auto& c : bits) {
        if (c == '0') {
            result += '0';
            countOnes = 0;
        } else { 
            result += '1';
            countOnes++;
        }

        if (countOnes == 5) {
            result += '0';
            countOnes = 0;
        }
    }
    return result;
}

std::string bitUnStuff(std::string stuff_bits) {
    std::string result = "";
    int countOnes = 0;
    for (const auto& c : stuff_bits) {
        if (countOnes == 5) {
            countOnes = 0;
            continue;
        } 
        if (c == '0') {
            result += '0';
            countOnes = 0;
        } else {
            result += '1';
            countOnes++;
        }
    }
    return result;
}

int main() {
    std::cout << "testing bit stuffing using 0111110101111100\n";
    std::string bitstream = "0111110101111100";  
    std::string stuffed_bitstream = bitStuff(bitstream);
    std::string unstuffed_bistream = bitUnStuff(stuffed_bitstream);

    bool same = (bitstream == unstuffed_bistream);
    std::cout << stuffed_bitstream << ", " << unstuffed_bistream << '\n';
    std::cout << same << '\n';
    return 0;
}