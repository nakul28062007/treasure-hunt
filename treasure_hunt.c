#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Location
{
    int x;
    int y;
};
struct Treasure
{
    char name[50];
    int distance;
    struct Location loc;
};

void taxicabDistance(int start_X, int start_Y , struct Treasure array[]){
    for (int k = 0 ; k<5; k++)
    {
        int distance=abs(start_X-array[k].loc.x) + abs(start_Y-array[k].loc.y);
        array[k].distance=distance;
    }
}

void addDetails(int x, int y ,char name[50], struct Treasure array[], int index)
{
    array[index].loc.x=x;
    array[index].loc.y=y;
    strcpy(array[index].name, name);
}

void printDetails(struct Treasure array[])
{
    struct Treasure minTreasure;
    int min_distance=array[0].distance;//assume first treasure has least distance
    minTreasure.distance=min_distance;
    strcpy(minTreasure.name, array[0].name);
    minTreasure.loc=array[0].loc;

    for (int j= 1 ; j<5; j++)
    {
        if (array[j].distance < min_distance)
        {
            min_distance=array[j].distance;
            minTreasure.distance=min_distance;
            strcpy(minTreasure.name, array[j].name);
            minTreasure.loc = array[j].loc;
        }
    }
    for ( int m = 0 ; m<5; m++)
    {
        printf("\n");
        printf("Treasure %d: \n",m+1);
        printf("name: %s\n",array[m].name);
        printf("location is (%d,%d)\n",array[m].loc.x,array[m].loc.y);
        printf("distance: %d\n",array[m].distance);
    }
    printf("\nNearest Treasure is: \n");
    printf("name: %s\n",minTreasure.name);
    printf("location is (%d,%d)\n",minTreasure.loc.x,minTreasure.loc.y);
    printf("distance: %d\n",minTreasure.distance);
}

int main()
{
    int x=0,y=0,startX,startY;
    char name[50];
    struct Treasure array[5];
    struct Treasure minTreasure;

    for (int i = 0 ; i<5 ; i++)
    {
        do
        {
            printf("Enter the Value for x for Treasure %d: ",i+1);
            scanf("%d",&x);
        }while (x<0 || x>9);

        do
        {
            printf("Enter the Value for y for Treasure %d: ",i+1);
            scanf("%d",&y);

        }while (y<0 || y>9);

        printf("Enter the Value for Named for Treasure %d: ",i+1);
        scanf("%s",name);
        addDetails(x,y,name,array,i);
    }
    printf("Enter Your Current positon X: \n");
    scanf("%d", &startX);
    printf("Enter Your Current positon Y: \n");
    scanf("%d", &startY);
    taxicabDistance(startX,startY,array);
    printDetails(array);
    return 0;
}