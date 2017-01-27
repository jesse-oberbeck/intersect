struct Node{
    char *word;
    struct Node *left;
    struct Node *right;
};
typedef struct Node node;

node *FindMin(node *root);

node *Delete(node *root, char *word);

FILE* openFile(char* fileName);

node * Insert(node *root, char* word);

void destroy(node* root);

node * Find(node *root, char *word);

node *processFile(node *root, FILE *file);

void inOrder(node *root);
