#include <util/kv.h>
#include <util/aes.h>
#include <util/padding.h>
#include <util/encoding.h>

#include <iostream>
#include <algorithm>

std::vector<uint8_t> key = generate_random_key(16);


std::string profile_for(std::string email)
{
    std::remove(email.begin(), email.end(), '&');
    std::remove(email.begin(), email.end(), '=');

    return "email=" + email + "&uid=10&role=user";
}

std::vector<uint8_t> encrypted_profile_for(const std::string& email)
{
    std::string padded_ptext = pkcs7_pad( profile_for(email), 16);
    std::vector<uint8_t> ptext_bs(padded_ptext.begin(), padded_ptext.end());

    return AES128_ECB_encrypt(ptext_bs, key);
}

std::map<std::string, std::string> decrypt_profile(const std::vector<uint8_t> &ct)
{
    auto ptext_bs = pkcs7_unpad(AES128_ECB_decrypt(ct, key));
    std::string ptext(ptext_bs.begin(), ptext_bs.end());

    std::cout << "Decrypted Profile: " << ptext << std::endl;

    return parse_kv_cookie(ptext);
}

int main()
{
    // Wanted block is "admin" plus the correct padding
    std::string wanted_block = pkcs7_pad(std::string("admin"), 16);

    // Generate Profile with desired block in
    std::string input = std::string(10, 'a') + wanted_block;
    auto ctext = encrypted_profile_for(input);

    std::vector<uint8_t> block(ctext.begin() + 16, ctext.begin() + 32);

    // Generate dummpy profile
    auto admin_profile = encrypted_profile_for("me@hacked.com");

    // Replace the target block with our crafted one.
    admin_profile.resize(admin_profile.size() - 16);
    for(size_t i = 0 ; i < 16 ; ++i) {
        admin_profile.push_back(block[i]);
    }

    std::map<std::string, std::string> profile = decrypt_profile(admin_profile);

    std::cout << "profile['role'] = " << profile["role"] << std::endl;

    return 0;
}