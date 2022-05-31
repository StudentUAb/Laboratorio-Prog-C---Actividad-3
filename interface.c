// interface.c
/*
UC: 21178 - Laboratorio de Programacao
Ano 2021/22 - EFOLIO B - Mundo dos rectangulos - UAb
**
** Aluno: 2100927 - Ivo Baptista 
*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<time.h>
#include<sys/ioctl.h>
#include<assert.h>
#include "interface.h"
#include "screen.h"

int rect_intersect(rectang* a, rectang* b){
  int res1 = 0;
  int res2 = 0;
  res1 = intersect_interval(a->_x, a->_x+a->_c-1, b->_x, b->_x + b->_c-1);
  res2 = intersect_interval(a->_y, a->_y+a->_a-1, b->_y, b->_y + b->_a-1);
  return res1 && res2;
}

int intersect_interval(short min0, short max0, short min1, short max1){
  double res1=0;
  double res2=0;
  res1 = fmax(min0, max0) > fmin(min1, max1);
  res2 = fmin(min0, max0) < fmax(min1, max1);
  return res1 && res2;
}

int limitex(short x0, short x1){
  short res = 0;
  if(g.print_buffer && g.rotacao ){
    res = !(x0 > 0 && x1 < 26); // return 1 é erro
  } else {
    res = !(x0 > 0 && x1 < 81); // return 1 é erro
  }
   return res;
}

int limitey(short y0, short y1){
  short res = 0;
  if(g.print_buffer && g.rotacao ){
    res = !(y0 > 0 && y1 < 81); // return 1 é erro
  } else {
    res = !(y0 > 0 && y1 < 26); // return 1 é erro
  }
   return res;
}

int rect_descendente(const void* a, const void* b){
  short res=0;
    rectang* _a = *(rectang**)a;
    rectang* _b = *(rectang**)b;
    if ( (_a->_y < _b->_y) && (_a->_x > _b->_x) ) res =  -1;
    if ( (_a->_y == _b->_y) && (_a->_x > _b->_x) ) res = -1;
    if ( (_a->_y == _b->_y) && (_a->_x < _b->_x) ) res =  1;
    if ( (_a->_y < _b->_y) && (_a->_x < _b->_x) ) res =  -1;
    if ( (_a->_y > _b->_y) ) res =   1;
    if ( (_a->_y < _b->_y) ) res =  -1;
    return res;
}

void order_rect_desc(){
  qsort (g._u, g._tam_uarray, sizeof(struct rectang*), rect_descendente);
}

int colisao(rectang* a){
  for (size_t j = 0; j < g._tam_uarray; j++) {
    if( a != g._u[j] ){
      if(rect_intersect(a, g._u[j])  ){
        return 1; // erro
      }
    }
  }
  return 0; // ok
}

int gravidade(){
  int an,ta;
  for (size_t i = 0; i < g._tam_uarray; i++) {
    while (1) {
      g._u[i]->_y--;
      an++;
      ta=an*0.1;
      if( (limitey(g._u[i]->_y, g._u[i]->_y+g._u[i]->_a-1)) || (colisao(g._u[i])) ){
        g._u[i]->_y++;
        --an;   //    an=an-1     altura nova
        rectanew(an, g._u[i]);       
        break;
      }
    }
  }
  return 0;
}

void rectanew(int an, rectang* a){
  
  //create(short x, short y, short l, short h);
  // create(a._param1, a._param2, a._param3, a._param4);
  //create(a._param1, a._param2, a._param3, an);
}



// int rot_rect(rectang* a){
//   short go = 1;
//   unsigned short tmp = a->_x;
//   a->_x = a->_y;
//   a->_y = 26 - tmp;
//   // swap dimensões
//   tmp = a->_a;
//   a->_a = a->_c;
//   a->_c = tmp;

//   while (go){
//     if(go > 25){
//       a->_x++;
//     }
//     if( limitey(a->_y, a->_y+a->_a+1) && (!colisao(a))  ){
//       a->_y--;
//     }
//     if( colisao(a) && (!limitey(a->_y, a->_y+a->_a+1)) ){
//        a->_y++;
//     }
//     if( (!colisao(a)) && (!limitey(a->_y, a->_y+a->_a+1)) ){
//       go = 0;
//     } else {
//       go++;
//     }
//   }
//   return 0;
// }

void sair(){exit(EXIT_SUCCESS);}
int sair_i(){exit(EXIT_SUCCESS);}
void func_stub(){  printf("%s\n", "Func stub");}

void handle_resize(int sig){
signal(SIGWINCH, SIG_IGN);
get_term_size();
signal(SIGWINCH, handle_resize);
}

void get_term_size(){
  struct winsize w; // lin x col
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  g.lin = w.ws_row;
  g.col = w.ws_col;
  printxy(20, 39,"Dimensão permitida em colunas x linhas  %d X  %d", w.ws_col, w.ws_row);
}

void set_ini_interface(){
  clrscr();
  get_term_size();
  print_in_middle(0, 4, g.col,"UC: 21178 - Laboratorio de Programacao");
  print_in_middle(0, 5, g.col,"EFOLIO B - 18/05/2020");
  print_in_middle(0, 6, g.col,"-   Ivo Baptista  -  Aluno: 2100927   -");
  print_in_middle(0, g.lin/2+3, g.col, "Presiona uma tecla para entrar:");
  print_in_middle(0, g.lin/2+4, g.col, "--< No Mundo dos Rectangulos >--\n");
  printxy(20, 20, "%s", " ");
  print_box_header();
  gotoxy(0,23);
}

int create(short x, short y, short l, short h){
rectang* item = 0;

if(l < MINLARGURA){
  fprintf(stdout, "Erro: Largura min é 3. Não é possível realizar a operação.\n");
  return 1;
}
if(h < MINALTURA){
  fprintf(stdout, "Erro: Altura min é 3. Não é possível realizar a operação.\n");
  return 1;
}

if (limitex(x,x+l)){
  fprintf(stdout, "Erro: Fora dos limites em x. Não é possível realizar a operação.\n");
  return 1;
}
if (limitey(y, y+h)){
  fprintf(stdout, "Erro: Fora dos limites em y. Não é possível realizar a operação.\n");
  return 1;
}

if ( (item = (rectang*)calloc(1, sizeof(struct rectang))) ){
  item->_i = g._tam_uarray;
  item->_x = x; // vertice x
  item->_y = y; // vertice y
  item->_c = l; // comprimento ou length
  item->_a= h;  // altura ou height
} else {
  return 1; // erro
}
if(g._tam_uarray > 0){
  if (colisao(item)){
    fprintf(stdout, "Colisao. Não é possível realizar a operação.\n");
    free(item);
    return 1; // erro
  }
}
  g._tam_uarray++;
  g._u[g._tam_uarray-1] = item;
  return 0; // ok
}

void print_all(){
  short offset=0;
  clrscr();
  if(g.print_buffer){
    for(int i=0;i < 2025;i++){
      *(g.mundo+i) = " ";
    }
  }
  for (size_t i = 0; i < g._tam_uarray; i++) {
    if (g.print_buffer){
      print_buffer(g._u[i]);
    } else {
      print_rect(g._u[i]);
    }
  }
   if(g.print_buffer){
     if(g.rotacao){
       offset = 25;
       for(int i=0;i < 80;i++){
         for(int j=1;j < 26;j++){
           printf("%s", *(g.mundo+j+(i*offset)) ); // 25
         }
         if (i < 79){
            printf("\n");
         }
       }
     } else {
       offset = 80;
       for(int i=0;i < 25;i++){
         for(int j=1;j < 81;j++){
           printf("%s", *(g.mundo+j+(i*offset)) ); // 79
         }
         if(i < 24){
            printf("\n");
         }
       }
     }
   }
}

int find_pos_rect(short x, short y){
  for(int i=0; i < g._tam_uarray; i++){
    if(g._u[i]->_x == x && g._u[i]->_y == y ){
      return g._u[i]->_i;
    }
  }
  return -1; // não encontrou
}


void print_rect(rectang* r){
  short x = r->_x;
  short y = 25 - r->_y; // 25-x corrigir o offset vertical
  short c = r->_c;
  short a = r->_a;
  short i = 0;
  printxy(x, y, "%s", "x"); // inf esq
  printx(x+1,y,c-1,'x'); // linha baixo
  for(i=1;i < a-1;i++){
    printxy(x,y-i,"%c", 'x'); // linha esquerda
  }
  printxy(x+c-1, y, "%s", "x"); // inf drt
  printx(x+1,y-a+1,c-1,'x'); // linha cima
  for(i=1;i < a-1;i++){
      printxy(x+c-1,y-i,"%c", 'x'); // linha direita
  }
  printxy(x, y-a+1, "%s", "x"); // sup esq
  printxy(x+c-1, y-a+1, "%s", "x"); // sup drt
  if (g.debug){
    printxy(x+1, y-1, "%ld", r->_i); // id do rect
  }
  gotoxy(80,24);
}


// void print_rect(rectang* r){
//   short x = r->_x;
//   short y = 25 - r->_y; // 25-x corrigir o offset vertical
//   short c = r->_c;
//   short a = r->_a;
//   short i = 0;
//   printxy(x, y, "%s", "+"); // inf esq
//   printx(x+1,y,c-1,'-'); // linha baixo
//   for(i=1;i < a-1;i++){
//     printxy(x,y-i,"%c", '|'); // linha esquerda
//   }
//   printxy(x+c-1, y, "%s", "+"); // inf drt
//   printx(x+1,y-a+1,c-1,'-'); // linha cima
//   for(i=1;i < a-1;i++){
//       printxy(x+c-1,y-i,"%c", '|'); // linha direita
//   }
//   printxy(x, y-a+1, "%s", "+"); // sup esq
//   printxy(x+c-1, y-a+1, "%s", "+"); // sup drt
//   if (g.debug){
//     printxy(x+1, y-1, "%ld", r->_i); // id do rect
//   }
//   gotoxy(80,24);
// }

void print_buffer(rectang* r){
  short x = r->_x;
  short y = 0;
  if (g.rotacao){
      y = 80 - r->_y; // 80-x corrigir o offset vertical
  } else {
      y = 25 - r->_y; // 25-x corrigir o offset vertical
  }
  short c = r->_c;
  short a = r->_a;
  short i = 0;
  short offset=0;

  (g.rotacao == 1) ?  (offset = 25) : (offset = 80);

  *(g.mundo+x+(y*offset)) = "x"; // inf esq
  for(i=1;i < c-1;i++){ // linha baixo
    *(g.mundo+x+i+(y*offset)) = "x";
  }
  for(i=1;i < a-1;i++){
    *(g.mundo+x+((y-i)*offset)) = "x"; // linha esquerda
  }
  *(g.mundo+x+c-1+(y*offset)) = "x"; // inf drt
  for(i=1;i < c-1;i++){ // linha cima
    *(g.mundo+x+i+((y-a+1)*offset)) = "x";
  }
  for(i=1;i < a-1;i++){
    *(g.mundo+x+c-1+((y-i)*offset)) = "x"; // linha direita
  }
  *(g.mundo+x+((y-a+1)*offset)) = "x"; // sup esq
  *(g.mundo+x+c-1+((y-a+1)*offset)) = "x"; // sup drt
  if (g.debug){
    snprintf(g.buffer, 3 , "%i", r->_i); // id do rect
    *(g.mundo+x+1+((y-1)*offset)) = strcpy((char*)calloc(3, sizeof(char*)), g.buffer);
  }
}

// void print_box_bottom(){
//   printx(1,g.lin-2,g.col,'-');
//   printx(1,g.lin,g.col,'-');

//   printxy(0,g.lin-1,"%c", '|');
//   printxy(g.col,g.lin-1,"%c", '|');

//   printxy(0,g.lin-2, "%s", "+");
//   printxy(g.col, g.lin-2, "%s", "+");
//   printxy(0, g.lin, "%s", "+");
//   printxy(g.col, g.lin, "%s", "+");

// }

// void print_box_nav(const char* seccao){
//   printx(1,4,g.col,'-');
//   printx(1,6,g.col,'-');

//   printxy(0,5,"%c", '|');
//   printxy(g.col,5,"%c", '|');

//   printxy(0, 4, "%s", "+");
//   printxy(g.col, 4, "%s", "+");
//   printxy(0, 6, "%s", "+");
//   printxy(g.col, 6, "%s", "+");

//   print_in_middle(2, 5, g.col-2, seccao);
// }

void print_box_header(){
  printx(1,2,g.col,'*');
  printx(1,9,g.col,'*');
  print_in_middle(4, 4, g.col-2, "EFOLIO B - Mundo dos rectangulos - UAb");
}

int c_getch(void){
  struct termios o, n; // ler 1 char sem eco
  int ch;
  tcgetattr(0, &o);
  n = o;
  n.c_lflag &= ~ICANON;
  n.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &n);
  ch = getchar();
  tcsetattr(0, TCSANOW, &o);
  return ch;
}

void printxy(int x, int y, const char *formato, ...){
    va_list args;
    va_start(args, formato);
    printf("\033[%d;%dH", y, x);
    vprintf(formato, args);
    va_end(args);
    fflush(stdout);
}

void printx(int x, int y, int lim, const char ch){
    printf("\033[%d;%dH", y, x);
    for (int i=0; i < lim; i++){
        putchar(ch);
    }
}

void gotox(unsigned int x){
printf("\x1b[%dG", x);
}

void gotoxy(unsigned int x, unsigned int y){
printf("\x1b[%d;%df", y, x);
}

void nocursor(){
printf("\x1b[?25l");
}

void showcursor(){
printf("\x1b[?25h");
}

void clrscr(){
  printf("\e[1;1H\e[2J");
}

void print_in_middle(int startx, int starty, int largura, const char* string){
  int comp=0, x=0, y=0;
	float temp=0;

	x = startx;
	y = starty;
	if(largura == 0) // col
		largura = 80;

	comp = strlen(string);
	temp = (largura - comp)/ 2;
	x = startx + (int)temp;
	printxy(x, y, "%s", string);
}



const char* lower_string(const char* t){
  int c = 0;
  char* s = (char*) t;
  while (s[c] != '\0') {
    if (s[c] >= 'A' && s[c] <= 'Z') {
      s[c] = s[c] + 32;
    }
    c++;
  }
  return (const char*) s;
}

void carregar_ficheiro(){
  FILE* f=NULL;

  if ( (f = fopen("valores.csv","r")) != NULL){
    while (!feof(f)) {
      memset(&g.param,0, STRPARAM);
      fgets(g.param, STRPARAM, f);
      if(strlen(g.param) < 7){
        continue;
      }
      if (get_param(4)){
        fprintf(stdout, "Load: Erro a obter parametros\n");
        return;
      } else {
        if(create(g._param1, g._param2, g._param3, g._param4 )){
          fprintf(stdout, "Erro na criação do rectangulo. %hu %hu %hu %hu\n", g._param1, g._param2, g._param3, g._param4);
        }
      }
    }
    fclose(f);
  }
}
