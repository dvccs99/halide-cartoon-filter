# Filtro de cartunização simplificado
Filtro para abstração de imagens, feito em [Halide](https://halide-lang.org/). Uma versão simplificada do filtro descrito [neste paper](https://dl.acm.org/doi/abs/10.1145/1141911.1142018).

Esse código foi produzido como projeto final da disciplina da DSL Halide no curso de fotografia computacional com ênfase em dispositivos móveis e inteligência artificial do CIn/UFPE ([link](https://luxai.cin.ufpe.br/cursos.html)).

## Requisitos
O Halide utilizado foi o [v16.0.0](https://github.com/halide/Halide/releases/tag/v16.0.0). Para executar o código, é necessário que:
- o diretório do Halide esteja acessível através da variável de linha de comando HALIDE_ROOT.
- A pasta ```$HALIDE_ROOT/share/Halide/tools``` seja movida para 
```$HALIDE_ROOT```
- Haja uma ```venv``` python com as dependências corretas.

Para criar o ambiente python correto, tenha instalado o [uv](https://docs.astral.sh/uv/)

Crie um ambiente virtual usando ```uv venv``` e depois use ```uv pip sync uv.lock```.

## Executando o código

Use então:
```
make test
```
Para executar o código. Além de compilar o Halide, ele também utilizará o pytest para avaliar o resultado contra imagens de referência disponibilizadas durante o  curso.

## Estrutura do filtro

![image](/docs/filter.png)
