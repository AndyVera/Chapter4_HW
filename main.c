
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP   100    /* max size of operand/operator */
#define NUMBER '0'     /* signal that a number was found */
#define MAXVAL  100


size_t sp = 0;   // aka unsigned long -- printf using %zu
double val[MAXVAL];   // stack of values

char buf[BUFSIZ];
size_t bufp = 0;

int getch_(void) { return (bufp > 0) ? buf[--bufp] : getchar(); }
void ungetch_(int c) {
  if (bufp >= BUFSIZ) { fprintf(stderr, "ungetch: too many characters\n");  return; }
  buf[bufp++] = c;
}

int getop(char* s) {
  int i, c;
  while ((s[0] = c = getch_()) == ' ' || c == '\t') { }  // skip whitespace
  s[1] = '\0';
  
  if (!isdigit(c) && c != '.') { return c; }  // if not a digit, return

  i = 0;
  if (isdigit(c)) {  // get digits before '.'
    while (isdigit(s[++i] = c = getch_())) { }
  }
  if (c == '.') {    // get digits after decimal (if any)
    while (isdigit(s[++i] = c = getch_())) { }
  }
  s[i] = '\0';
  if (c != EOF) { ungetch_(c); }
  return NUMBER;      // return type is NUMBER, number stored in s
}

double pop(void) {
  if (sp == 0) { fprintf(stderr, "stack underflow\n");  return 0.0; }
  return val[--sp];
}

double return_top(void){
    if( sp == 0) { fprintf(stderr, "stack underflow\n"); return 0.0; }
    return val[sp -1];
}

void push(double f) {
  if (sp == MAXVAL) { fprintf(stderr, "stack overflow -- can't push %g\n", f);  return; }
  val[sp++] = f;
}

void swapping(void){
    int first, second;
    first = pop();
    second = pop();
    
    push(first);
    push(second);
}


void rpn(void) {
  int type;
  double op2;
  char s[BUFSIZ];
  
  while ((type = getop(s)) != EOF) {
    switch(type) {
      case '\n':    printf("\t%.8g\n", pop());  break;
      case NUMBER:  push(atof(s));              break;
      case '+':     push(pop() + pop());        break;
      case '*':     push(pop() * pop());        break;
      case '-':     push(-(pop() - pop()));     break;
      case '/':
        if ((op2 = pop()) == 0.0) { fprintf(stderr, "divide by zero\n");  break; }
        push(pop() / op2);
        break;
     //4-3
      case '%':
            if((op2 = pop()) == 0.0){ fprintf(stderr, "divisor must not be zero\n"); break;}
            push(fmod(pop(), op2));
            break;
    //4-4
      case 't':
            return_top();
            break;
      case 'c':
            sp = 0;
            break;
        case 'm':
            swapping();
            break;
        case 'd':
            op2 = return_top();
            push(op2);
            break;
    //4-5
        case 'e':
            push(exp(pop()));
            break;
        case 's':
            push(sin(pop()));
            break;
        case 'p':
            op2 = pop();
            push(pow(pop(), op2));
            break;
        
            
    //4-6
      default:
            if(isalpha(type) && type != 't' && type != 'c' && type != 'm' && type != 'd' && type != 'e' && type != 's' && type != 'p'){
                push(type);
            }
      fprintf(stderr, "unknown command %s\n", s);  break;
    }
  }
}

int main(int argc, const char * argv[]) {
  rpn();

  return 0;
}


