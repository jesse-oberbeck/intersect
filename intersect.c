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


///////////////////////////////////////////////////////////////////////////////////
node *FindMin(node *root)
{
	if(root==NULL) {return NULL; }
	else if(root->left == NULL) {return root; }
	else // found match
	{ return FindMin(root->left); }
}

node *Delete(node *root, char *word)
{
	node *temp = NULL;
    //printf("DELETE WORD: %s\n", word);
    
	if(root==NULL)
	{
	    return root;
    }
	else if(   strcasecmp(word, root->word) < 0)
	{
	    root->left = Delete(root->left, word);
    }
	else if(   strcasecmp(word, root->word) > 0)
	{
	    root->right = Delete(root->right, word);
    }
	else // found the element
	{
		if(root->left && root->right) // 2 kids
		{ 
			// replace with smallest in right subtree
			temp = FindMin(root->right); 
			strcpy(root->word, temp->word); // this just replaces the data
			// delete (or swap again) node we just got the value from
			root->right = Delete(root->right, root->word); 
		}
		else // one kid
		{ 
			temp = root;
			root = (root->left != NULL) ? root->left : root->right;
			free(temp->word);
			free(temp);
		}
	}
	//free(temp);
	return root; 
}
/////////////////////////////////////////////////////////////////////////////////////


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
		root = (node *) malloc(sizeof(*root));

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
    free(root->word);
    free(root);
} 

node * Find(node *root, char *word)
{
    //puts("\nword check: ");
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
	    //puts("match found");
	    return root;
	}
}

node *processFile(node *root, FILE *file)
{
    //char *cursor = file;
    int numWords = 0;
    node *check = NULL;
    char *word = '\0';
    char buffer[256] = {'\0'};
    numWords = fscanf(file, "%s", buffer);//strtok(cursor, " ;,.\n\t");
    if(buffer[0] == NULL)
    {
        puts("EMPTY FILE!");
        destroy(root);
        return(NULL);
    }
    //Root should only be NULL if it's the first file.
    if(root == NULL)
    {
        while(!feof(file))
        {
            //printf("!!!%s!!!\n", buffer);
            root = Insert(root, buffer);
            numWords = fscanf(file, "%s", buffer);//word = strtok(NULL, " ;,.\n\t");
        }
        return(root);
    }

    else
    {
        node *newroot = NULL;//calloc(sizeof(node *), 1);
        while(!feof(file))
        {
            check = Find(root, buffer);
            root = Delete(root, buffer);
            if(check != NULL)
            {
                //printf("!!!%s!!!\n", buffer);
                newroot = Insert(newroot, buffer);
                
            }
            numWords = fscanf(file, "%s", buffer);
        }
        destroy(root);
        //if(!newroot){puts("NO NEW ROOT");}
    return(newroot);
    }
}

void inOrder(node *root){
    if (root == NULL) // or if (!root)
    {
        //puts("NO ROOT IN PRINT");
        return;
    }
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
        //puts("NEW FILE");
        root = processFile(root, file);
        fclose(file);
        if(root == NULL)
        {
            puts("EMPTY FILE WAS GIVEN, INTERSECT IS NOTHING.");
            destroy(root);
            exit(1);
        }
    }
    //puts("inorder time");
    inOrder(root);
    destroy(root);
}
