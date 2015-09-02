#include <util/padding.h>

#include <string>
#include <iostream>

int main()
{
    std::string s = "YELLOW SUBMARINE";
    unsigned int block_size = 20;

    std::string padded = pkcs7_pad(s, block_size);

    std::cout << "Input String: " << s << std::endl;
    std::cout << "Input String Length: " << s.size() << std::endl;
    std::cout << "Block Size: " << block_size << std::endl;

    std::cout << "Padded String: " << padded << std::endl;
    std::cout << "Padded String Length: " << padded.size() << std::endl;
}
