/*  jpeg_enc.h */
#ifndef _JPEG_ENC
#define _JPEG_ENC

#include        "r2b.h"
#include        "dct.h"
#include        "quantize1.h"
#include        "zz_enc.h"
#include        "rl_enc.h"
#include        "P2FF.h"
#include        "FF2P.h"

#define      block1   1
#define     block2    1
#define     block3    1

SC_MODULE(jpeg_enc) {

    /* in and output */
    my_fifo_in<int>   input;
    my_fifo_in<int>   parameters;
    my_fifo_out<int>  output;
    sc_in<bool>           clk;

    /* internal FIFOs */
    fifo_stat<int>        r2b_out;
    fifo_stat<float>      dct_out;
    fifo_stat<int>        quant_out;
    fifo_stat<int>        zz_enc_out;

    /* internal modules */
    r2b r2b_1;
    dct dct_1;
    quantize1 quant_1;
    zz_enc zz_enc_1;
    rl_enc rl_enc_1;

#if block1
    //P2FFC<sc_fixed<23,13>, float> p2ff_dct_quantize;
    //P2FF<float> p2ff_dct_quantize;
    FF2P<float> ff2p_dct_quantize;
    // fifo_stat<float>  fifo_dct_quantize;
    //fifo_stat<sc_fixed<23, 13> >    dct_quantize_out;
    fifo_stat<float>    dct_quantize_out;
    sc_signal<float>  float_dct_quantize;
    sc_signal<bool> bool_dct_quantize_ask;
    sc_signal<bool> bool_dct_quantize_ready;

#endif


#if block2
    P2FF<int> p2ff_quantize_zz;
    FF2P<int> ff2p_quantize_zz;
    fifo_stat<int>    quantize_zz_out;
    sc_signal<int>  int_quantize_zz;
    sc_signal<bool> bool_quantize_zz_ask;
    sc_signal<bool> bool_quantize_zz_ready;
#endif


#if block3
    //redo

    FF2P<int> ff2p_zz_rl_enc;
    P2FF<int> p2ff_zz_rl_enc;
    sc_signal<int> int_zz_rl_enc;
    sc_signal<bool> bool_zz_rl_enc_ask;
    sc_signal<bool> bool_zz_rl_enc_ready;
    fifo_stat<int> zz_rl_enc_out;

#endif
    jpeg_enc(sc_module_name _name, int* _quantization, int _maxwidth):
        sc_module(_name),
        r2b_out("r2b_out",1),
        dct_out("dct_out",1),
        quant_out("quant_out",1),
        zz_enc_out("zz_enc_out",1),
        r2b_1("r2b_1",_maxwidth),
        dct_1("dct_1"),
        quant_1("quant_1",_quantization),
        zz_enc_1("zz_enc_1"),
        rl_enc_1("rl_enc_1")
#if block1
        ,//p2ff_dct_quantize("p2ff_dct_quantize_"),
        ff2p_dct_quantize("ff2p_dct_quantize_"),
        dct_quantize_out("dct_quantize_out",1)
#endif
#if block2
            ,p2ff_quantize_zz("p2ff_quantize_zz"),
        ff2p_quantize_zz("ff2p_quantize_zz"),
        quantize_zz_out("quantize_zz_out",1)
#endif

#if block3
            ,ff2p_zz_rl_enc("ff2p_zz_rl_enc"),
        p2ff_zz_rl_enc("p2ff_zz_rl_enc"),
        zz_rl_enc_out("zz_rl_enc_out",1)
#endif
        {

            r2b_1.input(input);
            r2b_1.parameters(parameters);
            r2b_1.output(r2b_out);

            dct_1.input(r2b_out);
            dct_1.output(dct_out);

#if block1
            ff2p_dct_quantize.clk(clk);
            ff2p_dct_quantize.input(dct_out);
            ff2p_dct_quantize.output(float_dct_quantize);
            ff2p_dct_quantize.ask(bool_dct_quantize_ask);
            ff2p_dct_quantize.ready(bool_dct_quantize_ready);

     //       p2ff_dct_quantize.clk(clk);
     //       p2ff_dct_quantize.input(float_dct_quantize);
     //       p2ff_dct_quantize.ask(bool_dct_quantize_ask);
     //       p2ff_dct_quantize.ready(bool_dct_quantize_ready);
     //       p2ff_dct_quantize.output(dct_quantize_out);
     //       quant_1.input(dct_quantize_out);
            quant_1.input(float_dct_quantize);
            quant_1.clk(clk);
            quant_1.ready_i(bool_dct_quantize_ready);
            quant_1.ask_i(bool_dct_quantize_ask);
#endif


#if !block1
            quant_1.input(dct_out);
#endif

            quant_1.output(quant_out);
#if block2
            ff2p_quantize_zz.input(quant_out);
            ff2p_quantize_zz.clk(clk);
            ff2p_quantize_zz.output(int_quantize_zz);
            ff2p_quantize_zz.ask(bool_quantize_zz_ask);
            ff2p_quantize_zz.ready(bool_quantize_zz_ready);


            p2ff_quantize_zz.input(int_quantize_zz);
            p2ff_quantize_zz.clk(clk);
            p2ff_quantize_zz.output(quantize_zz_out);
            p2ff_quantize_zz.ask(bool_quantize_zz_ask);
            p2ff_quantize_zz.ready(bool_quantize_zz_ready);

            zz_enc_1.input(quantize_zz_out);
#endif

#if !block2
            zz_enc_1.input(quant_out);
#endif

            zz_enc_1.output(zz_enc_out);

#if block3

            ff2p_zz_rl_enc.input(zz_enc_out);
            ff2p_zz_rl_enc.clk(clk);
            ff2p_zz_rl_enc.ask(bool_zz_rl_enc_ask);
            ff2p_zz_rl_enc.ready(bool_zz_rl_enc_ready);

            ff2p_zz_rl_enc.output(int_zz_rl_enc);

            p2ff_zz_rl_enc.input(int_zz_rl_enc);
            p2ff_zz_rl_enc.clk(clk);
            p2ff_zz_rl_enc.ask(bool_zz_rl_enc_ask);
            p2ff_zz_rl_enc.ready(bool_zz_rl_enc_ready);

            p2ff_zz_rl_enc.output(zz_rl_enc_out);

            rl_enc_1.input(zz_rl_enc_out);

#endif
#if !block3
            rl_enc_1.input(zz_enc_out);
#endif
            rl_enc_1.output(output);

        }

    ~jpeg_enc() {
    }

};

//=======================================================================================

SC_MODULE(jpeg_enc_pr) {

    // port
    my_fifo_in<int>   input;
    my_fifo_in<int>   parameters;
    sc_out<sc_int<32> >           output;
    sc_in<bool>           ask;
    sc_out<bool>          ready;
    sc_in<bool>           clk;

    // internal FIFO
    sc_fifo<int>          temp;

    // submodules
    jpeg_enc jpeg_enc_1;
    FF2PC<int,sc_int<32> > fifo2prot;

    // constructor
    jpeg_enc_pr(sc_module_name _name, int* _quantization, int _maxwidth):
        sc_module(_name),
        input("jpeg_enc_pr_input"),
        jpeg_enc_1("jpeg_enc_1", _quantization, _maxwidth),
        fifo2prot("fifo2prot1") {

            jpeg_enc_1.input(input);
            jpeg_enc_1.parameters(parameters);
            jpeg_enc_1.output(temp);
            jpeg_enc_1.clk(clk);

            fifo2prot.input(temp);
            fifo2prot.ask(ask);
            fifo2prot.ready(ready);
            fifo2prot.output(output);
            fifo2prot.clk(clk);
        }

    // destructor
    ~jpeg_enc_pr() {
    }

};

#endif

