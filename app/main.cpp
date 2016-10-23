#include <cstdio>
#include <sp.h>

int main() {
    int major, minor, patch;
    SP_version(&major, &minor, &patch);
    printf("Spread version: %d.%d.%d\n", major, minor, patch);
    return 0;
}
