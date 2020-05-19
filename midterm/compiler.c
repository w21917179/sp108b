#include <assert.h>
#include "compiler.h"

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;
char *labelName[TMAX];  //儲存goto名稱，陣列位置儲存(L0,L1,L2,L3)


#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)
#define emit printf

int findName(char *set) {  //找labelName裡面有沒有相同字串 有的話傳回位置。沒有傳回-1表示要新增
  char eset[SMAX], elabel[SMAX];
  sprintf(eset, " %s ", set);
  for (int i=0; i<=labelIdx; i++){
    sprintf(elabel, " %s ", labelName[i]);
    //printf("set=(%s) isnext=(%s) labelIdx=(%d)\n",eset , elabel, i);
    if(strstr(eset, elabel) != NULL){ return i; }
  }
  return -1 ;
}

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  //printf("set=(%s) isnext=(%s) tokenIdx=(%d)\n",eset , etoken, tokenIdx);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

//偵測兩個字以後
int isNextNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx+1]);
  //printf("set=(%s) isnext=(%s) tokenIdx=(%d)\n",eset , etoken, tokenIdx+1);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  //printf("token[%d]=%s\n", tokenIdx, tokens[tokenIdx]);
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, next());
    assert(0);
  }
}

// F = (E) | Number | Id
int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    emit("t%d = %s\n", f, item);
  }
  return f;
}

// E = F (op E)*
int E() {
  int i1 = F();
  while (isNext("+ - * / & | ! < > =")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    emit("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}

// ASSIGN = id '=' E;
void ASSIGN() {
  char *id = next();
  skip("=");
  int e = E();
  skip(";");
  emit("%s = t%d\n", id, e);
}

void ASSIGNfor() {   //因為for最後一項沒有分號，位了避免錯誤才另外複製
  char *id = next();
  skip("=");
  int e = E();
  emit("%s = t%d\n", id, e);
}

// WHILE = while (E) STMT
void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  emit("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, whileEnd);
  skip(")");
  STMT();
  emit("goto L%d\n", whileBegin);
  emit("(L%d)\n", whileEnd);
}

// FOR = for (ASSIGN; E; ASSIGN) BLOCK
void FOR() {  
  int forCondition = nextLabel();
  int forIncrease = nextLabel();
  int forBegin = nextLabel();
  int forEnd = nextLabel();
  skip("for");
  skip("(");
  ASSIGN();  //int i=1
  emit("(L%d)\n", forCondition);
  int e = E(); //i<3
  emit("if T%d goto L%d\n", e, forBegin);
  emit("goto L%d\n", forEnd);
  skip(";");
  emit("(L%d)\n", forIncrease);
  ASSIGNfor(); //i=i+1
  emit("goto L%d\n", forCondition);
  skip(")");
  emit("(L%d)\n", forBegin);
  BLOCK();
  emit("goto L%d\n", forIncrease);
  emit("(L%d)\n", forEnd);
}
 
// IF = if (E) STMT (else STMT)?
void IF() {
  int ifBegin = nextLabel();
  int ifEnd = nextLabel();
  skip("if");
  skip("(");
  int e = E();
  emit("if not T%d goto L%d\n", e, ifBegin);
  skip(")");
  STMT();
  emit("goto L%d\n", ifEnd);
  emit("(L%d)\n", ifBegin);
  if (isNext("else")) {
    skip("else");
    STMT();
  }
  emit("(L%d)\n", ifEnd);
}

// GOTO
  void GOTO(){
    skip("goto");
    char *item = next();
    int temp = findName(item);
    if(temp==-1){         //新增label i為位置
      int i = nextLabel();
      labelName[i]=item;
      emit("goto L%d\n", i);
    }
    else{
      emit("goto L%d\n", temp);
    }
    skip(";");
  }

//GOTOlabel
  void GOTOlabel(){
    char *item = next();
    int temp = findName(item);
    if(temp==-1){         //新增label i為位置
      int i = nextLabel();
      labelName[i]=item;
      emit("(L%d)\n", i);
    }
    else{
      emit("(L%d)\n", temp);
    }
    skip(":");
  }

// STMT = WHILE | FOR | IF | BLOCK | GOTO | ASSIGN
void STMT() {
  if (isNext("while"))
    return WHILE();
  else if (isNext("for"))
    return FOR();
  else if (isNext("if"))
    return IF();
  else if (isNext("{"))
    BLOCK();
  else if (isNext("goto"))
    GOTO();
  else if (isNextNext(":"))
    GOTOlabel();
  else
    ASSIGN();
}

// STMTS = STMT*
void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

// BLOCK = { STMTS }
void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

// PROG = STMTS
void PROG() {
  STMTS();
}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}