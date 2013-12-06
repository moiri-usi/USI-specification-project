#include "zz_enc1.h"

void zz_enc1::process() {

    int     i, j;
    int     temp_block[64];
    int     block[64];

    int zig_zag[64] = {
        0, 1, 8, 16, 9, 2, 3, 10,
        17, 24, 32, 25, 18, 11, 4, 5,
        12, 19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63};

    while(1) {
        //read in the blocks for 8 lines
        ask_i.write(true);
        ready_o.write(false);
        wait();

        for ( i = 0 ; i < 64 ; i ++) {
            ask_i.write(true);
            wait();
            while (!ready_i.read()) wait();
            ready_o.write(false);
            temp_block[i]= input.read();
            ask_i.write(false);
            wait();
        }

        for (i=0; i < 64 ; i++){
            ready_o.write(false);
            wait();
            while (!ask_o.read()) wait();
            block[i] = temp_block[zig_zag[i]];
            output.write (block[i]);
            ready_o.write(true);
            wait();
        }
    }
}

