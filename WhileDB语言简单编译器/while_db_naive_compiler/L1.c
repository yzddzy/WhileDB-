#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "L1.h"
#include "lang.h"
#include "lib.h"

unsigned int number_of_spaces = 0;

void print_spaces()
{
  for (int i = 0; i < number_of_spaces; ++i)
  {
    printf(" ");
  }
}

struct L1_const_or_var *new_L1_const_or_var_ptr()
{
  struct L1_const_or_var *res = (struct L1_const_or_var *)malloc(sizeof(struct L1_const_or_var));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct L1_expr *new_L1_expr_ptr()
{
  struct L1_expr *res = (struct L1_expr *)malloc(sizeof(struct L1_expr));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct L1_cmd *new_L1_cmd_ptr()
{
  struct L1_cmd *res = (struct L1_cmd *)malloc(sizeof(struct L1_cmd));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct L1_cmd_list *new_L1_cmd_list_ptr()
{
  struct L1_cmd_list *res = (struct L1_cmd_list *)malloc(sizeof(struct L1_cmd_list));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct L1_cmd_listbox *new_L1_cmd_listbox_ptr()
{
  struct L1_cmd_listbox *res = (struct L1_cmd_listbox *)malloc(sizeof(struct L1_cmd_listbox));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct L1_basic_block_end *new_L1_basic_block_end_ptr()
{
  struct L1_basic_block_end *res = (struct L1_basic_block_end *)malloc(sizeof(struct L1_basic_block_end));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

void move_L1_const_or_var(struct L1_const_or_var *dst, struct L1_const_or_var *src)
{
  memcpy(dst, src, sizeof(struct L1_const_or_var));
  free(src);
}

void move_L1_expr(struct L1_expr *dst, struct L1_expr *src)
{
  memcpy(dst, src, sizeof(struct L1_expr));
  free(src);
}

struct L1_const_or_var *L1_TConst(unsigned int value)
{
  struct L1_const_or_var *res = new_L1_const_or_var_ptr();
  res->t = L1_T_CONST;
  res->d.CONST.value = value;
  return res;
}

struct L1_const_or_var *L1_TVar(unsigned int name)
{
  struct L1_const_or_var *res = new_L1_const_or_var_ptr();
  res->t = L1_T_VAR;
  res->d.VAR.name = name;
  return res;
}

struct L1_const_or_var *L1_ConstOrVar_from_Expr(struct L1_expr *arg)
{
  struct L1_const_or_var *res = new_L1_const_or_var_ptr();
  memcpy(res, &(arg->d.CONST_OR_VAR), sizeof(struct L1_const_or_var));
  free(arg);
  return res;
}

struct L1_expr *L1_TConstOrVar(struct L1_const_or_var *cv)
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_CONST_OR_VAR;
  move_L1_const_or_var(&(res->d.CONST_OR_VAR.cv), cv);
  return res;
}

struct L1_expr *L1_TBinop(enum BinOpType op, struct L1_const_or_var *cv1, struct L1_const_or_var *cv2)
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_BINOP;
  res->d.BINOP.op = op;
  move_L1_const_or_var(&(res->d.BINOP.cv1), cv1);
  move_L1_const_or_var(&(res->d.BINOP.cv2), cv2);
  return res;
}

struct L1_expr *L1_TUnop(enum UnOpType op, struct L1_const_or_var *cv)
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_UNOP;
  res->d.UNOP.op = op;
  move_L1_const_or_var(&(res->d.UNOP.cv), cv);
  return res;
}

struct L1_expr *L1_TDeref(struct L1_const_or_var *cv)
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_DEREF;
  move_L1_const_or_var(&(res->d.DEREF.cv), cv);
  return res;
}

struct L1_expr *L1_TMalloc(struct L1_const_or_var *cv)
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_MALLOC;
  move_L1_const_or_var(&(res->d.MALLOC.cv), cv);
  return res;
}

struct L1_expr *L1_TReadInt()
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_RI;
  return res;
}

struct L1_expr *L1_TReadChar()
{
  struct L1_expr *res = new_L1_expr_ptr();
  res->t = L1_T_RC;
  return res;
}

struct L1_cmd *L1_TAsgnVar(unsigned name, struct L1_expr *right)
{
  struct L1_cmd *res = new_L1_cmd_ptr();
  res->t = L1_T_ASGN_VAR;
  res->d.ASGN_VAR.name = name;
  move_L1_expr(&(res->d.ASGN_VAR.right), right);
  return res;
}

struct L1_cmd *L1_TAsgnMem(unsigned name, struct L1_const_or_var *cv)
{
  struct L1_cmd *res = new_L1_cmd_ptr();
  res->t = L1_T_ASGN_MEM;
  res->d.ASGN_MEM.name = name;
  move_L1_const_or_var(&(res->d.ASGN_MEM.cv), cv);
  return res;
}

struct L1_cmd *L1_TIf(struct L1_expr *cond, struct L1_cmd_listbox *left, struct L1_cmd_listbox *right)
{
  struct L1_cmd *res = new_L1_cmd_ptr();
  res->t = L1_T_IF;
  move_L1_expr(&(res->d.IF.cond), cond);
  res->d.IF.left = left;
  res->d.IF.right = right;
  return res;
}

struct L1_cmd *L1_TWhile(struct L1_cmd_listbox *pre, struct L1_expr *cond, struct L1_cmd_listbox *body)
{
  struct L1_cmd *res = new_L1_cmd_ptr();
  res->t = L1_T_WHILE;
  res->d.WHILE.pre = pre;
  move_L1_expr(&(res->d.WHILE.cond), cond);
  res->d.WHILE.body = body;
  return res;
}

struct L1_cmd *L1_TWriteInt(struct L1_const_or_var *cv)
{
  struct L1_cmd *res = new_L1_cmd_ptr();
  res->t = L1_T_WI;
  move_L1_const_or_var(&(res->d.WI.cv), cv);
  return res;
}

struct L1_cmd *L1_TWriteChar(struct L1_const_or_var *cv)
{
  struct L1_cmd *res = new_L1_cmd_ptr();
  res->t = L1_T_WC;
  move_L1_const_or_var(&(res->d.WC.cv), cv);
  return res;
}

struct L1_cmd_listbox *L1_CmdListBox_Empty()
{
  struct L1_cmd_listbox *res = new_L1_cmd_listbox_ptr();
  res->head = NULL;
  res->tail = &(res->head);
  return res;
}

struct L1_cmd_listbox *L1_CmdListBox_Single(struct L1_cmd *c)
{
  struct L1_cmd_listbox *res = new_L1_cmd_listbox_ptr();
  res->head = new_L1_cmd_list_ptr();
  res->head->head = c;
  res->head->tail = NULL;
  res->tail = &(res->head->tail);
  return res;
}

void L1_CmdListBox_Add1(struct L1_cmd_listbox *cs, struct L1_cmd *c)
{
  struct L1_cmd_list *cl = new_L1_cmd_list_ptr();
  *(cs->tail) = cl;
  cl->head = c;
  cl->tail = NULL;
  cs->tail = &(cl->tail);
}

void L1_CmdListBox_Append(struct L1_cmd_listbox *cs, struct L1_cmd_listbox *cs0)
{
  *(cs->tail) = cs0->head;
  cs->tail = cs0->tail;
  free(cs0);
}

struct L1_basic_block_end *L1_TUJmp(unsigned int dst)
{
  struct L1_basic_block_end *res = new_L1_basic_block_end_ptr();
  res->t = L1_T_UJMP;
  res->dst1 = dst;
  return res;
}

struct L1_basic_block_end *L1_TCJmp(struct L1_expr *cond, unsigned int dst1, unsigned int dst2)
{
  struct L1_basic_block_end *res = new_L1_basic_block_end_ptr();
  res->t = L1_T_CJMP;
  memcpy(&(res->cond), cond, sizeof(struct L1_expr));
  res->dst1 = dst1;
  res->dst2 = dst2;
  return res;
}

struct L1_basic_block_end *L1_BBEndDup(struct L1_basic_block_end *end_info)
{
  struct L1_basic_block_end *res = new_L1_basic_block_end_ptr();
  memcpy(res, end_info, sizeof(struct L1_basic_block_end));
  return res;
}

void L1_set_BBEnd(struct L1_basic_block *bb, struct L1_basic_block_end *end_info)
{
  memcpy(&(bb->end_info), end_info, sizeof(struct L1_basic_block_end));
  free(end_info);
}

struct L1_basic_blocks *L1_BasicBlocks_Init(unsigned int n)
{
  struct L1_basic_blocks *res = (struct L1_basic_blocks *)malloc(sizeof(struct L1_basic_blocks));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  res->n = n;
  res->l = (struct L1_basic_block *)malloc(sizeof(struct L1_basic_block) * n);
  if (res->l == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  res->order = (unsigned int *)malloc(sizeof(unsigned int) * n);
  if (res->order == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

void print_L1_const_or_var(struct L1_const_or_var *cv)
{
  switch (cv->t)
  {
  case L1_T_CONST:
    printf("$(%d)", cv->d.CONST.value);
    break;
  case L1_T_VAR:
    printf("#%d", cv->d.VAR.name);
    break;
  }
}

void print_L1_expr(struct L1_expr *e)
{
  switch (e->t)
  {
  case L1_T_CONST_OR_VAR:
    print_L1_const_or_var(&(e->d.CONST_OR_VAR.cv));
    return;
  case L1_T_BINOP:
    print_binop(e->d.BINOP.op);
    printf("(");
    print_L1_const_or_var(&(e->d.BINOP.cv1));
    printf(", ");
    print_L1_const_or_var(&(e->d.BINOP.cv2));
    printf(")");
    return;
  case L1_T_UNOP:
    print_unop(e->d.UNOP.op);
    printf("(");
    print_L1_const_or_var(&(e->d.UNOP.cv));
    printf(")");
    return;
  case L1_T_DEREF:
    printf("DEREF(");
    print_L1_const_or_var(&(e->d.DEREF.cv));
    printf(")");
    return;
  case L1_T_MALLOC:
    printf("malloc(");
    print_L1_const_or_var(&(e->d.MALLOC.cv));
    printf(")");
    return;
  case L1_T_RI:
    printf("read_int()");
    return;
  case L1_T_RC:
    printf("read_char()");
    return;
  }
}

void print_L1_cmd(struct L1_cmd *c)
{
  print_spaces();
  switch (c->t)
  {
  case L1_T_ASGN_VAR:
    printf("#%d = ", c->d.ASGN_VAR.name);
    print_L1_expr(&(c->d.ASGN_VAR.right));
    printf("\n");
    break;
  case L1_T_ASGN_MEM:
    printf("* #%d = ", c->d.ASGN_MEM.name);
    print_L1_const_or_var(&(c->d.ASGN_MEM.cv));
    printf("\n");
    break;
  case L1_T_IF:
    printf("if (");
    print_L1_expr(&(c->d.IF.cond));
    printf(")\n");
    print_spaces();
    printf("then\n");
    print_L1_cmd_listbox(c->d.IF.left);
    print_spaces();
    printf("else\n");
    print_L1_cmd_listbox(c->d.IF.right);
    break;
  case L1_T_WHILE:
    printf("do\n");
    print_L1_cmd_listbox(c->d.WHILE.pre);
    print_spaces();
    printf("while (");
    print_L1_expr(&(c->d.WHILE.cond));
    printf(") do\n");
    print_L1_cmd_listbox(c->d.WHILE.body);
    break;
  case L1_T_WI:
    printf("write_int(");
    print_L1_const_or_var(&(c->d.WI.cv));
    printf(")\n");
    break;
  case L1_T_WC:
    printf("write_char(");
    print_L1_const_or_var(&(c->d.WC.cv));
    printf(")\n");
    break;
  }
}

void print_L1_cmd_list(struct L1_cmd_list *c)
{
  if (c)
  {
    print_L1_cmd(c->head);
    print_L1_cmd_list(c->tail);
  }
}

void print_L1_cmd_listbox(struct L1_cmd_listbox *c)
{
  print_spaces();
  printf("{\n");
  number_of_spaces += 2;
  print_L1_cmd_list(c->head);
  number_of_spaces -= 2;
  print_spaces();
  printf("}\n");
}

void print_L1_basic_block_end(struct L1_basic_block_end *end_info)
{
  switch (end_info->t)
  {
  case L1_T_CJMP:
    print_spaces();
    printf("if (");
    print_L1_expr(&(end_info->cond));
    printf(") then jmp %d else jmp %d\n", end_info->dst1, end_info->dst2);
    return;
  case L1_T_UJMP:
    print_spaces();
    printf("jmp %d\n", end_info->dst1);
    return;
  }
}

void print_L1_basic_block(struct L1_basic_block *bb)
{
  print_L1_cmd_list(bb->head);
  print_L1_basic_block_end(&(bb->end_info));
}

void print_L1_basic_blocks(struct L1_basic_blocks *bbs)
{
  int i;
  number_of_spaces = 2;
  for (i = 0; i < bbs->n; ++i)
  {
    int id = bbs->order[i];
    printf("Block %d:\n", id);
    print_L1_basic_block((bbs->l) + id);
  }
}

struct SU_hash_table *hashtable;
unsigned int cnt, id, od;
struct L1_basic_blocks *bbs;
struct L1_basic_block_end *end;
struct L1_basic_block *BB_then, *BB_else, *BB_next, *BB_pre, *BB_now, *BB_body;

struct L1_expr *TAC_gen_expr(struct expr *e, struct L1_cmd_listbox *cs)
{
  switch (e->t)
  {
  case T_CONST:
    return L1_TConstOrVar(L1_TConst(e->d.CONST.value));
  case T_VAR:
    return L1_TConstOrVar(L1_TVar(SU_hash_get(hashtable, e->d.VAR.name)));
  case T_BINOP:
  {
    struct L1_expr *ex1, *ex2;
    struct L1_const_or_var *cv1, *cv2;
    ex1 = TAC_gen_expr(e->d.BINOP.left, cs);
    ex2 = TAC_gen_expr(e->d.BINOP.right, cs);
    if (e->d.BINOP.op == T_AND)
    {
      struct L1_cmd_listbox *l1 = L1_CmdListBox_Empty(), *l2 = L1_CmdListBox_Empty();
      L1_CmdListBox_Add1(l1, L1_TAsgnVar(++cnt, ex2));
      l2 = L1_CmdListBox_Single(L1_TAsgnVar(cnt++, L1_TConstOrVar(L1_TConst(0))));
      L1_CmdListBox_Add1(cs, L1_TIf(ex1, l1, l2));
      return L1_TConstOrVar(L1_TVar(cnt - 1));
    }
    else if (e->d.BINOP.op == T_OR)
    {
      struct L1_cmd_listbox *left = L1_CmdListBox_Empty(), *right = L1_CmdListBox_Empty();
      L1_CmdListBox_Add1(right, L1_TAsgnVar(++cnt, ex2));
      left = L1_CmdListBox_Single(L1_TAsgnVar(cnt++, L1_TConstOrVar(L1_TConst(1))));
      L1_CmdListBox_Add1(cs, L1_TIf(ex1, left, right));
      return L1_TConstOrVar(L1_TVar(cnt - 1));
    }
    else
    {
      if ((ex1->t) == L1_T_CONST_OR_VAR)
      {
        cv1 = L1_ConstOrVar_from_Expr(ex1);
      }
      else
      {
        cv1 = L1_TVar(cnt++);
        L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex1));
      }
      if ((ex2->t) == L1_T_CONST_OR_VAR)
      {
        cv2 = L1_ConstOrVar_from_Expr(ex2);
      }
      else
      {
        cv2 = L1_TVar(cnt++);
        L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex2));
      }
      return L1_TBinop(e->d.BINOP.op, cv1, cv2);
    }
  }
  case T_UNOP:
  {
    struct L1_expr *ex;
    struct L1_const_or_var *cv;
    ex = TAC_gen_expr(e->d.UNOP.arg, cs);
    if ((ex->t) == L1_T_CONST_OR_VAR)
    {
      cv = L1_ConstOrVar_from_Expr(ex);
    }
    else
    {
      cv = L1_TVar(cnt++);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex));
    }

    return L1_TUnop(e->d.UNOP.op, cv);
  }
  case T_DEREF:
  {
    struct L1_expr *ex;
    struct L1_const_or_var *cv;
    ex = TAC_gen_expr(e->d.DEREF.arg, cs);
    if ((ex->t) == L1_T_CONST_OR_VAR)
    {
      cv = L1_ConstOrVar_from_Expr(ex);
    }
    else
    {
      cv = L1_TVar(cnt++);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex));
    }
    return L1_TDeref(cv);
  }
  case T_MALLOC:
  {
    struct L1_expr *ex;
    struct L1_const_or_var *cv;
    ex = TAC_gen_expr(e->d.MALLOC.arg, cs);
    if ((ex->t) == L1_T_CONST_OR_VAR)
    {
      cv = L1_ConstOrVar_from_Expr(ex);
    }
    else
    {
      cv = L1_TVar(cnt++);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex));
    }
    return L1_TMalloc(cv);
  }
  case T_RI:
    return L1_TReadInt();
  case T_RC:
    return L1_TReadChar();
  }
}

struct L1_cmd_listbox *TAC_gen(struct cmd *c)
{
  switch (c->t)
  {
  case T_DECL:
    SU_hash_set(hashtable, c->d.DECL.name, cnt++);
    return L1_CmdListBox_Empty();
  case T_ASGN:
  {
    struct L1_cmd_listbox *cs = L1_CmdListBox_Empty();
    struct L1_expr *ex, *ex1, *ex2;
    if ((c->d.ASGN.left->t) == T_VAR)
    {
      ex = TAC_gen_expr(c->d.ASGN.right, cs);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(SU_hash_get(hashtable, c->d.ASGN.left->d.VAR.name), ex));
      return cs;
    }
    if ((c->d.ASGN.left->t) == T_DEREF)
    {
      ex1 = TAC_gen_expr(c->d.ASGN.left->d.DEREF.arg, cs);
      ex2 = TAC_gen_expr(c->d.ASGN.right, cs);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt++, ex1));
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt++, ex2));
      L1_CmdListBox_Add1(cs, L1_TAsgnMem(cnt - 2, L1_TVar(cnt - 1)));
      return cs;
    }
  }
  case T_SEQ:
  {
    struct L1_cmd_listbox *cs1, *cs2;
    cs1 = TAC_gen(c->d.SEQ.left);
    cs2 = TAC_gen(c->d.SEQ.right);
    L1_CmdListBox_Append(cs1, cs2);
    return cs1;
  }
  case T_IF:
  {
    struct L1_cmd_listbox *cs;
    struct L1_expr *ex;
    cs = L1_CmdListBox_Empty();
    ex = TAC_gen_expr(c->d.IF.cond, cs);
    L1_CmdListBox_Add1(cs, L1_TIf(ex, TAC_gen(c->d.IF.left), TAC_gen(c->d.IF.right)));
    return cs;
  }
  case T_WHILE:
  {
    struct L1_cmd_listbox *cs1, *cs;
    struct L1_expr *ex;
    cs = L1_CmdListBox_Empty();
    cs1 = L1_CmdListBox_Empty();
    ex = TAC_gen_expr(c->d.WHILE.cond, cs1);
    L1_CmdListBox_Add1(cs, L1_TWhile(cs1, ex, TAC_gen(c->d.WHILE.body)));
    return cs;
  }
  case T_WI:
  {
    struct L1_cmd_listbox *cs;
    struct L1_expr *ex;
    struct L1_const_or_var *cv;
    cs = L1_CmdListBox_Empty();
    ex = TAC_gen_expr(c->d.WI.arg, cs);
    if ((ex->t) == L1_T_CONST_OR_VAR)
    {
      cv = L1_ConstOrVar_from_Expr(ex);
    }
    else
    {
      cv = L1_TVar(cnt++);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex));
    }
    L1_CmdListBox_Add1(cs, L1_TWriteInt(cv));
    return cs;
  }
  case T_WC:
  {
    struct L1_cmd_listbox *cs;
    struct L1_expr *ex;
    struct L1_const_or_var *cv;
    cs = L1_CmdListBox_Empty();
    ex = TAC_gen_expr(c->d.WC.arg, cs);
    if ((ex->t) == L1_T_CONST_OR_VAR)
    {
      cv = L1_ConstOrVar_from_Expr(ex);
    }
    else
    {
      cv = L1_TVar(cnt++);
      L1_CmdListBox_Add1(cs, L1_TAsgnVar(cnt - 1, ex));
    }
    L1_CmdListBox_Add1(cs, L1_TWriteChar(cv));
    return cs;
  }
  }
}

void basic_block_gen(struct L1_cmd_list *p, struct L1_basic_block_end *end_info, struct L1_basic_block *bb)
{
  struct L1_cmd_list **ptr_p = &(bb->head);
  *ptr_p = p;
  while (p)
  {
    switch (p->head->t)
    {
    case L1_T_IF:
      if (p->tail)
      {
        end = L1_TUJmp(id + 2);
        BB_now = bb;
        BB_then = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.IF.left->head, L1_BBEndDup(end), BB_then);
        BB_else = bbs->l + id;
        bbs->order[od++] = id++;

        basic_block_gen(p->head->d.IF.right->head, L1_BBEndDup(end), BB_else);
        BB_next = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->tail, L1_BBEndDup(end_info), BB_next);
        *ptr_p = NULL;
        L1_set_BBEnd(BB_now, L1_TCJmp(&(p->head->d.IF.cond), id - 3, id - 2));
      }
      else if (end_info->t == L1_T_CJMP)
      {
        end = L1_TUJmp(id + 2);
        BB_now = bb;
        BB_then = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.IF.left->head, L1_BBEndDup(end), BB_then);
        BB_else = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.IF.right->head, L1_BBEndDup(end), BB_else);
        BB_next = bbs->l + id;
        bbs->order[od++] = id++;

        basic_block_gen(L1_CmdListBox_Empty()->head, L1_BBEndDup(end_info), BB_next);
        *ptr_p = NULL;
        L1_set_BBEnd(bb, L1_TCJmp(&(p->head->d.IF.cond), id - 3, id - 2));
      }
      else
      {
        BB_now = bb;
        BB_then = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.IF.left->head, L1_BBEndDup(end_info), BB_then);
        BB_else = bbs->l + id;
        bbs->order[od++] = id++;

        basic_block_gen(p->head->d.IF.right->head, L1_BBEndDup(end_info), BB_else);
        *ptr_p = NULL;
        L1_set_BBEnd(BB_now, L1_TCJmp(&(p->head->d.IF.cond), id - 2, id - 1));
      }
      return;
    case L1_T_WHILE:
      if (p->tail)
      {
        BB_now = bb;
        if (BB_now)
        {
          BB_pre = bbs->l + id;
          bbs->order[od++] = id++;
          basic_block_gen(p->head->d.WHILE.pre->head, L1_BBEndDup(L1_TCJmp(&(p->head->d.WHILE.cond), id + 1, id + 2)), BB_pre);
          L1_set_BBEnd(BB_now, L1_TUJmp(id - 1));
        }
        else
        {
          BB_pre = BB_now;
          L1_set_BBEnd(BB_pre, L1_TCJmp(&(p->head->d.WHILE.cond), id + 1, id + 2));
        }
        end = L1_TUJmp(id - 1);
        BB_body = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.WHILE.body->head, L1_BBEndDup(end), BB_body);
        BB_next = bbs->l + id;
        bbs->order[od++] = id++;

        basic_block_gen(p->tail, L1_BBEndDup(end_info), BB_next);

        *ptr_p = NULL;
      }
      else if (end_info->t == L1_T_CJMP)
      {
        BB_now = bb;
        if (BB_now)
        {
          BB_pre = bbs->l + id;
          bbs->order[od++] = id++;
          basic_block_gen(p->head->d.WHILE.pre->head, L1_BBEndDup(L1_TCJmp(&(p->head->d.WHILE.cond), id + 1, id + 2)), BB_pre);
          L1_set_BBEnd(BB_now, L1_TUJmp(id - 1));
        }
        else
        {
          BB_pre = BB_now;
          L1_set_BBEnd(BB_pre, L1_TCJmp(&(p->head->d.WHILE.cond), id + 1, id + 2));
        }
        end = L1_TUJmp(id - 1);
        BB_body = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.WHILE.body->head, L1_BBEndDup(end), BB_body);
        BB_next = bbs->l + id;
        bbs->order[od++] = id++;

        basic_block_gen(L1_CmdListBox_Empty()->head, L1_BBEndDup(end_info), BB_next);

        *ptr_p = NULL;
      }
      else
      {
        BB_now = bb;
        if (BB_now)
        {
          BB_pre = bbs->l + id;
          bbs->order[od++] = id++;
          basic_block_gen(p->head->d.WHILE.pre->head, L1_BBEndDup(L1_TCJmp(&(p->head->d.WHILE.cond), id + 1, id + 2)), BB_pre);
          L1_set_BBEnd(BB_now, L1_TUJmp(id - 1));
        }
        else
        {
          BB_pre = BB_now;
          L1_set_BBEnd(BB_pre, L1_TCJmp(&(p->head->d.WHILE.cond), id + 1, id + 2));
        }
        end = L1_TUJmp(id - 1);
        BB_body = bbs->l + id;
        bbs->order[od++] = id++;
        basic_block_gen(p->head->d.WHILE.body->head, L1_BBEndDup(end), BB_body);
      }
      return;
    default:
      ptr_p = &(p->tail);
      p = *ptr_p;
      break;
    }
  }
  L1_set_BBEnd(bb, end_info);
}

struct L1_basic_blocks *BB_gen(struct L1_cmd_listbox *cs)
{
  bbs = L1_BasicBlocks_Init(1000);
  bbs->order[od++] = id++;
  basic_block_gen(cs->head, L1_TUJmp(1000), bbs->l);
  bbs->n = id;
  return bbs;
}
