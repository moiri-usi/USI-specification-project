#include "bit_packing.h"

void bit_packing::process() {
    sc_bv<VAL_WIDTH>    value;
    sc_bv<RL_WIDTH>     rl;
    sc_bv<BUS_WIDTH>    buf;
    int     i;
    int     buf_idx;

    while (1) {
        buf_idx = 0;
        do {
            // fill value bits into output bits
            value = static_cast<sc_bv<VAL_WIDTH> >(input.read());
            for (i = 0; i < VAL_WIDTH; i++) {
                if (buf_idx < BUS_WIDTH) {
                    buf[buf_idx] = value[i];
                }
                else {
                    if (buf_idx == BUS_WIDTH) {
                        output.write(buf);
                        buf_idx = 0;
                    }
                    buf[buf_idx] = value[i];
                }
                buf_idx++;
            }
            rl = static_cast<sc_bv<RL_WIDTH> >(input.read());
            // fill rl bits into output bits
            for (i = 0; i < RL_WIDTH; i++) {
                if (buf_idx < BUS_WIDTH) {
                    buf[buf_idx] = rl[i];
                }
                else {
                    if (buf_idx == BUS_WIDTH) {
                        output.write(buf);
                        buf_idx = 0;
                    }
                    buf[buf_idx] = rl[i];
                }
                buf_idx++;
            }
        }
        while (rl.to_int() < 63);

        for (i = buf_idx; i < BUS_WIDTH; i++) {
            buf[i] = 0;
        }
        output.write(buf);
    }
}
