/*
	Name:		Farhan Jiva
	Date:		04-24-08
	Course:		CSCI 2720
	Purpose:		Implementing advanced binary tree based
				data structures called AVL trees
*/
/*
	Notes:
		1 == true
		0 == false
*/

#include <stdio.h> /* printf() scanf() fopen() perror() fscanf() */
#include <string.h> /* strncpy() */
#include <stdlib.h> /* malloc() free() */

typedef struct node
{
	int key; /* the node's key value */
	char data[5]; /* the node's data value */
	int balance; /* the node's balance value: -1, 0, 1 */
	int rh; /* the node's right height */
	int lh; /* the node's left height */
	struct node *parent; /* the node's parent */
	struct node *rc; /* the node's right child */
	struct node *lc; /* the node's left child */
} anode;

anode *root = NULL; /* the root of the AVL tree */

/* function which returns the max between m1 and m2 */
int max(int m1, int m2)
{
	if(m1 > m2)
	{
		return m1;
	}
	else if(m1 < m2)
	{
		return m2;
	}
	else
	{
		return m1;
	}
}

/* function which returns the height of a node */
int height(anode *node, int cur)
{
	if(node == NULL)
	{
		return cur;
	}
	else if(node->lc != NULL && node->rc != NULL)
	{
		return max(height(node->lc, cur + 1), height(node->rc, cur + 1));
	}
	else if(node->lc != NULL)
	{
		return max(height(node->lc, cur + 1), cur);
	}
	else if(node->rc != NULL)
	{
		return max(cur, height(node->rc, cur + 1));
	}
	else
	{
		return cur;
	}
}

/* void function which reassigns the balance values for each node */
void reassbal(anode *node)
{
	if(node->lc != NULL)
	{
		reassbal(node->lc);
	}
	
	if(node->lc == NULL && node->rc == NULL)
	{
		node->lh = height(node->lc, 0);
		node->rh = height(node->rc, 0);
		node->balance = node->rh - node->lh;
	}
	else if(node->lc != NULL && node->rc == NULL)
	{
		node->lh = height(node->lc, 1);
		node->rh = height(node->rc, 0);
		node->balance = node->rh - node->lh;
	}
	else if(node->lc == NULL && node->rc != NULL)
	{
		node->lh = height(node->lc, 0);
		node->rh = height(node->rc, 1);
		node->balance = node->rh - node->lh;
	}
	else if(node->lc != NULL && node->rc != NULL)
	{
		node->lh = height(node->lc, 1);
		node->rh = height(node->rc, 1);
		node->balance = node->rh - node->lh;
	}
	
	if(node->rc != NULL)
	{
		reassbal(node->rc);
	}
}

/* void function which performs a left rotation on node */
void leftrotate(anode *node)
{
	anode *p = node; /* the root of the unbalanced subtree */
	anode *pp = node->parent; /* p's parent */
	anode *q = node->rc; /* p's right child */
	
	if(pp != NULL)
	{
		if(pp->rc == p) /* q is new root */
		{
			pp->rc = q;
			q->parent = pp;
		}
		else if(pp->lc == p)
		{
			pp->lc = q;
			q->parent = pp;
		}
	}
	else
	{
		root = q;
		q->parent = NULL;
	}
	p->rc = q->lc;
	q->lc = p;
	
	p->parent = q;
	if(p->rc != NULL)
	{
		p->rc->parent = p;
	}
}

/* void function which performs a right rotation on node */
void rightrotate(anode *node)
{
	anode *q = node; /* the root of the unbalanced subtree */
	anode *qq = node->parent; /* q's parent */
	anode *p = q->lc; /* q's left child */
	
	if(qq != NULL)
	{
		if(qq->rc == q) /* p is the new root */
		{
			qq->rc = p;
			p->parent = qq;
		}
		else if(qq->lc == q)
		{
			qq->lc = p;
			p->parent = qq;
		}
	}
	else
	{
		root = p;
		p->parent = NULL;
	}
	q->lc = p->rc;
	p->rc = q;
	
	q->parent = p;
	if(q->lc != NULL)
	{
		q->lc->parent = q;
	}
}

/* void function which determines the rotation type */
void rproc(anode *p)
{
	if(p->balance < -2 || p->balance > 2)
	{
		rproc(p->lc);
		rproc(p->rc);
	}
	if(p->balance == 2)
	{
		if(p->rc->balance == 2 || p->rc->balance == -2)
		{
			rproc(p->rc);
		}
		if(p->rc->balance == 1) /* single rotation */
		{
			leftrotate(p);
			reassbal(root);
			return;
		}
		if(p->rc->balance == -1) /* double rotation */
		{
			rightrotate(p->rc);
			leftrotate(p);
			reassbal(root);
			return;
		}
	}
	if(p->balance == -2)
	{
		if(p->lc->balance == 2 || p->lc->balance == -2)
		{
			rproc(p->lc);
		}
		if(p->lc->balance == -1) /* single rotation */
		{
			rightrotate(p);
			reassbal(root);
			return;
		}
		if(p->lc->balance == 1) /* double rotation */
		{
			leftrotate(p->lc);
			rightrotate(p);
			reassbal(root);
			return;
		}
	}
}

/* void function which balances the AVL tree */
void balance(anode *node)
{
	if(node->lc != NULL)
	{
		balance(node->lc);
	}
	if((node->balance <= -2) || (node->balance >= 2))
	{
		rproc(node);
	}
	if(node->rc != NULL)
	{
		balance(node->rc);
	}
}

/* void function which performs an inorder traversal on the AVL tree */
void inorder(anode *node)
{
	if(node == NULL)
	{
		printf("Error: Tree does not exist!\n");
		return;
	}
	if(node->lc != NULL)
	{
		inorder(node->lc);
	}
	printf("(%d %s) ", node->key, node->data);
	if(node->rc != NULL)
	{
		inorder(node->rc);
	}
}

/* void function which prints the nodes along with its corresponding balance */
void printbalances(anode *node)
{
	if(node == NULL)
	{
		printf("Error: Tree does not exist!\n");
		return;
	}
	if(node->lc != NULL)
	{
		printbalances(node->lc);
	}
	printf("(%d %s %d %d %d) ", node->key, node->data, node->lh, node->rh, node->balance);
	if(node->rc != NULL)
	{
		printbalances(node->rc);
	}
}

/* void function which inserts a node into the AVL tree */
void insert(int key, char *data)
{
	if(root == NULL)
	{
		root = (anode *)malloc(sizeof(anode));
		root->key = key;
		strncpy(root->data, data, 5);
		root->balance = 0;
		root->parent = NULL;
		root->rc = NULL;
		root->lc = NULL;
	}
	else
	{
		anode *iter = root;
		while(1)
		{
			if(key < iter->key)
			{
				if(iter->lc == NULL)
				{
					anode *new = (anode *)malloc(sizeof(anode));
					new->key = key;
					strncpy(new->data, data, 5);
					new->balance = 0;
					new->parent = iter;
					new->rc = NULL;
					new->lc = NULL;
					iter->lc = new;
					reassbal(root);
					balance(root);
					break;
				}
				iter = iter->lc;
			}
			else if(key > iter->key)
			{
				if(iter->rc == NULL)
				{
					anode *new = (anode *)malloc(sizeof(anode));
					new->key = key;
					strncpy(new->data, data, 5);
					new->balance = 0;
					new->parent = iter;
					new->rc = NULL;
					new->lc = NULL;
					iter->rc = new;
					reassbal(root);
					balance(root);
					break;
				}
				iter = iter->rc;
			}
			else if(key == iter->key)
			{
				strncpy(iter->data, data, 5);
				break;
			}
		}
	}
}

/* function which returns 1 if the given key can be found, 0 otherwise */
int find(int key)
{
	anode *iter = root;
	if(iter == NULL)
	{
		return 0;
	}
	while(1)
	{
		if(key == iter->key)
		{
			return 1;
		}
		else if(key < iter->key)
		{
			iter = iter->lc;
			if(iter == NULL)
			{
				return 0;
			}
		}
		else
		{
			iter = iter->rc;
			if(iter == NULL)
			{
				return 0;
			}
		}
	}
}

/* void function which looks up the node associated with the key value */
void lookup(int key)
{
	int result = find(key);
	if(result == 0)
	{
		printf("Key not found!\n");
	}
	else
	{
		anode *iter = root;
		while(1)
		{
			if(key == iter->key)
			{
				printf("(%d %s)\n", iter->key, iter->data);
				break;
			}
			else if(key < iter->key)
			{
				printf("(%d %s) -> ", iter->key, iter->data);
				iter = iter->lc;
			}
			else
			{
				printf("(%d %s) -> ", iter->key, iter->data);
				iter = iter->rc;
			}
		}
	}
}

/* function which returns a pointer to the inorder successor of node */
anode * getsuccessor(anode *node)
{
	anode *succ = node;
	if(succ->rc != NULL)
	{
		succ = succ->rc;
	}
	while(succ->lc != NULL)
	{
		succ = succ->lc;
	}
	return succ;
}

/* void function which deletes a node given its cooresponding key value */
void delete(int key)
{
	int result = find(key);
	
	if(result == 0)
	{
		printf("Key not found!\n");
	}
	else
	{
		anode *iter = root;
		if(iter->lc == NULL && iter->rc == NULL) /* root is the only node */
		{
			free(iter);
			root = NULL;
			return;
		}
		
		while(1)
		{
			if(key == iter->key)
			{
				if(iter->lc == NULL && iter->rc == NULL) /* is leaf */
				{
					if(iter->parent->rc == iter)
					{
						iter->parent->rc = NULL;
					}
					else if(iter->parent->lc == iter)
					{
						iter->parent->lc = NULL;
					}
					free(iter);
					reassbal(root);
					balance(root);
					return;
				}
				else if(iter->rc == NULL)
				{
					iter->key = iter->lc->key;
					strncpy(iter->data, iter->lc->data, 5);
					free(iter->lc);
					iter->lc = NULL;
					reassbal(root);
					balance(root);
					return;
				}
				else
				{
					anode *succ = getsuccessor(iter);
					/* 2 cases */
					if(succ->lc == NULL && succ->rc == NULL) /* succ is a leaf */
					{
						if(succ->parent->rc == succ)
						{
							succ->parent->rc = NULL;
						}
						else if(succ->parent->lc == succ)
						{
							succ->parent->lc = NULL;
						}
						succ->parent = NULL;
						iter->key = succ->key;
						strncpy(iter->data, succ->data, 5);
						free(iter);
						reassbal(root);
						balance(root);
						return;
					}
					else if(succ->rc != NULL)
					{
						/* 2 cases */
						if(succ->rc->rc != NULL)
						{
							anode *schild1 = succ->rc;
							anode *schild2 = succ->rc->rc;
							
							iter->key = succ->key;
							strncpy(iter->data, succ->data, 5);
							
							succ->key = schild1->key;
							strncpy(succ->data, schild1->data, 5);
							
							schild1->key = schild2->key;
							strncpy(schild1->data, schild2->data, 5);
							
							schild1->rc = NULL;
							free(schild2);
							reassbal(root);
							balance(root);
							return;
						}
						else
						{
							iter->key = succ->key;
							strncpy(iter->data, succ->data, 5);
							
							anode *schild = succ->rc;
							
							succ->key = schild->key;
							strncpy(succ->data, schild->data, 5);
							
							succ->rc = NULL;
							
							free(schild);
							reassbal(root);
							balance(root);
							return;
						}
					}
				}
			}
			else if(key < iter->key)
			{
				iter = iter->lc;
			}
			else
			{
				iter = iter->rc;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int choice = -1; /* initialized to -1 */
	int key0;
	int key1;
	int key2;
	char data[5];
	
	FILE *input = fopen("input.txt", "r");
	if(input == NULL) /* error checking */
	{
		perror("Error, can not open input.txt");
		return -1;
	}
	while(fscanf(input, "%d %s", &key0, data) == 2)
	{
		insert(key0, data);
	}
	
	while(choice != 5) /* a menu */
	{
		printf("Main Menu: Choose an option...\n");
		printf("1. Lookup a node from the AVL tree\n");
		printf("2. Print the AVL tree\n");
		printf("3. Delete a node from the AVL tree\n");
		printf("4. Print the balances of the nodes\n");
		printf("5. Exit\n");
		
		scanf("%d", &choice);
		
		switch(choice)
		{
			case 1:
				printf("Enter the key value you wish to lookup: ");
				scanf("%d", &key1);
				lookup(key1);
				break;
			case 2:
				inorder(root);
				printf("\n");
				break;
			case 3:
				printf("Enter the key value you wish to delete: ");
				scanf("%d", &key2);
				delete(key2);
				break;
			case 4:
				printbalances(root);
				printf("\n");
				break;
			case 5:
				break;
			default:
				printf("Error: %d is not a valid option\n", choice);
				break;
		}
	}
	
	return 0;
}