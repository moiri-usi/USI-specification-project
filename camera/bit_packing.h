/*  bit_packing.h */
#ifndef _BIT_PACK
#define _BIT_PACK
#include <systemc.h>
#include "add2systemc.h"

#define BUS_WIDTH 8
#define RL_WIDTH  7
#define VAL_WIDTH 9

SC_MODULE(bit_packing){

    sc_in<sc_int<32> >      input;
    sc_in<bool>     ready_i;
    sc_in<bool>     clk;
    sc_out<bool>    ask_i;
    //my_fifo_in<int>                 input;
    my_fifo_out<sc_int<BUS_WIDTH> >  output;

    SC_HAS_PROCESS(bit_packing);

    bit_packing(sc_module_name name):
        sc_module(name) {
            SC_THREAD(process);
            sensitive << clk.pos();
        }

    void process();
};

#endif
