/*Iterative version*/

#include <sys/types.h>

#include <dirent.h>

#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>

#include <stdio.h>

#include <stdbool.h>

struct node *begNode;

struct node

{

    struct node *nextNode; 

    int sizeFile; 

    char nameofFile[1024]; 

};



struct node *OpenList = NULL;

struct node *ClosedList = NULL;

void addNode(struct node *insert, char LinkedList)

{

   // struct node *newNode = malloc(sizeof(struct node));

   if(LinkedList == 'd')

   {    

        struct node *pNode = OpenList;

        if(OpenList == NULL)

    {

            OpenList = insert;

    }



        else

        {

            while(pNode -> nextNode != NULL)

            {

                pNode = pNode -> nextNode;

            }

            pNode -> nextNode = insert;

        }



   } 



   if(LinkedList == 'f')

   {

        struct node *pNode = ClosedList;

    if(ClosedList == NULL)

    {

            ClosedList = insert;

    }



        else

        {

            while(pNode -> nextNode != NULL)

            {

                pNode = pNode -> nextNode;

            }

            pNode -> nextNode = insert;

        }



   }

   

}



int isValidFile(char *path) // making sure i can actually access the path

{

    bool isValid;

    bool notValid;

    FILE *checker = fopen(path, "r");

    if (checker)

    {

        fclose(checker);

        isValid = true;

        return isValid;

    }

    else

    {

        notValid = false;

        return notValid;

    }

}



int LLcount(struct node *beginning)

{

    struct node *recent = beginning;

    if (recent == NULL)

        return 0;

    else

    { int count = 0;

        while (recent != NULL)

        {

            count++;

            recent = recent->nextNode;

        }

        return count;

    }

}



void printLL(struct node *beginning)

{

   while (beginning != NULL)

    {

        printf("%d\t%s\n",(beginning->sizeFile), (beginning->nameofFile));

        beginning = beginning->nextNode;

    }  

}



void swap(struct node* firstP, struct node* secondP)

{

    struct node* temsecP;

    char tempFile[1024];

    strcpy(tempFile, secondP->nameofFile);

    strcpy(secondP->nameofFile, firstP->nameofFile);

    strcpy(firstP->nameofFile,tempFile);



    int temp = firstP->sizeFile; 

    firstP->sizeFile = secondP->sizeFile; 

    secondP->sizeFile = temp; 



}

void sort(struct node *start) 

{ 



    

    int signal, i; 

    struct node *firstP; 

    struct node *leftFirstP = NULL; 

  

    if (start == NULL) 

        return; 

  

    do

    { 

        signal = 0; 

        firstP = start; 

  

        while (firstP->nextNode != leftFirstP) 

        { 

            if (firstP->sizeFile > firstP->nextNode->sizeFile) 

            { 

                swap(firstP, firstP->nextNode); 

                signal = 1; 

            } 

            firstP = firstP->nextNode; 

        } 

        leftFirstP = firstP; 

    } 

    while (signal); 

} 



void construct(char *filename, struct stat statbuf, char traverseNode)

{    struct node *current; //node current points to begNode

    struct node *node = (struct node *)malloc(sizeof(struct node));

    node->sizeFile = statbuf.st_size;

    strcpy(node->nameofFile, filename);

    addNode(node, traverseNode);

    //sort(node);

}







void traverse(char* nameOfPath)

{

    DIR *pDir;

    int res;

    struct stat statbuf;

    mode_t mode;

    struct dirent * pEnt;

    char fileName[1024];

    res = stat(nameOfPath, &statbuf);

    bool first = true;

    mode = statbuf.st_mode; 

    if(isValidFile(nameOfPath))

    { 

        while(OpenList != NULL || first == true)

        {

            

            if(first == true)

            {

                pDir = opendir(nameOfPath);

                //first = false;

            }

            else

            {

                pDir = opendir(OpenList ->nameofFile);

                nameOfPath = OpenList -> nameofFile;

            }

            while((pEnt = readdir(pDir)) != NULL)

            {

                if (strcmp(pEnt->d_name, ".")!=0 && strcmp(pEnt->d_name, "..")!=0)

                {

                   

                    strcpy(fileName, nameOfPath);

                    strcat(fileName, "/");

                    strcat(fileName, pEnt->d_name);

                    res = stat(fileName, &statbuf); 

                    mode = statbuf.st_mode; 

                    //first = false;

                    if(S_ISREG(mode))

                {

                    construct(fileName, statbuf, 'f');

                }     

                else if(S_ISDIR(mode))   

                {

                    construct(fileName, statbuf, 'd');



                } 

                }

            

                   

            }

            closedir(pDir);

            if(OpenList != NULL && first != true)

            OpenList = OpenList ->nextNode; 

        

            first = false;



        }



    }

}



int main(int argc, char *argv[])

{   

    traverse(argv[1]);

    sort(ClosedList);

    printLL(ClosedList);

    return 0;

}
