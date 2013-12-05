#include "rl_enc1.h"

void rl_enc1::process() {

    int     count;
    int     k;
    int     value;

    while (1) {
        // read and write DC value
        ask_i.write(true);
        ready_o.write(false);
        wait();
        while (!ready_i.read()) wait();
        value = input.read();
        ask_i.write(false);
        wait();
        while (!ask_o.read()) wait();
        output.write(value);
        ready_o.write(true);
        wait();

        count = 0;

        for( k = 1 ; k < 64 ; k++ ) {
            ask_i.write(true);
            ready_o.write(false);
            wait();
            while (!ready_i.read()) wait();
            value = input.read();
            ask_i.write(false);
            wait();
            if ( value == 0 ) {
                count++;
            }
            else {
                while (!ask_o.read()) wait();
                output.write(count);
                ready_o.write(true);
                wait();
                count = 0;
                ready_o.write(false);
                wait();
                while (!ask_o.read()) wait();
                output.write(value);
                ready_o.write(true);
                wait();
            }
        }
        ready_o.write(false);
        wait();
        while (!ask_o.read()) wait();
        output.write(static_cast<sc_int<32> >(63));
        ready_o.write(true);
        wait();
    }
}

