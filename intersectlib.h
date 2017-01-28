struct Node
{
    char *word;
    struct Node *left;
    struct Node *right;
};
typedef struct Node node;

node *FindMin(
    node * root);

node *Delete(
    node * root,
    char *word);

FILE *openFile(
    char *fileName);

node *Insert(
    node * root,
    char *word);

void destroy(
    node * root);

node *Find(
    node * root,
    char *word);

node *processFile(
    node * root,
    FILE * file);

void printTree(
    node * root);
    
size_t tree_height(node *t);

/////////////////https://gist.github.com/tonious/1377768

void avl_balance( node *tree );

node *avl_balance_node( node *n );

node *avl_rotate_rightright( node *n );

node *avl_rotate_rightleft( node *n );

node *avl_rotate_leftright( node *n );

node *avl_rotate_leftleft( node *n );

int avl_balance_factor( node *node );
