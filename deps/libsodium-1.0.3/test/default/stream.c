
#define TEST_NAME "stream"
#include "cmptest.h"

unsigned char firstkey[32]
    = { 0x1b, 0x27, 0x55, 0x64, 0x73, 0xe9, 0x85, 0xd4, 0x62, 0xcd, 0x51,
        0x19, 0x7a, 0x9a, 0x46, 0xc7, 0x60, 0x09, 0x54, 0x9e, 0xac, 0x64,
        0x74, 0xf2, 0x06, 0xc4, 0xee, 0x08, 0x44, 0xf6, 0x83, 0x89 };

unsigned char nonce[24] = { 0x69, 0x69, 0x6e, 0xe9, 0x55, 0xb6, 0x2b, 0x73,
                            0xcd, 0x62, 0xbd, 0xa8, 0x75, 0xfc, 0x73, 0xd6,
                            0x82, 0x19, 0xe0, 0x03, 0x6b, 0x7a, 0x0b, 0x37 };

unsigned char output[4194304];

unsigned char h[32];

int main(void)
{
    int i;

    crypto_stream(output, sizeof output, nonce, firstkey);
    crypto_hash_sha256(h, output, sizeof output);

    for (i = 0; i < 32; ++i)
        printf("%02x", h[i]);
    printf("\n");

    assert(sizeof output > 4000);

    crypto_stream_xsalsa20_xor_ic(output, output, 4000, nonce, 0U, firstkey);
    for (i = 0; i < 4000; ++i)
        assert(output[i] == 0);

    crypto_stream_xsalsa20_xor_ic(output, output, 4000, nonce, 1U, firstkey);
    crypto_hash_sha256(h, output, sizeof output);
    for (i = 0; i < 32; ++i)
        printf("%02x", h[i]);
    printf("\n");

    assert(crypto_stream_keybytes() > 0U);
    assert(crypto_stream_noncebytes() > 0U);
    assert(strcmp(crypto_stream_primitive(), "xsalsa20") == 0);
    assert(crypto_stream_keybytes() == crypto_stream_xsalsa20_keybytes());
    assert(crypto_stream_noncebytes() == crypto_stream_xsalsa20_noncebytes());

    return 0;
}
