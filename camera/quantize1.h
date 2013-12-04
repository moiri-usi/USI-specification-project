/*  quantize1.h */ 
#ifndef _QUANTIZE1
#define _QUANTIZE1 
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(quantize1) {

    //my_fifo_in<sc_fixed<23,13> >    input;
    sc_in<float>        input;
    sc_in<bool>         clk;
    sc_in<bool>         ready_i;
    sc_out<bool>        ready_o;
    sc_out<bool>        ask_i;
    sc_in<bool>         ask_o;
    //my_fifo_in<float>    input;
    //my_fifo_out<int>  output;
    sc_out<int>         output;

    int* quantization;

    SC_HAS_PROCESS(quantize1);

    quantize1(sc_module_name name, int* _quantization):
        sc_module(name),
        quantization(_quantization) {
            SC_THREAD(process);
            sensitive << clk.pos();
        }

    void process();
};

#endif
