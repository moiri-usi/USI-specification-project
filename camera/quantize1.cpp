#include "quantize1.h"

void quantize1::process() {

    sc_fixed<IN_WITDH_1, IN_WITDH_2, SC_RND, SC_SAT>  value;
    sc_uint<INC_WITDH> i_temp;
    sc_uint<INC_WITDH> j_temp;

    if (reset.read()) {
        i = 0;
        j = 0;
        ask_i.write(false);
        ready_o.write(false);
        state.write(RESET);
    }
    else {
        switch (state) {
            case RESET:
                ask_i.write(true);
                ready_o.write(false);
                state = CHECKLOOP;
                break;
            case CHECKLOOP:
                if (j >= (sc_uint<INC_WITDH>)8) {
                    j = 0;
                    if (i >= (sc_uint<INC_WITDH>)7) {
                        i = 0;
                    }
                    else {
                        i_temp = i;
                        i = i_temp + 1;
                    }
                }
                ask_i.write(true);
                ready_o.write(false);
                state = WAITREAD;
                break;
            case WAITREAD:
                if (ready_i.read()) {
                    ready_o.write(false);
                    value = input.read();
                    ask_i.write(false);
                    i_temp = i;
                    j_temp = j;
                    temp_out=(sc_int<OUT_WITDH>)(floor(
                                value*quantization[i_temp * 8 + j_temp]+0.5));
                    state = WAITWRITE;
                }
                break;
            case WAITWRITE:
                if (ask_o.read()) {
                    output.write(temp_out);
                    ask_i.write(true);
                    ready_o.write(true);
                    j_temp = j;
                    j = j_temp + 1;
                    state = CHECKLOOP;
                }
                break;
            deafult:
                state.write(CHECKLOOP);
                ask_i.write(true);
                ready_o.write(false);
        }
    }
}

