#ifndef BACKGROUND_ANIMATION_MODULE
#define BACKGROUND_ANIMATION_MODULE

#include <unistd.h>
#include <stdio.h>
#include "../Lib/Colenda_Lib/colenda.h"

/**
 * \brief função responsavel por iniciar o modulo da animação do background
 * \return retorna 1 caso sucesso e 0 em caso de falha
 */
int bg_animation_module_init();

/**
 * \brief função responsavel por atualizar a imagem atual do fundo
 * \return não há retorno
 */
void bg_animation();

#endif