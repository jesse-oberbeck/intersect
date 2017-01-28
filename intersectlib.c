#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "intersectlib.h"

//https://gist.github.com/tonious/1377768 from here down to next reference.
/* Find the balance of an AVL node */
int avl_balance_factor( node *node ) {
	int bf = 0;

	if( node->left  ) bf += tree_height( node->left );
	if( node->right ) bf -= tree_height( node->right );

	return bf ;
}

/* Left Left Rotate */
node *avl_rotate_leftleft( node *n ) {
 	node *a;
 	a = n;
	node *b;
	b = a->left;
	
	a->left = b->right;
	b->right = a;

	return( b );
}

/* Left Right Rotate */
node *avl_rotate_leftright( node *n ) {
	node *a = n;
	node *b = a->left;
	node *c = b->right;
	
	a->left = c->right;
	b->right = c->left; 
	c->left = b;
	c->right = a;

	return( c );
}

/* Right Left Rotate */
node *avl_rotate_rightleft( node *n ) {
	node *a = n;
	node *b = a->right;
	node *c = b->left;
	
	a->right = c->left;
	b->left = c->right; 
	c->right = b;
	c->left = a;

	return( c );
}

/* Right Right Rotate */
node *avl_rotate_rightright( node *n ) {
	node *a = n;
	node *b = a->right;
	
	a->right = b->left;
	b->left = a; 

	return( b );
}

/* Balance a given node */
node *avl_balance_node( node *n ) {
	node *newroot = NULL;

	/* Balance our children, if they exist. */
	if( n->left )
		n->left  = avl_balance_node( n->left  );
	if( n->right ) 
		n->right = avl_balance_node( n->right );

	int bf = avl_balance_factor( n );

	if( bf >= 2 ) {
		/* Left Heavy */	

		if( avl_balance_factor( n->left ) <= -1 ) 
			newroot = avl_rotate_leftright( n );
		else 
			newroot = avl_rotate_leftleft( n );

	} else if( bf <= -2 ) {
		/* Right Heavy */

		if( avl_balance_factor( n->right ) >= 1 )
			newroot = avl_rotate_rightleft( n );
		else 
			newroot = avl_rotate_rightright( n );

	} else {
		/* This node is balanced -- no change. */

		newroot = n;
	}

	return( newroot );	
}

/* Balance a given tree */
void avl_balance( node *tree ) {

	node *newroot = NULL;

	newroot = avl_balance_node( tree );

	if( newroot != tree )  {
		tree = newroot; 
	}
}
//https://gist.github.com/tonious/1377768 from here up to next reference.


node *
FindMin(
    node * root)
{
    //Function taken and modified from BST exercise done in class.
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->left == NULL)
    {
        return root;
    }
    else                        // found match
    {
        return FindMin(root->left);
    }
}

node *
Delete(
    node * root,
    char *word)
{
    //Function taken and modified from BST exercise done in class.
    node *temp = NULL;

    if (root == NULL)
    {
        return root;
    }
    else if (strcasecmp(word, root->word) < 0)
    {
        root->left = Delete(root->left, word);
    }
    else if (strcasecmp(word, root->word) > 0)
    {
        root->right = Delete(root->right, word);
    }
    else                        // found the element
    {
        if (root->left && root->right)  // 2 kids
        {
            // replace with smallest in right subtree
            temp = FindMin(root->right);
            // this just replaces the data
            strncpy(root->word, temp->word, strlen(root->word));
            // delete (or swap again) node we just got the value from
            root->right = Delete(root->right, root->word);
        }
        else                    // one kid
        {
            temp = root;
            root = (root->left != NULL) ? root->left : root->right;
            free(temp->word);
            free(temp);
        }
    }
    return root;
}

FILE *
openFile(
    char *fileName)
{

    char file[255];
    char *path = getenv("HOME");

    sprintf(file, "%s%s", path, fileName);
    return (fopen(fileName, "r"));
}

node *
Insert(
    node * root,
    char *word)
{
    //Function taken and modified from BST exercise done in class.
    if (root == NULL)
    {
        root = (node *) malloc(sizeof(*root));

        if (root == NULL)
        {
            printf("Memory Error");
            return NULL;
        }
        else
        {
            root->word = calloc(strlen(word) + 1, 1);
            strncpy(root->word, word, strlen(word));
            root->left = root->right = NULL;
        }
    }
    else
    {
        if (strcasecmp(word, root->word) < 0)
        {
            root->left = Insert(root->left, word);
        }
        else if (strcasecmp(word, root->word) > 0)
        {
            root->right = Insert(root->right, word);
        }
    }
    return root;
}

void
destroy(
    node * root)
{
    //Function taken and modified from BST exercise done in class.
    if (root == NULL)           // or if (!root)
        return;

    destroy(root->left);
    destroy(root->right);
    free(root->word);
    free(root);
}

node *
Find(
    node * root,
    char *word)
{
    //Function taken and modified from BST exercise done in class.
    if (root == NULL)
    {
        return NULL;
    }
    if (strcasecmp(word, root->word) < 0)
    {
        return Find(root->left, word);
    }
    else if (strcasecmp(word, root->word) > 0)
    {
        return Find(root->right, word);
    }
    else                        //match found.
    {
        return root;
    }
}

node *
processFile(
    node * root,
    FILE * file)
{
    int numWords = 0;
    node *check = NULL;

    //char *word = '\0';
    char buffer[256] = { '\0' };
    numWords = fscanf(file, "%s", buffer);
    if (numWords == 0)
    {
        puts("EMPTY FILE!");
        destroy(root);
        return (NULL);
    }
    //Root should only be NULL if it's the first file.
    if (root == NULL)
    {
        while (!feof(file))
        {
            root = Insert(root, buffer);
            numWords = fscanf(file, "%s", buffer);
        }
        return (root);
    }

    //Any file beyond the first will be handled here.
    else
    {
        node *newroot = NULL;

        while (!feof(file))
        {
            check = Find(root, buffer);
            root = Delete(root, buffer);
            if (check != NULL)
            {
                newroot = Insert(newroot, buffer);

            }
            numWords = fscanf(file, "%s", buffer);
        }
        destroy(root);
        return (newroot);
    }
}

void
printTree(
    node * root)
{
    if (root == NULL)
    {
        return;
    }
    printTree(root->left);
    printf("%s\n", root->word);
    printTree(root->right);
}

size_t tree_height(node *t)
{
    if(t == NULL)
    {
        return(0);
    }
    if((t->left == NULL) && (t->right == NULL))
    {
        return(1);
    }
    size_t left = 1 + tree_height(t->left);
    size_t right = 1 + tree_height(t->right);
    if(left > right)
    {
        return(left);
    }
    else
    {
        return(right);
    }
}
