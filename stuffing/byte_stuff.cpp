#include <iostream>
const char flag = 0x7E;
const char esc = 0x7D;

std::string byteStuff(std::string bytes) {
    std::string result = "";
    result += flag;
    for (const auto& c : bytes) {
        if (c == flag || c == esc) {
            result += esc;
        }
        result += c;
    }
    result += flag;
    return result;
}

std::string byteUnStuff(std::string stuff_bytes) {
    size_t size = stuff_bytes.size();
    if (size < 2 ||
        stuff_bytes[0] != flag ||
        stuff_bytes[size - 1] != flag 
    ) return "";
    std::string result = "";
    for (int i = 1; i < size - 1; i++) {
        if (stuff_bytes[i] == esc) {
            if (i + 1 > size - 1) return "";
            i++;
        } 
        result += stuff_bytes[i];
    }
    return result;
}

int main() {
    std::cout << "Testing byte stuffing for 'hello 0x7E world'\n";
    std::string bytestream = "Hello 0x7E world";
    std::string stuffedbyte = byteStuff(bytestream);
    std::string unstuffedbyte = byteUnStuff(stuffedbyte);

    std::cout << stuffedbyte << ", " << unstuffedbyte << '\n';
    bool same = (bytestream == unstuffedbyte);
    std::cout << same <<  '\n';
    return 0;
}