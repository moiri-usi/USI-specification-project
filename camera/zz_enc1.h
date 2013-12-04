/*  zz_enc.h */ 
#ifndef _ZZ_ENC1
#define _ZZ_ENC1
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(zz_enc1) {
  
	sc_in<int>		 	input;
	  sc_out<int>  output;
	  sc_in<bool>			ready_i;
	  sc_out<bool>			ask_i;
	  sc_out<bool>			ready_o;
	  sc_in<bool>			ask_o;
	  sc_in_clk				clk;

  SC_HAS_PROCESS(zz_enc1);

  zz_enc1(sc_module_name name):
	sc_module(name) {
		SC_THREAD(process);
		sensitive << clk.pos();
	}
  
  void process();
};



//===================================================================================


#endif
