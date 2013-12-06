

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
	sc_in<bool>				reset;

// variables as signals
enum		ctrl_state {RESET, READZIGZAG, WAITREAD, WRITEZIGZAG, WAITWRITE};
sc_signal<ctrl_state>	state;//	ctrl_state	state;
sc_signal< sc_int<32> >	count;
sc_signal< sc_int<32> >	i;
	sc_signal< sc_int<32> >	value;
sc_signal< sc_int<32> >	out_counter;

	void process_();


  SC_CTOR(zz_enc1){
			SC_METHOD(process_);
			sensitive << clk.pos();
  }

};
