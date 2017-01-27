#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "intersectlib.h"


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
