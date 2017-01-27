#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
//#include "BST.c" //Pre-existing work from class/instructor.

//open file handle, strtok through file. for each word, use
//strcasecmp to decide where the word goes in the tree

//once the tree is made, move to next file. open new pointer.
//tokenize, going over each word, and do the same string compare

//adjust insert to skip the word if the comparison returns 0.


struct Node{
    char *word;
    struct Node *left;
    struct Node *right;
};
typedef struct Node node;

FILE* openFile(char* fileName)
{
    
    char file[255];
    char *path = getenv("HOME");
    sprintf(file, "%s%s", path, fileName);
    //printf("%s\n", fileName);
    return(fopen(fileName, "r"));
}


node * Insert(node *root, char* word)
{
	if(root==NULL) 
	{
		root = (node *) malloc(sizeof(node));

		if(root == NULL)
		{
			printf("Memory Error");
			return NULL; 
		}
		else
		{
		    root->word = calloc(strlen(word) + 1, 1);
			strcpy(root->word, word);
			root->left = root->right = NULL;
		}
	}
	else
	{
		if(   strcasecmp(word, root->word) < 0)
		{
		    root->left = Insert(root->left, word);
		}
		else if(   strcasecmp(word, root->word) > 0)
		{
		    root->right = Insert(root->right, word);
		}
	}
	return root; 
}


void destroy(node* root){
     if (root == NULL) // or if (!root)
          return;
       
     destroy(root->left);  
     destroy(root->right);

     free(root);
} 

node * Find(node *root, char *word)
{
	if(root==NULL) {return NULL; }
	if(   strcasecmp(word, root->word) < 0)
	{
	    return Find(root->left, word);
	}
	else if(   strcasecmp(word, root->word) > 0)
	{
	    return Find(root->right, word);
	}
	else // found match
	{
	    puts("match found");
	    return NULL;
	}
}

node *processFile(node *root, FILE *file)
{
    //char *cursor = file;
    int numWords = 0;
    node *check = NULL;
    char *word = '\0';
    char buffer[256];
    numWords = fscanf(file, "%s", buffer);//strtok(cursor, " ;,.\n\t");

    //Root should only be NULL if it's the first file.
    if(root == NULL)
    {
        while(!feof(file))
        {
            printf("!!!%s!!!\n", buffer);
            root = Insert(root, buffer);
            numWords = fscanf(file, "%s", buffer);//word = strtok(NULL, " ;,.\n\t");
        }
        return(root);
    }

    else
    {
        node *newroot = calloc(sizeof(node *), 1);
        while(!feof(file))
        {
            check = Find(root, buffer);
            if(check != root)
            {
                printf("!!!%s!!!\n", buffer);
                root = Insert(newroot, buffer);
                
            }
            numWords = fscanf(file, "%s", buffer);
        }
        destroy(root);
    return(newroot);
    }
}//CURRENT ISSUE: it looks like newroot is not being passed correctly to Insert. Segfaults on line 54, the first compare. It's doing that thing where it's not getting a good copy of the struct again.

void inOrder(node *root){
    puts("loop");
     if (root == NULL) // or if (!root)
          return;
    inOrder(root->left);
    printf("%s\n", root->word);
    inOrder(root->right);
}



int main(int argc, char **argv)
{
    if(argc < 2)
    {
        puts("Not enough arguments.");
        return(1);
    }
    node *root = NULL;
    FILE *file;
    int numFiles = argc;
    for(int i = 1; i < argc; ++i)
    {
        file = openFile(argv[i]);
        if(!file)
        {
            printf("No file.\n");
            return(1);
        }
        puts("NEW FILE");
        root = processFile(root, file);
    }
    puts("inorder time");
    inOrder(root);
}