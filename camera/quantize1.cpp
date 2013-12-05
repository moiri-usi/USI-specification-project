#include "quantize1.h"

void quantize1::process() {

    int     i, j;
    sc_fixed<23, 13, SC_RND, SC_SAT>  value;
    int     temp_out;

    while (1) {
        ask_i.write(true);
        ready_o.write(false);
        wait();
        for(i=0;i<8;i++) {
            for (j=0; j<8;j++){
                while (!ready_i.read()) wait();
                ready_o.write(false);
                value = input.read();
                ask_i.write(false);
                temp_out=(int)(floor(value/quantization[i*8+j]+0.5));
                wait();
                // cout << temp_out << " ";
                while (!ask_o.read()) wait();
                output.write(temp_out);
                ask_i.write(true);
                ready_o.write(true);
                wait();
            }
            // cout << endl;
        }
    }
}

