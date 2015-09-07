#include <util/padding.h>

#include <iostream>

void validate(const std::vector<uint8_t>& s)
{
    try {
        pkcs7_unpad(s);
        std::cout << "Valid Padding" << std::endl;
    } catch(const InvalidPadding & e) {
        std::cout << "Invalid Padding" << std::endl;
    }
}

int main()
{
    const std::string base = "ICE ICE BABY";

    std::vector<uint8_t> s1(base.begin(), base.end());
    s1.insert(s1.end(), 4, 4);
    validate(s1);


    std::vector<uint8_t> s2(base.begin(), base.end());
    s2.insert(s2.end(), 4, 5);
    validate(s2);

    std::vector<uint8_t> s3(base.begin(), base.end());
    for(int i = 1 ; i < 5 ; ++i) {
        s3.push_back(i);
    }
    validate(s3);

    return 0;
}
