#include "rl_enc1.h"

void rl_enc1::process() {

    int     count;
    int     k;
    int     value;

    while (1) {
        // read and write DC value
        ask_i.write(true);
        wait();
        while (!ready_i.read()) wait();
        value = input.read();
        ask_i.write(false);
        wait();
        output.write(value);

        count = 0;

        for( k = 1 ; k < 64 ; k++ ) {
            ask_i.write(true);
            wait();
            while (!ready_i.read()) wait();
            value = input.read();
            ask_i.write(false);
            wait();
            if ( value == 0 ) {
                count++;
            }
            else {
                output.write(count);
                count = 0;
                output.write(value);
            }
        }
        output.write(63);
    }
}

