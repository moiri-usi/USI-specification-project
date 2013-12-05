#include "bit_packing.h"

void bit_packing::process() {
    int i_temp;
    int buf_idx_temp;
    sc_bv<BUS_WIDTH> buf_temp;

    if (reset.read()) {
        i = 0;
        buf_idx = 0;
        ask_i.write(false);
        ready_o.write(false);
        state.write(RESET);
//        cout << "RESET:          " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
    }
    else {
        switch (state) {
            case RESET:
                ask_i.write(true);
                ready_o.write(false);
                state = WAIT_READ_VAL;
//                cout << "RESET:          " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
                break;
            case WAIT_READ_VAL:
                if (ready_i.read()) {
                    value = static_cast<sc_bv<VAL_WIDTH> >(input.read());
                    ask_i.write(false);
                    state = READ_VAL;
                }
//                cout << "WAIT_READ_VAL:  " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
                break;
            case READ_VAL:
                ask_i.write(true);
                state = WAIT_READ_RL;
//                cout << "READ_VAL:       " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read()  << " val:" << value << endl;
                break;
            case WAIT_READ_RL:
                if (ready_i.read()) {
                    rl = static_cast<sc_bv<RL_WIDTH> >(input.read());
                    ask_i.write(false);
                    state = PREPARE_VAL;
                }
//                cout << "WAIT_READ_RL:   " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
                break;
            case PREPARE_VAL:
                ready_o.write(false);
                if ((i < VAL_WIDTH) || (buf_idx >= BUS_WIDTH)) {
                    if (buf_idx < BUS_WIDTH) {
                        buf_temp = buf;
                        buf_temp[buf_idx] = value.read()[i];
                        buf = buf_temp;
                        buf_idx_temp = buf_idx;
                        buf_idx = buf_idx_temp + 1;
                        i_temp = i;
                        i = i_temp + 1;
                    }
                    else {
                        state = WAIT_WRITE_VAL;
                        buf_idx = 0;
                    }
                }
                else {
                    state = PREPARE_RL;
                    i = 0;
                }
//                cout << "PREPARE_VAL:    " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << " i:" << i << " buf_idx:" << buf_idx << " rl:" << rl << endl;
                break;
            case WAIT_WRITE_VAL:
                if (ask_o.read()) {
                    buf_temp = buf;
                    output.write(buf_temp);
                    ready_o.write(true);
                    //buf_temp[buf_idx] = value.read()[i];
                    //buf = buf_temp;
                    //buf_idx_temp = buf_idx;
                    //buf_idx = buf_idx_temp + 1;
                    //i_temp = i;
                    //i = i_temp + 1;
                    state = PREPARE_VAL;
                }
//                cout << "WAIT_WRITE_VAL: " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << " i:" << i << " buf_idx:" << buf_idx << " output:" << buf_temp << endl;
                cout << buf_temp << endl;
                break;
            case PREPARE_RL:
                ready_o.write(false);
                if ((i < RL_WIDTH) || (buf_idx >= BUS_WIDTH)) {
                    if (buf_idx < BUS_WIDTH) {
                        buf_temp = buf;
                        buf_temp[buf_idx] = rl.read()[i];
                        buf = buf_temp;
                        buf_idx_temp = buf_idx;
                        buf_idx = buf_idx_temp + 1;
                        i_temp = i;
                        i = i_temp + 1;
                    }
                    else {
                        state = WAIT_WRITE_RL;
                        buf_idx = 0;
                    }
                }
                else {
                    //state = CHECK_END;
                    state = WAIT_READ_VAL;
                    ask_i.write(true);
                    ready_o.write(false);
                    i = 0;
                }
//                cout << "PREPARE_RL:     " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << " i:" << i << " buf_idx:" << buf_idx << endl;
                break;
            case WAIT_WRITE_RL:
                if (ask_o.read()) {
                    buf_temp = buf;
                    output.write(buf_temp);
                    ready_o.write(true);
                    //buf_temp[buf_idx] = rl.read()[i];
                    //buf = buf_temp;
                    //buf_idx_temp = buf_idx;
                    //buf_idx = buf_idx_temp + 1;
                    //i_temp = i;
                    //i = i_temp + 1;
                    state = PREPARE_RL;
                }
//                cout << "WAIT_WRITE_RL:  " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << " i:" << i << " buf_idx:" << buf_idx << " output:" << buf_temp << endl;
                cout << buf_temp << endl;
                break;
            case CHECK_END:
                if (rl.read().to_int() < 63) {
                    ask_i.write(true);
                    state = WAIT_READ_VAL;
                }
                else {
                    buf_temp = buf;
                    buf_temp << BUS_WIDTH;
                    buf = buf_temp;
                    //for (i_temp = buf_idx; i_temp < BUS_WIDTH; i_temp++) {
                    //    buf_temp = buf;
                    //    buf_temp[i_temp] = 0;
                    //    buf = buf_temp;
                    //}
                    state = WAIT_WRITE_END;
                }
                ready_o.write(false);
//                cout << "CHECK_END:      " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
                break;
            case WAIT_WRITE_END:
                if (ask_o.read()) {
                    buf_temp = buf;
                    output.write(buf_temp);
                    ready_o.write(true);
                    state = WRITE_END;
                }
//                cout << "WAIT_WRITE_END: " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
                break;
            case WRITE_END:
                ask_i.write(true);
                ready_o.write(false);
                state = WAIT_READ_VAL;
                cout << "WRITE_END:      " << " ai:" << ask_i.read() << " ri:" << ready_i.read() << " ao:" << ask_o.read() << " ro:" << ready_o.read() << endl;
                cout << "EOB" << endl;
                break;
            default:
                ask_i.write(true);
                ready_o.write(false);
        }
    }

//    while (1) {
//        buf_idx = 0;
//        do {
//            ask_i.write(true);
//            ready_o.write(false);
//            wait();
//            // fill value bits into output bits
//            while (!ready_i.read()) wait();
//            value = static_cast<sc_bv<VAL_WIDTH> >(input.read());
//            ask_i.write(false);
//            wait();
//            ask_i.write(true);
//            wait();
//            while (!ready_i.read()) wait();
//            rl = static_cast<sc_bv<RL_WIDTH> >(input.read());
//            ask_i.write(false);
//            wait();
//            for (i = 0; i < VAL_WIDTH; i++) {
//                if (buf_idx < BUS_WIDTH) {
//                    buf[buf_idx] = value[i];
//                }
//                else {
//                    ready_o.write(false);
//                    wait();
//                    while (!ask_o.read()) wait();
//                    output.write(buf);
//                    ready_o.write(true);
//                    wait();
//                    buf_idx = 0;
//                    buf[buf_idx] = value[i];
//                }
//                buf_idx++;
//            }
//            // fill rl bits into output bits
//            for (i = 0; i < RL_WIDTH; i++) {
//                if (buf_idx < BUS_WIDTH) {
//                    buf[buf_idx] = rl[i];
//                }
//                else {
//                    ready_o.write(false);
//                    wait();
//                    while (!ask_o.read()) wait();
//                    output.write(buf);
//                    ready_o.write(true);
//                    wait();
//                    buf_idx = 0;
//                    buf[buf_idx] = rl[i];
//                }
//                buf_idx++;
//            }
//        }
//        while (rl.to_int() < 63);
//
//        for (i = buf_idx; i < BUS_WIDTH; i++) {
//            buf[i] = 0;
//        }
//        ready_o.write(false);
//        wait();
//        while (!ask_o.read()) wait();
//        output.write(buf);
//        ready_o.write(true);
//        wait();
//    }
}
