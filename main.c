//main.c
/*
UC: 21178 - Laboratorio de Programacao
Ano 2021/22 - EFOLIO B - Mundo dos rectangulos - UAb
**
** Aluno: 2100927 - Ivo Baptista 
*/

#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<signal.h>
#include "interface.h"
#include "screen.h"

int main() {
  size_t n = 11;
  rectang* u[n];
  rectang* r[n];
  char* mundo[2025] = {0};
  memset(u,0,n*sizeof(struct rectang));
  memset(r,0,n*sizeof(struct rectang));

  g._u = u;
  g._r = r;
  g.mundo = (char**)mundo;
  //g.debug = 1;

  set_ini_interface();

  char ch = c_getch();
  if(ch == 'r' || ch == 'R'){
    g.print_buffer = 1;
  }
  while(1){
    _tela();
  }

  return 0;
}
