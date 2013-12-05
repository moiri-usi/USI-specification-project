/*  quantize1.h */ 
#ifndef _QUANTIZE1
#define _QUANTIZE1 
#define SC_INCLUDE_FX
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(quantize1) {

    sc_in<sc_fixed<23, 13, SC_RND, SC_SAT> > input;
    sc_in<bool>         clk;
    sc_in<bool>         ready_i;
    sc_out<bool>        ready_o;
    sc_out<bool>        ask_i;
    sc_in<bool>         ask_o;
    sc_in<bool>         reset;
    sc_out<int>         output;
    //sc_out<sc_int<8> >  output;

    int* quantization;
    enum ctrl_state {WAITREAD, READ, WAITWRITE, CHECKLOOP, TERMINATION};
    sc_signal<ctrl_state> state;
    sc_signal<int> i;
    sc_signal<int> j;
    sc_signal<int> temp_out;

    SC_HAS_PROCESS(quantize1);

    quantize1(sc_module_name name, int* _quantization):
        sc_module(name),
        quantization(_quantization) {
            SC_METHOD(process);
            sensitive << clk.pos();
        }

    void process();
};

#endif
