#include <algorithm>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>

int main()
{
    std::string fname;

    std::cout << "Enter Path to Datafile: ";
    std::cin >> fname;

    std::ifstream fin;
    fin.open(fname);
    if( !fin.is_open() ) {
        std::cerr << "Cannot Find Datafile\n";
        return 2;
    }

    std::vector< std::string > ctexts;
    std::string s;
    while( fin >> s ) {
        ctexts.push_back(s);
    }

    uint64_t max_count = 0;
    std::string ecb_text;

    for(auto& ctext : ctexts) {
        std::vector< std::string > blocks;
        for(size_t i = 0 ; i < ctext.size()/32 ; ++i ) {
            blocks.emplace_back(ctext.begin() + i*32,
                                ctext.begin() + (i+1)*32);
        }

        uint64_t count = 0;
        for(auto& block : blocks) {
            count += std::count(blocks.begin(), blocks.end(), block);
        }

        if(count > max_count) {
            max_count = count;
            ecb_text = ctext;
        }
    }

    std::cout << "Text encrypted with ECB is:\n" << ecb_text << std::endl;

}
