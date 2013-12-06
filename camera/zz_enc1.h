//*  rl_enc.h */
#include <systemc.h>
#include "add2systemc.h"

SC_MODULE(zz_enc1) {


	// ports

	sc_out< sc_int<32> >	output;
	sc_in<bool>				ask_o;
	sc_out<bool>			ready_o;
	sc_in< sc_int<32> >		input;
	sc_in<bool>				ready_i;
	sc_out<bool>			ask_i;
	sc_in<bool>				clk;

	void process();


  SC_CTOR(zz_enc1){
			SC_THREAD(process);
			sensitive << clk.pos();
  }

};




