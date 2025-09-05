#ifndef HALIDE_QUANTIZE
#define HALIDE_QUANTIZE

#include "Halide.h"

using namespace Halide;
using namespace Halide::ConciseCasts;

class HalideQuantize : public Generator<HalideQuantize> {
    public:
    // TODO:
    // - Adicionar uma Func Float(32) de input e uma Func Float(32) de saída, ambas com duas dimensões
        // inputs:

            Input<Func> img_input{"input_qnt", Float(32), 2};
            Output<Func> img_output{"output_qnt", Float(32), 2};
    // - Adicionar dois GeneratorParamas: um int chamado levels com valor padrão '10' e um float chamado phi_q com valor padrão '0.25'
            GeneratorParam<int> levels{"levels",10};
            GeneratorParam<float> phi_q{"phi_q", 0.25f};
    // - Criar método generate que computa a quantiza descrita no protótipo no Colab [vide o README para o link]
            void generate() {
                float step = 100/levels; 
                Var x("x"),y("y");
                Expr q_n = floor(img_input(x,y)/step )*step ;
                Func q_nearest;
                q_nearest(x,y) = q_n;
                Func Q;
                Expr q = q_nearest(x,y) + (step*tanh(phi_q *(img_input(x,y) - q_nearest(x,y))));
                Q(x,y) = q ; 
                img_output(x,y) = q;
            }
    // - Criar método schedule que seta estimativas para os valores de tamanho das dimensões dos Buffers para uso do autoscheduler
            void schedule() {
                if (using_autoscheduler()) {
                    img_input.set_estimates({{0,1920}, {0, 1920}});
                    img_output.set_estimates({{0,1920}, {0, 1920}});
                    return; 
                }
            }
    
};

#endif

