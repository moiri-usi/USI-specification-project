/*  rl_enc1.h */ 
#ifndef _RL_ENC1
#define _RL_ENC1
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(rl_enc1) {

    //my_fifo_in<int>   input;
    sc_in<int>          input;
    sc_in<bool>         ready_i;
    sc_in<bool>         clk;
    sc_out<bool>        ask_i;
    my_fifo_out<int>  output;

    SC_HAS_PROCESS(rl_enc1);

    rl_enc1(sc_module_name name):
        sc_module(name) {
            SC_THREAD(process);
            sensitive << clk.pos();
        }

    void process();
};

#endif
