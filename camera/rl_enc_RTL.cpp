#include "rl_enc_RTL.h"

void rl_enc_RTL::process_RTL(){
	
	sc_int<32> value2;
	sc_int<32> value3;
	
	if (reset.read() == true ){
		state.write(RESET);
		ready_o.write(false);
		ask_i.write(false);
	} else {
		switch (state.read()){
			
			
	case RESET: 
				ask_i.write(true);		//indicate that we want to read in next cycle
				ready_o.write(false);		//indicate that we do not write data in this cycle
				state.write(READZIGZAG);
				count.write(0);
				increment.write(0);
				break;
	
	case READZIGZAG:
		ready_o.write(false);
			
		if (ready_i.read()==true){		
			value.write(input.read());
			ask_i.write(false);
			state.write(WRITEDC);
		}
		else {
			ask_i.write(true);
			state.write(READZIGZAG);
		};
		break;
		
	case WRITEDC:
		if(ask_o.read()==true){
			output.write(value.read());
			count.write(0); //CHECK PLEASE
			ask_i.write(false);
			ready_o.write(true);
			state.write(READZEROS);
		}
		else{
			ready_o.write(false);
			ask_i.write(false);
			state.write(WRITEDC);
		}
		break;

//	case WAITFORZEROS:
	case READZEROS:
		ready_o.write(false);
		if(ready_i.read()== true){
			value.write(input.read());
			if (increment.read() == ((sc_int<32>)(63))){
				state.write(TERMINATION);
			}
			else{
				if(value.read()== (sc_int<32>)(0)){
					state.write(COUNTING);
				}
				else{
					state.write(WRITECOUNT);
				}
			}
		}
		break;
		
	case COUNTING:
		ready_o.write(false);
		ask_i.write(false);
		//ready_i.write(false); //PLEASE CHECK
		value2 = count.read();
		value2++;
		count.write(value2);
		value3 = increment.read();
		value3++;
		increment.write(value3);
		state.write(READZEROS);
		
		break;

	case WRITECOUNT:
		if (ask_o.read() == true){
			output.write(count.read());
			count.write(0);
			ready_o.write(true);
			ask_i.write(false);
			state.write(WRITEVALUE);
		}
		break;

	case WRITEVALUE:
		if(ask_o.read() == true){
			output.write(value.read());
			ready_o.write(true);
			ask_i.write(false);
			value3 = increment.read();
			value3++;
			increment.write(value3);
			state.write(READZEROS);
		}
		break;

	case TERMINATION:
		if(ask_o.read() == true){
			output.write(value.read());
			ready_o.write(true);
			ask_i.write(true);
			increment.write(0);
			state.write(READZIGZAG);
		}
		break;
					
			}
		}
	}
