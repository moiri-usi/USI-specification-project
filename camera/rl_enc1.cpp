#include "rl_enc1.h"

void rl_enc1::process(){

    sc_int<32> value2;
    sc_int<32> value3;

    if (reset.read() == true ){
        state.write(RESET);
        ready_o.write(false);
        ask_i.write(false);
    } else {
        switch (state.read()){


            case RESET: 
                ask_i.write(true);      //indicate that we want to read in next cycle
                ready_o.write(false);       //indicate that we do not write data in this cycle
                state.write(READZIGZAG);
                count.write(0);
                increment.write(0);
                break;

            case READZIGZAG:

                ready_o.write(false);
                increment.write(1);

                if (ready_i.read()==true){
                    value.write(input.read());
                    ask_i.write(false);
                    state.write(WRITEDC);
                }
                else {
                    ask_i.write(true);//fine!!!
                    state.write(READZIGZAG);
                }
                break;

            case WRITEDC:
                //cout<< "writedc_state"<< endl;
                //cout<< "writedc_state"<< endl;
                if(ask_o.read()==true){
                    output.write(value.read());

                    ask_i.write(false); // check it out!!

                    ready_o.write(true);//count to zeros
                    count.write(0);
                    state.write(WAITFORZEROS);
                }
                else{
                    ready_o.write(false);//
                    ask_i.write(false);///
                    state.write(WRITEDC);
                }
                break;

                //  case WAITFORZEROS:

            case WAITFORZEROS: //wait for succesfull write AC/DC value

                ready_o.write(false);
                if (increment.read() < ((sc_int<32>)(64))){
                    ask_i.write(true);
                    value3 = increment.read();
                    value3++;
                    increment.write(value3);

                    state.write(READZEROS);
                }
                else{

                    state.write(TERMINATION);
                }
                break;


            case READZEROS:

                if(ready_i.read()== true){ // ojo after

                    value.write(input.read());
                    ask_i.write(false);
                    state.write(COUNTING);
                }
                else{
                    state.write(READZEROS);
                }

                break;

            case COUNTING:

                if (value.read()== (sc_int<32>)(0)){
                    value2 = count.read();
                    value2++;
                    count.write(value2);
                    state.write(WAITFORZEROS);
                }
                else{
                    state.write(WRITECOUNT);
                }



                break;

            case WRITECOUNT:

                ready_o.write(false);
                ask_i.write(false);

                if (ask_o.read() == true){
                    output.write(count.read());
                    ready_o.write(true);
                    state.write(WAITTOWRITE);
                }
                else{
                    state.write(WRITECOUNT);
                }

                break;

            case WAITTOWRITE:

                ready_o.write(false);
                ask_i.write(false);
                state.write(WRITEDC);
                break;


            case TERMINATION:

                if(ask_o.read() == true){
                    output.write((sc_int<32>)(63));
                    ready_o.write(true);
                    ask_i.write(true);
                    state.write(READZIGZAG);
                }
                else{
                    state.write(TERMINATION);
                }
                break;

        }
    }
}
