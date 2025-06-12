#include <stdlib.h>
#include "bmp.h"
#include "cmd.h"

int main() {
    bmp_fileheader* bmp_fh = calloc(1, sizeof(bmp_fileheader));
    bmp_infoheader* bmp_ih = calloc(1, sizeof(bmp_infoheader));
    cmdParser(bmp_fh, bmp_ih);
    return 0;
}
