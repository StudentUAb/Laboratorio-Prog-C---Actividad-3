// shell.c
/*
UC: 21178 - Laboratorio de Programacao
Ano 2021/22 - EFOLIO B - Mundo dos rectangulos - UAb
**
** Aluno: 2100927 - Ivo Baptista 
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include "interface.h"
#include "screen.h"



const char *txt_comandos[] = {
  "help", // para impressao na nav
  "exit",
  "create",
  "moveright",
  "moveleft",
  "colide",
  "gravidade",
  "load",
  "clear",
};


const char *txt_help =  "COMANDOS QUE PODE EXECUTAR:\n"
"help - mostra esta mensagem\n" \
"exit - termina a aplicação\n" \
"create xx,xx+xx,xx - adiciona um rectangulo\n" \
"moveright xx,xx+xx - move para a direita\n" \
"moveleft xx,xx+xx - move para a esquerda\n" \
"colide - colide na mesma zona\n" \
"gravidade - em testes\n" \
"clear - limpa tela\n" \
"load - carrega dados exemplo";

int _help(){
  fprintf(stdout, "%s", txt_help);
  return 0;
}

int _clear(){
  clrscr();
  return 0;
}

void _tela(){

  int tam_buffer=0;
  int tam_cmd=0;
  int tam_txt_comandos = TAM_ARRAY(txt_comandos);
  int (*opcoes[tam_txt_comandos])();
  char* cmd;

  opcoes[0] = _help;
  opcoes[1] = sair_i;
  opcoes[2] = _create;
  opcoes[3] = _moveright;
  opcoes[4] = _moveleft;
  opcoes[5] = _colide;
  opcoes[6] = _gravidade;
  opcoes[7] = _load;
  opcoes[8] = _clear;
  

  fprintf(stdout,"                                                                                 \n");
  fprintf(stdout,"---------------------------------------------------------------------------------\n");
  fprintf(stdout,"12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
  fprintf(stdout,"         1         2         3         4         5         6         7         8\n\n");
  fprintf(stdout,"\nDigite o comando help para ver as opções validas\n" );
  fprintf(stdout, "-->");

  fgets(g.buffer,MAXSTR,stdin);
  tam_buffer = strlen(g.buffer);
  if (tam_buffer > 2){
    g.buffer[tam_buffer-1]=0;
    cmd = strtok(g.buffer," ");
    tam_cmd = strlen(cmd);
    if(tam_buffer - tam_cmd > 2){
        strcpy(g.param, strtok(0, "\n"));
    }
    if (tam_cmd > 1){
      for(int i=0;i < tam_txt_comandos;i++){
        if(!strcmp(cmd, txt_comandos[i])){
          opcoes[i]();
          break;
        }
      }
      if(i == tam_txt_comandos){
        fprintf(stdout, "Erro tenta de novo \n");
      }
    }
  }
}

int _load(){
  carregar_ficheiro();
  gravidade();
  print_all();
  return 0;
}



int _gravidade(){
  gravidade();
  return 0;
}

int _colide(){
  if (get_param(2)){
 fprintf(stdout, "Erro tenta de novo \n");
    return 1;
  }
  if(rect_intersect(g._u[g._param1],g._u[g._param2]) ){
    fprintf(stderr, "A colide com B\n");
  } else {
    fprintf(stderr, "A não colide com B\n");
  }
  return 0;
}


int _create(){
  if (get_param(4)){
    fprintf(stdout, "Erro tenta de novo \n");
    return 1;
  }
  if(create(g._param1, g._param2, g._param3, g._param4 )){
  fprintf(stdout, "Erro tenta de novo \n");
    return 1;
  }
  gravidade();
  print_all();
  return 0;
}

int _moveright(){
  int pos=-1;
  if (get_param(3)){
  fprintf(stdout, "Erro tenta de novo \n");
  }
  pos = find_pos_rect(g._param1, g._param2);
  if( pos > -1){
    g._u[pos]->_x += g._param3;
    if ( (limitex(g._u[pos]->_x, g._u[pos]->_x+g._u[pos]->_c-1)) || (colisao(g._u[pos])) ){
      fprintf(stdout, "Colisao. \n Não é possível realizar o movimento \n Existe um obstaculo.\n");
      g._u[pos]->_x -= g._param3;
      return 1;
    }
    gravidade();
    print_all();
  } else {
    fprintf(stdout, "Não existe rectangulo com essa possição\n");
    return 1;
  }
  return 0;
}

int _moveleft(){
  int pos=-1;
  if (get_param(3)){
 fprintf(stdout, "Erro tenta de novo \n");
  }
  pos = find_pos_rect(g._param1, g._param2);
  if( pos > -1){
    g._u[pos]->_x -= g._param3;
    if ( (limitex(g._u[pos]->_x, g._u[pos]->_x+g._u[pos]->_c-1)) || (colisao(g._u[pos])) ){
      fprintf(stdout, "Colisao. \n Não é possível realizar o movimento \n Existe um obstaculo.\n");
      g._u[pos]->_x += g._param3;
      return 1;
    }
    gravidade();
    print_all();
  } else {
    fprintf(stdout, "Não existe rectangulo com essa pos\n");
    return 1;
  }
  return 0;
}

int get_param(int nr_params){
  int tam = 0;
  int delim = 0;
  if(nr_params == 1){
    if (!(sscanf (g.param,"%hu",&g._param1) == 1) ){
      fprintf(stdout, "Erro: É esperado 1 parametro no formato xx\n" );
      return 1; // erro
    }
  }
  if (strlen(g.param) > 2){
    tam = strlen(g.param);
    for(int i=0; i < tam; i++){
      if (g.param[i] == ',' || g.param[i] == '+' ){
        delim++;
      }
    }
    if(nr_params == 2){
      if(delim == 1){
        if (!(sscanf (g.param,"%hu,%hu",&g._param1, &g._param2) == 2) ){
          fprintf(stdout, "Erro: Sao esperados 2 parametros no formato xx,xx\n" );
          return 1; // erro
        }
      } else {
        fprintf(stdout, "Erro: Sao esperados 2 parametros no formato xx,xx\n" );
        return 1; // erro
      }
    }
    if(nr_params == 3){
      if(delim == 2){
        if (!(sscanf (g.param,"%hu,%hu+%hu",&g._param1, &g._param2, &g._param3) == 3) ){
          fprintf(stdout, "Erro: Sao esperados 3 parametros no formato xx,xx+xx\n" );
          return 1; // erro
        }
      } else {
        fprintf(stdout, "Erro: Sao esperados 3 parametros no formato xx,xx+xx\n" );
        return 1; // erro
      }
    }
    if(nr_params == 4){
      if(delim == 3){
        if (!(sscanf (g.param,"%hu,%hu+%hu,%hu",&g._param1, &g._param2, &g._param3, &g._param4) == 4) ){
          fprintf(stdout, "Erro: Sao esperados 4 parametros no formato xx,xx+xx,xx\n" );
          return 1; // erro
        }
      } else {
        fprintf(stdout, "Erro: Sao esperados 4 parametros no formato xx,xx+xx,xx\n" );
        return 1; // erro
      }
    }
  }
  return 0; // ok
  }
