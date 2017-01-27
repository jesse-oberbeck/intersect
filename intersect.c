#include <stdio.h>
#include <stdlib.h>
#include "intersectlib.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        puts("Not enough arguments.");
        return(1);
    }
    node *root = NULL;
    FILE *file;
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
            fclose(file);
            exit(1);
        }
    }
    //puts("inorder time");
    inOrder(root);
    destroy(root);
}
