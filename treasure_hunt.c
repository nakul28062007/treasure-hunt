#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CAPACITY 10 //Macros are used to define constants

typedef struct Location
{
    int x;
    int y;
}location;

typedef struct Treasure
{
    char name[50];
    int distance;
    struct Location loc;
}treasure;

treasure storage[MAX_CAPACITY];
treasure *treasures= &storage[0];

int current_size = 0,num_treasures;

void addDetails(int x, int y ,char name[50],  treasure *array, int index)
{
    array[index].loc.x=x;
    array[index].loc.y=y;
    strcpy(array[index].name, name);
    current_size++;
}

void calculate_and_save_distance(location player_loc, treasure *ptr)
{
   ptr->distance= abs(ptr->loc.x-player_loc.x) + abs(ptr->loc.y-player_loc.y);
}



void add_bonus_treasure(treasure bonus, treasure *array, int *size)
{
    if (*size>=MAX_CAPACITY)
    {
        printf("Map capacity full!");
        return;
    }
    array[(*size)]=bonus;
    (*size)++;

}


void get_nearest_treasure(treasure *array, int size, treasure **closest_treasure)
{
    *closest_treasure = &array[0];
    int min_distance = array[0].distance;
    (*closest_treasure)->distance=min_distance;
    for (int j= 1 ; j<size; j++)
    {
        if (array[j].distance < min_distance)
        {
            min_distance=array[j].distance;
           *closest_treasure=&array[j];
        }
    }
    printf("\nNearest Treasure is: %s\n",(*closest_treasure)->name);
    printf("distance: %d occurs at (%d,%d).\n",(*closest_treasure)->distance,(*closest_treasure)->loc.x,(*closest_treasure)->loc.y);
}



void printDetails(treasure *array)
{
    treasure minTreasure;
    int min_distance=array[0].distance;//assume first treasure has least distance
    minTreasure.distance=min_distance;
    strcpy(minTreasure.name, array[0].name);
    minTreasure.loc=array[0].loc;

    for (int j= 1 ; j<current_size; j++)
    {
        if (array[j].distance < min_distance)
        {
            min_distance=array[j].distance;
            minTreasure.distance=min_distance;
            strcpy(minTreasure.name, array[j].name);
            minTreasure.loc = array[j].loc;
        }
    }
    for ( int m = 0 ; m<current_size; m++)
    {
        printf("\n");
        printf("Treasure %d: \n",m+1);
        printf("name: %s\n",array[m].name);
        printf("location is (%d,%d)\n",array[m].loc.x,array[m].loc.y);
        printf("distance: %d\n",array[m].distance);
    }
    get_nearest_treasure(treasures, current_size, &treasures);
}

int main()
{

    int x=0,y=0,startX,startY;
    char name[50];

    location player_loc;
    treasure bonus;

    printf("Enter the Number of treasures: ");
    scanf("%d",&num_treasures);
    if (num_treasures<=5)
    {
        for (int i = 0 ; i<num_treasures; i++)
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

            printf("Enter the Value for Name for Treasure %d: ",i+1);
            scanf("%s",name);
            addDetails(x,y,name,treasures,i);
        }

        printf("Enter Your Current positon X: \n");
        scanf("%d", &player_loc.x);
        printf("Enter Your Current positon Y: \n");
        scanf("%d", &player_loc.y);
        for (int i = 0; i < num_treasures; i++) {
            calculate_and_save_distance(player_loc, &treasures[i]);
        }
        printDetails(treasures);

        char ans='y';
        printf("Do you want to add bonus treasure? :y/n: ");
        scanf(" %c",&ans);
        while (ans=='y')//for adding bonus treasures
        {
            printf("Enter Name of Bonus treasure: \n");
            scanf("%s",name);
            printf("Enter the Value for x for Bonus Treasure: ");
            scanf("%d",&x);
            printf("Enter the Value for y for Bonus Treasure: ");
            scanf("%d",&y);
            printf("Do you want to add bonus treasure? :y/n: ");
            scanf(" %c",&ans);
            bonus.loc.x=x;
            bonus.loc.y=y;
            calculate_and_save_distance(player_loc,&bonus);
            strcpy(bonus.name,name);
            add_bonus_treasure(bonus,treasures,&current_size);
            if (ans!='y')
            {
                break;
            }
        }
        printDetails(treasures);
    }
    else
    {
        printf("Number of treasures must be from 1 to 5 only.");
    }

    return 0;
}






