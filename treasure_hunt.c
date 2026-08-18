#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CAPACITY 10

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



void add_bonus_treasure(treasure bonus, treasure **pptr, int *size){
    if (*size>=MAX_CAPACITY){
        printf("Map capacity full!");
        return;
    }
  treasure *temp = (treasure*)realloc(*pptr,(*size +1)*sizeof(treasure));
    if (temp==NULL)
    {
        printf("Memory Allocation Failed!\n");
        return;
    }
    *pptr=temp;
    (*pptr)[*size]=bonus;
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
void transform_player_location(int matrix[2][2], location *player_location)
{
    for (int i = 0 ; i<2  ;i++)
    {
        for (int j = 0 ; j<2 ; j++)
        {
            printf("Enter the value of matrix at (%d,%d): ",i,j);
            scanf("%d",&matrix[i][j]);
        }
    }

    int vector[2]={player_location->x, player_location->y};
    int result[2]={0,0};
    for (int m = 0 ; m<2 ; m++)
    {
        for (int n = 0 ; n<2 ; n++)
        {
            result[m]+=matrix[m][n]*vector[n];
        }
    }

    player_location->x=result[0];
    player_location->y=result[1];

};



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
    get_nearest_treasure(array, current_size, &array);
}

int main()
{

    int x=0,y=0;
    char name[50];

    location player_loc;
    treasure bonus;

    printf("Enter the Number of treasures: ");
    scanf("%d",&num_treasures);
    treasure *treasures= (treasure*)malloc(num_treasures*sizeof(treasure));
    if (treasures==NULL)
    {
        printf("Memory Allocation Failed!");
        exit(1);
    }

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
            add_bonus_treasure(bonus,&treasures,&current_size);
            if (ans!='y')
            {
                break;
            }
        }
        printDetails(treasures);

    int transform[2][2];
    transform_player_location(transform,&player_loc);
    for (int i = 0 ; i<current_size ; i++)
    {
        calculate_and_save_distance(player_loc,&treasures[i]);
    }

    printf("\nUpdated Distances after transform: \n");
    printDetails(treasures);

    free(treasures);

    return 0;
}






