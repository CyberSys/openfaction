#include <ctype.h>
#include <stdio.h>

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
