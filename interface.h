// interface.h
/*
UC: 21178 - Laboratorio de Programacao
Ano 2021/22 - EFOLIO B - Mundo dos rectangulos - UAb
**
** Aluno: 2100927 - Ivo Baptista 
*/
#define TAM_ARRAY(a) (sizeof(a) / sizeof(a[0]))
#define MAXSTR 512
#define STRPARAM 24
#define MAXTAM 11
#define NO_ROTATION 0
#define ROTATION    1
#define MINLARGURA 3
#define MINALTURA  3


//Definição como macros, fmin retorna o mínimo dentre 2 variáveis; fmax retorna o máximo.
// funcionam com qualquer tipo em que haja uma ordem "maior" ou "menor", seja char, int, float, ou o que for.
#ifndef fmin
#define fmin(a,b) ((a) < (b) ? (a) : (b))
#endif // fmin

#ifndef fmax
#define fmax(a,b) ((a) > (b) ? (a) : (b))
#endif // fmin

typedef struct rectang {
  short _i;
  short _x;
  short _y;
  short _c;
  short _a;
} rectang;

struct global {
unsigned short lin;
unsigned short col;
unsigned short _param1;
unsigned short _param2;
unsigned short _param3;
unsigned short _param4;
unsigned short _tam_uarray;
unsigned short _tam_rarray;
char buffer[MAXSTR];
char param[STRPARAM];
char** mundo;
rectang** _u;
rectang** _r;
short rotacao;
short print_buffer;
short debug;
} g;

// helper funcs
void handle_resize(int sig);
void func_stub();
void sair();
int sair_i();
void printxy(int x, int y, const char *format, ...);
void printx(int x, int y, int lim, const char ch);
void gotox(unsigned int x);
void gotoxy(unsigned int x, unsigned int y);
void nocursor();
void showcursor();
void print_in_middle(int startx, int starty, int largura, const char* string);
void print_box_header();
void print_box_nav(const char* seccao);
void print_box_bottom();
int c_getch();
void clrscr();
void get_term_size();
void set_ini_interface();
int random_number(int interval);
const char* lower_string(const char* s);
void carregar_ficheiro();

// funcs a exportar
//(x,y) são as coordenadas do canto inferior esquerdo
int create(short x, short y, short l, short h);
int moveright(short x, short y, short offset);
int moveleft(short x, short y, short offset);
void print_rect(rectang* r);
void print_buffer(rectang* r);
void print_all();
int find_pos_rect(short x, short y);
int rot_rect();
// int info_rect(short id);
int rect_intersect(rectang* a, rectang* b);
int intersect_interval(short min0, short max0, short min1, short max1);
int limitex(short x0, short x1);
int limitey(short y0, short y1);
int rect_descendente(const void* a, const void* b);
void order_rect_desc();
int gravidade();
int colisao(rectang* a);
void rectanew(int an, rectang* a);

