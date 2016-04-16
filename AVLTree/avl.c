#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include "avl.h"

void liberar_memoria_impl ( node * node_ptr )
{
	if ( node_ptr == NULL )
		return;

	liberar_memoria_impl ( node_ptr->esq );
	liberar_memoria_impl ( node_ptr->dir );

	free ( node_ptr );
}


void liberar_memoria ( tree * tree_ptr )
{
	liberar_memoria_impl ( tree_ptr->raiz );
}


tree * init_tree ( )
{
	tree * tree = NULL;

	if ( ( tree = malloc ( sizeof ( tree ) ) ) == NULL )
	{
		return NULL;
	}

	tree->raiz = NULL;

	return tree;
}

node * init_node ( int valor )
{
	node * node_ptr;

	node_ptr = malloc ( sizeof ( node ) );
	if ( node_ptr == NULL )
	{
		puts ( "o programa ficou sem memória\n" );
		system ( "pause" );
	}

	node_ptr->esq = NULL;
	node_ptr->dir = NULL;
	node_ptr->valor = valor;

	return node_ptr;
}

int altura ( const node * const node_ptr )
{
	int altura_esq = 0;
	int altura_dir = 0;

	if ( node_ptr->esq )
		altura_esq = altura ( node_ptr->esq );

	if ( node_ptr->dir )
		altura_dir = altura ( node_ptr->dir );

	return max ( altura_dir, altura_esq ) + 1;
}


int fator_bal ( const node * const node_ptr )
{
	int fator = 0;

	if ( node_ptr->esq )
		fator += altura ( node_ptr->esq );

	if ( node_ptr->dir )
		fator -= altura ( node_ptr->dir );

	return fator;
}


node * rotacionar_esq_esq ( const node * const node_ptr )
{
	node * const temp_ptr = node_ptr;
	node * esq_ptr = temp_ptr->esq;

	temp_ptr->esq = esq_ptr->dir;
	esq_ptr->dir = temp_ptr;

	return esq_ptr;
}

node * rotationar_esq_dir (const node * const node_ptr )
{
	node * const temp_ptr = node_ptr;
	node * esq_ptr = temp_ptr->esq;
	node * dir_ptr = esq_ptr->dir;

	temp_ptr->esq = dir_ptr->dir;
	esq_ptr->dir = dir_ptr->esq;
	dir_ptr->esq = esq_ptr;
	dir_ptr->dir = temp_ptr;

	return dir_ptr;
}

node * rotacionar_dir_esq ( const node * const node_ptr )
{
	node * const temp_ptr = node_ptr;
	node * dir_ptr = temp_ptr->dir;
	node * esq_ptr = dir_ptr->esq;

	temp_ptr->dir = esq_ptr->esq;
	dir_ptr->esq = esq_ptr->dir;
	esq_ptr->dir = dir_ptr;
	esq_ptr->esq = temp_ptr;

	return esq_ptr;
}


node * rotacionar_dir_dir ( const node * const node_ptr )
{
	node * const temp_ptr = node_ptr;
	node * dir_ptr = temp_ptr->dir;

	temp_ptr->dir = dir_ptr->esq;
	dir_ptr->esq = temp_ptr;

	return dir_ptr;
}

node * balancear_node ( node * const node_ptr )
{
	node * node_balanceado = NULL;

	if ( node_ptr->esq )
		node_ptr->esq = balancear_node ( node_ptr->esq );

	if ( node_ptr->dir )
		node_ptr->dir = balancear_node ( node_ptr->dir );

	int fator = fator_bal ( node_ptr );

	if ( fator >= 2 )
	{
		/* pesando pra esquerda */

		if ( fator_bal ( node_ptr->esq ) <= -1 )
			node_balanceado = rotationar_esq_dir ( node_ptr );
		else
			node_balanceado = rotacionar_esq_esq ( node_ptr );

	}
	else if ( fator <= -2 )
	{
		/* pesando pra direita */

		if ( fator_bal ( node_ptr->dir ) >= 1 )
			node_balanceado = rotacionar_dir_esq ( node_ptr );
		else
			node_balanceado = rotacionar_dir_dir ( node_ptr );

	}
	else
	{
		node_balanceado = node_ptr;
	}

	return node_balanceado;
}


void balancear_tree ( tree * const tree_ptr )
{
	node * nova_raiz = NULL;

	nova_raiz = balancear_node ( tree_ptr->raiz );

	if ( nova_raiz != tree_ptr->raiz )
	{
		tree_ptr->raiz = nova_raiz;
	}
}

void inserir ( tree * tree_ptr, int valor )
{
	node * novo_node_ptr = NULL;
	node * next_ptr = NULL;
	node * last_ptr = NULL;

	if ( tree_ptr->raiz == NULL )
	{
		novo_node_ptr = init_node ( valor );
		tree_ptr->raiz = novo_node_ptr;
	}
	else
	{
		next_ptr = tree_ptr->raiz;

		while ( next_ptr != NULL )
		{
			last_ptr = next_ptr;

			if ( valor < next_ptr->valor )
			{
				next_ptr = next_ptr->esq;

			}
			else if ( valor > next_ptr->valor )
			{
				next_ptr = next_ptr->dir;
			}
			else if ( valor == next_ptr->valor )
			{
				return;
			}
		}

		novo_node_ptr = init_node ( valor );

		if ( valor < last_ptr->valor )
			last_ptr->esq = novo_node_ptr;

		if ( valor > last_ptr->valor )
			last_ptr->dir = novo_node_ptr;

	}

	balancear_tree ( tree_ptr );
}

//retorna NULL se não encontrar. Essa é função para que a árvore é construída: para se buscar valores nela.
node * encontrar_valor (const tree * const tree, int valor )
{
	node * atual = tree->raiz;

	while ( atual && atual->valor != valor )
	{
		if ( valor > atual->valor )
			atual = atual->dir;
		else
			atual = atual->esq;
	}

	return atual;
}

void listar ( node * node_ptr )
{
	if ( node_ptr != NULL )
	{
		printf ( "No = %d, altura = %d, FATBAL = %d\n", node_ptr->valor, altura ( node_ptr ), fator_bal ( node_ptr ) );
		listar ( node_ptr->esq );
		listar ( node_ptr->dir );
	}
}


/*int main ( int argc, char **argv )
{
	tree * tree = NULL;
	int i;
	int valor;

	tree = init_tree ( );

	srand ( time ( NULL ) );

	for ( i = 0; i < 100; i++ )
	{
		do
		{
			valor = rand ( ) % 1000;
		} while ( encontrar_valor ( tree, valor ) );

		inserir ( tree, valor );
	}

	avl_traverse_dfs ( tree );

	system ( "pause" );

	return 0;
}*/

int main ( )
{
	tree * tree_ptr = init_tree ( );

	int op, valor;
	while ( 1 )
	{
		puts ( "------------------------------"
				 "\n| 0- Sair;"
				 "\n| 1- Inserir;"
				 "\n| 2- Listar;"
				 "\n------------------------------"
				 "\n\n| Opcao: " );
		scanf_s ( "%d", &op );
		switch ( op )
		{
			case 0:
				liberar_memoria ( tree_ptr );
				system ( "pause" );
				exit ( 0 );
				break;
			case 1:
				puts ( "\nInforme o valor: " );
				scanf_s ( "%d", &valor );
				inserir ( tree_ptr, valor );
				break;
			case 2:
				if ( tree_ptr->raiz == NULL )
				{
					puts ( "\n| Arvore vazia!\n\n" );
				}
				else
				{
					puts ( "\n" );
					listar ( tree_ptr->raiz );
					puts ( "\n\n" );
				}
				break;
		}
	}
}

