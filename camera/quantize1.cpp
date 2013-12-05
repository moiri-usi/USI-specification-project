#include "quantize1.h"

void quantize1::process() {

    sc_fixed<23, 13, SC_RND, SC_SAT>  value;
    int i_temp;
    int j_temp;

    if (reset.read()) {
        i = 0;
        j = 0;
        ask_i.write(false);
        ready_o.write(false);
        state.write(CHECKLOOP);
        cout << "RESET      ai:" << ask_i << " ri:" << ready_i << " ao:" << ask_o << " ro:" << ready_o << endl;
    }
    else {
        switch (state) {
            case CHECKLOOP:
                ready_o.write(false);
                if (i >= 8) {
                    j = 0;
                    i = 0;
                    cout << "START_OVER" << endl;
                }
                else {
                    if(j >= 8) {
                        j = 0;
                        i_temp = i;
                        i = i_temp + 1;
                    }
                    ask_i.write(true);
                    state = WAITREAD;
                }
                cout << "CHECKLOOP  ai:" << ask_i << " ri:" << ready_i << " ao:" << ask_o << " ro:" << ready_o << endl;
                break;
            case WAITREAD:
                if (ready_i.read()) {
                    ready_o.write(false);
                    value = input.read();
                    ask_i.write(false);
                    i_temp = i;
                    i_temp = i_temp * 8;
                    j_temp = j;
                    temp_out=(int)(floor(value/quantization[i_temp + j_temp]+0.5));
                    state = WAITWRITE;
                }
                cout << "WAITREAD   ai:" << ask_i << " ri:" << ready_i << " ao:" << ask_o << " ro:" << ready_o << endl;
                break;
            case READ:
                state = WAITWRITE;
                cout << "READ       ai:" << ask_i << " ri:" << ready_i << " ao:" << ask_o << " ro:" << ready_o << endl;
                break;
            case WAITWRITE:
                if (ask_o.read()) {
                    output.write(temp_out);
                    //ask_i.write(true);
                    //ask_i.write(false);
                    ready_o.write(true);
                    j_temp = j;
                    j = j_temp + 1;
                    state = CHECKLOOP;
                    //state = TERMINATION;
                }
                cout << "WAITWRITE  ai:" << ask_i << " ri:" << ready_i << " ao:" << ask_o << " ro:" << ready_o << endl;
                break;
            case TERMINATION:
                state = CHECKLOOP;
                ask_i.write(true);
                ready_o.write(false);
                break;
            deafult:
                state.write(CHECKLOOP);
                ask_i.write(true);
                ready_o.write(false);
        }
    }
   // while (1) {
   //     ask_i.write(true);
   //     ready_o.write(false);
   //     wait();
   //     for(i=0;i<8;i++) {
   //         for (j=0; j<8;j++){
   //             while (!ready_i.read()) wait();
   //             ready_o.write(false);
   //             value = input.read();
   //             ask_i.write(false);
   //             temp_out=(int)(floor(value/quantization[i*8+j]+0.5));
   //             wait();
   //             // cout << temp_out << " ";
   //             while (!ask_o.read()) wait();
   //             output.write(temp_out);
   //             ask_i.write(true);
   //             ready_o.write(true);
   //             wait();
   //         }
   //         // cout << endl;
   //     }
   // }
}

