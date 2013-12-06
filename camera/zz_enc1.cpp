#include "zz_enc1.h"

void zz_enc1::process_() {

	int zig_zag[64] = {0,1,8,16,9,2,3,10 ,
				    		17,24,32,25,18,11,4,5 ,
				    		12,19, 26, 33, 40, 48, 41, 34,
				    		27, 20, 13, 6, 7, 14, 21, 28,
				    		35, 42, 49, 56, 57, 50, 43, 36,
				    		29, 22, 15, 23, 30, 37, 44, 51,
				    		58, 59, 52, 45, 38, 31, 39, 46,
				    		53, 60, 61, 54, 47, 55, 62, 63};

	int     temp_block[64];

	sc_int<32> index;
	sc_int<32> i_temp;


	if (reset.read() == true ){
		state.write(RESET);
		ready_o.write(false);
		ask_i.write(false);
		cout << "reset signal low" << endl;
	} else {
		switch (state.read()){


	case RESET:
				cout<< "reset_state"<< endl;

				ask_i.write(true);		//indicate that we want to read in next cycle
				ready_o.write(false);		//indicate that we do not write data in this cycle
				state.write(READZIGZAG);
				i.write(0);
				break;

	case READZIGZAG:

		if (i.read() < ((sc_int<32>)(64))){
			ask_i.write(true);
			state.write(WAITREAD);
		}
		else{
			state.write(WRITEZIGZAG);
			i.write(0);
		}
		cout << "READZIGZAG: a_i:"<< ask_i << " r_i:"<< ready_i << " A_o:"<< ask_o << " R_o:"<< ready_o << endl;
		break;

	case WAITREAD:
		if (ready_i.read()==true){
			ready_o.write(false);
			i_temp = i.read();
			temp_block[i_temp] = input.read();
			ask_i.write(false);
			i_temp++;
			i.write(i_temp);
			state.write(READZIGZAG);

		}
		cout << "WAITREAD: a_i: "<< ask_i << " r_i:"<< ready_i << " A_o:"<< ask_o << " R_o:"<< ready_o << endl;
		break;

	case WRITEZIGZAG:
		if (i.read() < ((sc_int<32>)(64))){
			ready_o.write(false);
			state.write(WAITWRITE);
		}
		else{
			ready_o.write(false);
			i.write(0);
			ask_i.write(true);
			state.write(READZIGZAG);
		}
		cout << "WRITEZIGZAG: a_i: "<< ask_i << " r_i:"<< ready_i << " A_o:"<< ask_o << " R_o:"<< ready_o << endl;
		break;

	case WAITWRITE:
		if(ask_o.read()==true){
			output.write(temp_block[zig_zag[i.read()]]);
			ask_i.write(false); // check it out!!
			ready_o.write(true);//count to zeros
			i_temp = i.read();
			i_temp++;
			i.write(i_temp);
			state.write(WRITEZIGZAG);

		}
		cout << "WAITWRITE: a_i: "<< ask_i << " r_i:"<< ready_i << " A_o:"<< ask_o << " R_o:"<< ready_o << endl;
		break;


			}
		}
	}
































