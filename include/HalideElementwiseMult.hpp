#ifndef HALIDE_ELEMENTWISE_MULT
#define HALIDE_ELEMENTWISE_MULT

#include "Halide.h"

using namespace Halide;
using namespace Halide::ConciseCasts;

class HalideElementwiseMult : public Generator<HalideElementwiseMult> {
    public:
    // TODO:
    // - Adicionar duas Funcs Float(32) de input e uma Func Float(32) de saída, todas com duas dimensões
            Input<Func> img_input1{"input1_mult", Float(32), 2};
            Input<Func> img_input2{"input2_mult", Float(32), 2};
            Output<Func> img_output{"output", Float(32), 2};
    // - Criar método generate que computa o produto entre os pixels das duas Funcs de entrada e salva na saída
            void generate() {
                Var x("x"), y("y");
                img_output(x,y) = img_input1(x,y) * img_input2(x,y);
            }
    // - Criar método schedule que seta estimativas para os valores de tamanho das dimensões dos Buffers para uso do autoscheduler
            void schedule() {
                if (using_autoscheduler()) {
                    img_input1.set_estimates({{0,1920}, {0, 1920}});
                    img_input2.set_estimates({{0,1920}, {0, 1920}});
                    img_output.set_estimates({{0,1920}, {0, 1920}});
                    return; 
                }
            }

};

#endif
