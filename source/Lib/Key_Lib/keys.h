/**
 * \file            keys.h
 * \brief           Arquivo de cabeçalho da biblioteca keys
 */

/*
 * Copyright (c) 2024 Brenda Babosa, Camila queiroz, Maike Nascimento
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of Super Auto.
 *
 * Authors:          Brenda Barbosa, Camila Queiroz, Maike e Oliveira
 */

#ifndef KEYS

#define KEYS

/* Caminho do driver dos botões */
#define KEYS_DRIVER_PATH "/dev/key_driver"

/* Indentificadores dos botões*/
#define BUTTON0 '0'
#define BUTTON1 '1'
#define BUTTON2 '2'
#define BUTTON3 '3'

/**
 * \brief           Inicia a comunicação com o driver dos botões.
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna -1
*/
int
KEYS_open();

/**
 * \brief           Realia a leitura dos botões 
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna -1
*/

int
KEYS_read(char* button);

/**
 * \brief           Encerra a comunicação com o driver dos botões.
 * \return          Retorna 0 caso a operação seja bem sucedida. Caso contrário, retorna -1
*/
int
KEYS_close();



#endif
