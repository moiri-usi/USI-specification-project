#include "rl_enc1.h"

void rl_enc1::process() {

    int     count;
    int     k;
    int     value;

    while (1) {
        // read and write DC value
        output.write(input.read());

        count = 0;

        for( k = 1 ; k < 64 ; k++ ) {
            value = input.read();
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

