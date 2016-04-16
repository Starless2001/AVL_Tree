#pragma once

struct node_struct
{
	struct node_struct * esq;
	struct node_struct * dir;
	int valor;
};

typedef struct node_struct node;

struct tree_struct
{
	struct node_struct * raiz;
};

typedef struct tree_struct tree;

void liberar_memoria_impl ( node * node_ptr );

void liberar_memoria ( tree * tree_ptr );

tree * init_tree ( );

node * init_node ( int valor );

int altura ( const node * const node_ptr );

int fator_bal ( const node * const node_ptr );

node * rotacionar_esq_esq ( const node * const node_ptr );

node * rotationar_esq_dir ( const node * const node_ptr );

node * rotacionar_dir_esq ( const node * const node_ptr );

node * rotacionar_dir_dir ( const node * const node_ptr );

node * balancear_node ( node * const node_ptr );

void balancear_tree ( tree * const tree_ptr );

void inserir ( tree * tree_ptr, int valor );

node * encontrar_valor ( const tree * const tree, int valor );

void avl_traverse_node_dfs ( node * node, int depth );

void avl_traverse_dfs ( tree * tree );

