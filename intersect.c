#include <stdio.h>
#include <stdlib.h>
#include "intersectlib.h"

int
main(
    int argc,
    char **argv)
{
    //Check for sufficient arguments/files.
    if (argc < 2)
    {
        puts("Not enough arguments.");
        return (1);
    }
    node *root = NULL;
    FILE *file;

    //Loop through every file in argv. Checks for empty files,
    //calling the required functions to handle each.
    for (int i = 1; i < argc; ++i)
    {
        file = openFile(argv[i]);
        if (!file)
        {
            printf("No file.\n");
            return (1);
        }
        root = processFile(root, file);
        fclose(file);
        if (root == NULL)
        {
            puts("EMPTY FILE WAS GIVEN, INTERSECT IS NOTHING.");
            destroy(root);
            fclose(file);
            exit(1);
        }
    }
    avl_balance(root);
    printTree(root);
    printf("height: %zd\n",tree_height(root));
    destroy(root);
}
