#include "bit_packing.h"

void bit_packing::process() {
    sc_bv<VAL_WIDTH>    value;
    sc_bv<RL_WIDTH>     rl;
    sc_bv<BUS_WIDTH>    buf;

    int i_temp;
    int buf_idx_temp;

    if (reset.read()) {
        i = 0;
        buf_idx = 0;
        ask_i.write(false);
        ready_o.write(false);
        state.write(RESET);
    }
    else {
        switch (state) {
            case RESET:
                ask_i.write(true);
                ready_o.write(false);
                state = WAIT_READ_VAL;
                break;
            case WAIT_READ_VAL:
                if (ready_i.read()) {
                    value = static_cast<sc_bv<VAL_WIDTH> >(input.read());
                    ask_i.write(false);
                    state = READ_VAL;
                }
                break;
            case READ_VAL:
                ask_i.write(true);
                state = WAIT_READ_RL;
                break;
            case WAIT_READ_RL:
                if (ready_i.read()) {
                    rl = static_cast<sc_bv<RL_WIDTH> >(input.read());
                    ask_i.write(false);
                    state = READ_RL;
                }
                break;
            case WRITE_VAL:
                if (i < VAL_WIDTH) {
                    if (buf_idx < BUS_WIDTH) {
                        buf[buf_idx] = value[i];
                        buf_idx_temp = buf_idx;
                        buf_idx = buf_idx_temp + 1;
                        i_temp = i;
                        i = i_temp + 1;
                    }
                    else {
                        ready_o.write(false);
                        state = WAIT_WRITE_VAL;
                    }
                }
                else {
                    state = WRITE_RL;
                    i = 0;
                }
                break;
            case WAIT_WRITE_VAL:
                if (ask_o.read()) {
                    output.write(buf);
                    ready_o.write(true);
                    buf_idx = 0;
                    buf[buf_idx] = value[i];
                    buf_idx_temp = buf_idx;
                    buf_idx = buf_idx_temp + 1;
                    i_temp = i;
                    i = i_temp + 1;
                    state = WRITE_VAL;
                }
                break;
            case WRITE_RL:
                if (i < RL_WIDTH) {
                    if (buf_idx < BUS_WIDTH) {
                        buf[buf_idx] = rl[i];
                        buf_idx_temp = buf_idx;
                        buf_idx = buf_idx_temp + 1;
                        i_temp = i;
                        i = i_temp + 1;
                    }
                    else {
                        ready_o.write(false);
                        state = WAIT_WRITE_RL;
                    }
                }
                else {
                    state = CHECK_END;
                    i = 0;
                }
                break;
            case WAIT_WRITE_RL:
                if (ask_o.read()) {
                    output.write(buf);
                    ready_o.write(true);
                    buf_idx = 0;
                    buf[buf_idx] = rl[i];
                    buf_idx_temp = buf_idx;
                    buf_idx = buf_idx_temp + 1;
                    i_temp = i;
                    i = i_temp + 1;
                    state = WRITE_RL;
                }
                break;
            case CHECK_END:
                if (rl.to_int() < 63) {
                    ask_i.write(true);
                    state = READ_VAL;
                }
                else {
                    for (i_temp = buf_idx; i_temp < BUS_WIDTH; i_temp++) {
                        buf[i_temp] = 0;
                    }
                    state = WAIT_WRITE_END;
                }
                ready_o.write(false);
                break;
            case WAIT_WRITE_END:
                if (ask_o.read()) {
                    output.write(buf);
                    ready_o.write(true);
                    state = WRITE_END;
                }
                break;
            case WRITE_END:
                ask_i.write(true);
                ready_o.write(false);
                state=READ_VAL;
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
