#ifndef MOUSE_MODULE_COLENDA
#define MOUSE_MODULE_COLENDA


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

/**
 * \brief função responsavel por realizar a leitura e retornar os valor do mouse 1
 * \param[out] key inteiro referente a tecla pressionada
 * \param[out] coord_x inteiro referente ao valor traduzido da aceleração do mouse
 * \return 1 caso sucesso, 0 em caso de erro
 */
int read_mouse_1_event(int* key, int* coord_x);

/**
 * \brief função responsavel por realizar a leitura e retornar os valor do mouse 2
 * \param[out] key inteiro referente a tecla pressionada
 * \param[out] coord_x inteiro referente ao valor traduzido da aceleração do mouse
 * \return 1 caso sucesso, 0 em caso de erro
 */
int read_mouse_2_event(int* key, int* coord_x);

/**
 * \brief função responsavel por iniciar a comunicação com o mouse 1
 * \return 1 caso sucesso, 0 em caso de erro
 */
int module_init_mouse_1();

/**
 * \brief função responsavel por encerrar a comunicação com o mouse 1
 * \return 1 caso sucesso, 0 em caso de erro
 */
int module_exit_mouse_1();

/**
 * \brief função responsavel por iniciar a comunicação com o mouse 2
 * \return 1 caso sucesso, 0 em caso de erro
 */
int module_init_mouse_2();

/**
 * \brief função responsavel por encerrar a comunicação com o mouse 2
 * \return 1 caso sucesso, 0 em caso de erro
 */
int module_exit_mouse_2();

#endif