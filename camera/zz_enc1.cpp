#include "zz_enc1.h"

void zz_enc1::process() {

    sc_int<9> zig_zag[64] = {0,1,8,16,9,2,3,10 ,
        17,24,32,25,18,11,4,5 ,
        12,19, 26, 33, 40, 48, 41, 34,
        27, 20, 13, 6, 7, 14, 21, 28,
        35, 42, 49, 56, 57, 50, 43, 36,
        29, 22, 15, 23, 30, 37, 44, 51,
        58, 59, 52, 45, 38, 31, 39, 46,
        53, 60, 61, 54, 47, 55, 62, 63};

    sc_int<32> i_temp;

    if (reset.read() == true ){
        state.write(RESET);
        ready_o.write(false);
        ask_i.write(false);
    } else {
        switch (state.read()){
            case RESET:
                ask_i.write(true);      //indicate that we want to read in next cycle
                ready_o.write(false);   //indicate that we do not write data in this cycle
                state.write(READZIGZAG);
                i = 0;
                break;

            case READZIGZAG:
                if (i < ((sc_int<32>)(64))){
                    ask_i.write(true);
                    state.write(WAITREAD);
                }
                else{
                    state.write(WRITEZIGZAG);
                    i = 0;
                }
                break;

            case WAITREAD:
                if (ready_i.read()==true){
                    ready_o.write(false);
                    i_temp = i;
                    block[i_temp] = input.read();
                    ask_i.write(false);
                    i_temp++;
                    i = i_temp;
                    state.write(READZIGZAG);
                }
                break;

            case WRITEZIGZAG:
                if (i < ((sc_int<32>)(64))){
                    ready_o.write(false);
                    state.write(WAITWRITE);
                }
                else{
                    ready_o.write(false);
                    i = 0;
                    ask_i.write(true);
                    state.write(READZIGZAG);
                }
                break;

            case WAITWRITE:
                if(ask_o.read()==true){
                    i_temp = i;
                    output.write((sc_int<32>)block[zig_zag[i_temp]]);
                    ask_i.write(false); // check it out!!
                    ready_o.write(true);//count to zeros
                    i_temp++;
                    i = i_temp;
                    state.write(WRITEZIGZAG);
                }
                break;
        }
    }
}

