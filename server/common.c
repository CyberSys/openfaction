#include <ctype.h>
#include <stdio.h>
#include <limits.h>

void dump(char const *prefix, void const *data, size_t size, FILE *file)
{
    int indent = fprintf(file, "%s", prefix), end_of_line;
    if (!size) {
        fputs("nothing\n", file);
        return;
    }

    unsigned char const * const byte = (unsigned char *) data;
    size_t x = 0;
    do {
        fprintf(file, "%02X ", byte[x]);
        end_of_line = x++ && x % 16 == 0;
        if (!end_of_line) {
            continue;
        }

        fputs("   ", file);
        x -= 16;
        do {
            fputc(isgraph(byte[x]) ? byte[x] : '.', file);
        } while (++x % 16);
    } while (x < size && (!end_of_line || fprintf(file, "\n%*s", indent, "")));

    fprintf(file, "%*s", 3 * -~-~(~x % 16), "");
    x -= x % 16;

    while (x < size) {
        fputc(isgraph(byte[x]) ? byte[x] : '.', file);
        x++;
    }

    fputc('\n', file);
}

unsigned long crc_loop(unsigned long crc, unsigned char *byte, size_t size) {
    for (size_t x = 0; x < size; x++) {
        crc ^= byte[x];
        for (size_t y = 0; y < 8; y++) {
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
        }
    }
    return crc;
}

void pack_little_754(void *destination, long double source, size_t exponent_size, size_t significand_size) {
    unsigned char *d = destination;
    int sign = source < 0.0;
    unsigned long long x = pack_abs_754(sign ? -source : source, exponent_size, significand_size);
    x = (x >> 7 << 8) + (sign << 7) + (x % 0x80);

    for (size_t size = (exponent_size + significand_size) / 8; size > 0; size--, *d++ = x % 0x100, x /= 0x100);
}

unsigned long long pack_abs_754(long double source, size_t exponent_size, size_t significand_size) {
    unsigned long long x = 0;

    while (source >= 2.0) {
        source /= 2.0;
        x++;
    }

    while (source < 1.0) {
        source *= 2.0;
        x--;
    }

    source -= 1.0;
    source *= (1ULL << significand_size) + 0.5f;

    x += ~(ULLONG_MAX << exponent_size - 1);
    x <<= significand_size;
    x |= (unsigned long long) source;

    return x;
}

long double unpack_little_754(void *source, size_t exponent_size, size_t significand_size) {
    unsigned char *s = source;
    unsigned long long x = 0;
    for (size_t size = (exponent_size + significand_size) / 8; size > 0; x *= 0x100, x += s[--size]);

    int sign = x & 0x80;
    x = (x >> 8) + x % 0x80;

    return sign ? -unpack_abs_754(x, exponent_size, significand_size) : unpack_abs_754(x, exponent_size, significand_size);
}

long double unpack_abs_754(unsigned long long source, size_t exponent_size, size_t significand_size) {
    long double x = source & ~(ULLONG_MAX << significand_size);
    x /= 1ULL << significand_size;
    x += 1.0f;

    source >>= significand_size;
    source -= ~(ULLONG_MAX << exponent_size - 1);

    while (source >= (unsigned long long) LLONG_MIN) {
        x /= 2.0;
        source++;
    }

    while (source > 0) {
        x *= 2.0;
        source++;
    }

    return x;
}
