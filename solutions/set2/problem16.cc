#include <util/aes.h>
#include <util/encoding.h>
#include <util/padding.h>

#include <iostream>
#include <algorithm>

std::vector<uint8_t> key = generate_random_key(16);
std::vector<uint8_t> iv  = generate_random_key(16);

std::vector<uint8_t> encrypt( const std::string &s )
{
    //Quote out ; and =
    std::string to_encrypt = "comment1=cooking%20MCs;userdata=";

    for( auto c : s ) {
        if( c == ';' || c == '=' ) {
            to_encrypt += '"';
            to_encrypt += c;
            to_encrypt += '"';
        } else {
            to_encrypt += c;
        }
    }

    to_encrypt += ";comment2=%20like%20a%20pound%20of%20bacon";

    std::vector<uint8_t> to_encrypt_bs(to_encrypt.begin(), to_encrypt.end());

    return AES128_CBC_encrypt(pkcs7_pad(to_encrypt_bs, 16), key, iv);
}

std::string decrypt( const std::vector<uint8_t>& ctext )
{
    auto ptext = pkcs7_unpad( AES128_CBC_decrypt(ctext, key, iv) );
    return std::string(ptext.begin(), ptext.end());
}

inline bool is_admin(const std::string& s)
{
    return s.find(";admin=true;") != std::string::npos;
}

void verify(const std::vector<uint8_t>& ctext)
{
    try {
        if( is_admin(decrypt( ctext )) ) {
            std::cout << "Cookie given is an admin" << std::endl;
        } else {
            std::cout << "Cookie given isn't an admin" << std::endl;
        }
    } catch( InvalidPadding & e ) {
        std::cout << "Decrypted cookie is padded incorrectly" << std::endl;
    }
}

int main()
{
    std::cout << "Testing sanitization of input string:" << std::endl;
    verify( encrypt(";admin=true;") );
    std::cout << std::endl;

    std::cout << "Attempting modifed ciphertext attack:" << std::endl;

    // Use the API to encrypt a known block of data and
    // an extra one we can mess with
    auto ctext = encrypt("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    // Choose plaintext block we want in the ciphertext
    std::string inject_str = ";admin=true;fooo";
    std::vector<uint8_t> inject_block(inject_str.begin(), inject_str.end());

    // Split the ciphertext into blocks
    std::vector< std::vector<uint8_t> > ctext_blocks;
    for(size_t i = 0 ; i < ctext.size() / 16 ; ++i ) {
        ctext_blocks.push_back( std::vector<uint8_t>(ctext.begin() + i*16,
                                                     ctext.begin() + (i+1)*16) );
    }

    // Construct the block of ciphertext that will decrypt into our
    // desired plaintext
    auto prev_block = ctext_blocks[2];

    std::vector<uint8_t> crafty_block;
    for( size_t i = 0 ; i < 16 ; ++i ) {
        uint8_t x = 'a' ^ prev_block[i];
        crafty_block.push_back( inject_block[i] ^ x );
    }

    // Concatinate our new ciphertext together
    ctext_blocks[2] = crafty_block;
    std::vector<uint8_t> new_ctext;
    for(auto & b : ctext_blocks ) {
        new_ctext.insert(new_ctext.end(), b.begin(), b.end());
    }

    // Are we an admin now?
    verify(new_ctext);
}
