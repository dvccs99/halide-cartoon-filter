#ifndef HALIDE_XDOG_HPP
#define HALIDE_XDOG_HPP

#include "Halide.h"
#include "gaussian.hpp"

using namespace Halide;
using namespace Halide::ConciseCasts;

class HalideXDoG : public Generator<HalideXDoG> {
    // TODO:
    // - Adicionar uma Func Float(32) de input e uma Func Float(32) de saída, ambas com duas dimensões
    public:
            Input<Func> img_input{"input_xdog", Float(32), 2};
            Output<Func> img_output{"output_xdog", Float(32), 2};


    // - Adicionar quatro GeneratorParamas (todos são do tipo float):
    //    - sigma com valor padrão '0.5'
    //    - k com valor padrão '7.0'
    //    - gamma com valor padrão '0.95'
    //    - phi_e com valor padrão '5.0'

            GeneratorParam<float> sigma{"sigma",0.5f};
            GeneratorParam<float> k{"k", 7.0f};
            GeneratorParam<float> gamma{"gamma",0.95f};
            GeneratorParam<float> phi_e{"phi_e", 5.0f};

    // - Criar método generate que computa a XDoG descrita no protótipo no Colab [vide o README para o link]
        void generate() {
            
            Expr lk_width = get_kernel_width(sigma.value());
            Expr hk_width = get_kernel_width(k.value() * sigma.value());

            Expr low_sigma = sigma;
            Expr high_sigma = k * low_sigma;
            weights_low(i) = gaussian(i, low_sigma);
            weights_high(i) = gaussian(i, high_sigma);

            RDom rd_low(-(lk_width / 2), lk_width, -(lk_width / 2), lk_width);
            RDom rd_high(-(hk_width / 2), hk_width, -(hk_width / 2), hk_width);

            Expr pixel_low = img_input(x + rd_low.x, y + rd_low.y);
            Expr sum_low = sum(weights_low(rd_low.x) * weights_low(rd_low.y), "sum_low");
            filtered_low(x, y) = sum(
                pixel_low * weights_low(rd_low.x) * weights_low(rd_low.y), "sum_conv_low"
            ) / sum_low;
            

            Expr pixel_high = img_input(x + rd_high.x, y + rd_high.y);
            Expr sum_high = sum(weights_high(rd_high.x) * weights_high(rd_high.y), "sum_high");
            filtered_high(x, y) = sum(
                pixel_high * weights_high(rd_high.x) * weights_high(rd_high.y), "sum_conv_high"
            ) / sum_high;

            img_output(x, y) = f32(
                select(
                    (filtered_low(x, y) - gamma * filtered_high(x, y)) > 0.0f, 
                    1, 
                    1+tanh(phi_e * (filtered_low(x, y) - gamma * filtered_high(x, y)))
                )
            );
        }

    //    *  Use o exemplo de DoG da semana 04 como base. Não é necessário criar um generator separado para o DoG puro, apenas adiciona ao exemplo se usar
    // - Criar método schedule que seta estimativas para os valores de tamanho das dimensões dos Buffers para uso do autoscheduler
            void schedule() {
                if (using_autoscheduler()) {
                    img_input.set_estimates({{0,1920}, {0, 1920}});
                    img_output.set_estimates({{0,1920}, {0, 1920}});
                    return; 
                }
            }
    // Dica: não é necessário explicitar padding aqui pois a Func que deve ser entrada já está coberta por uma BoundaryCondition
    private:
    Var x{"x"}, y{"y"}, c{"c"}, i{"i"};
    Func weights_low{"weights_low"}, weights_high{"weights_high"};
    Func filtered_low{"filtered_low"}, filtered_high{"filtered_high"};
};
#endif
