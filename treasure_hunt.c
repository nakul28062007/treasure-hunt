#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int max_size = 5,current_size = 0,num_treasures;
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
treasure* create_treasure_list(int size) //this function returns a treasure type pointer
{
    treasure *treasures = (treasure *)malloc(size * sizeof(treasure));
    if (treasures==NULL)
    {
        printf("Memory Allocation Failed!.");
        exit(1);
    }
    return treasures;
}
void calculate_and_save_distance(location player_loc, treasure *ptr)
{
    ptr->distance= abs(ptr->loc.x-player_loc.x) + abs(ptr->loc.y-player_loc.y);
}
void insert_treasure(treasure **pptr, treasure newTreasure, int position, location player_loc)
{
    if ( position < 0 || position > current_size)
    {
        printf("Invalid position.\n");
        return;
    }
    if (current_size == max_size)
    {
        printf("Max Capacity Reached! -> Reallocating Memory");
        treasure *temp;
        temp = (treasure*)realloc(*pptr , (current_size+1)*sizeof(treasure));
        if (temp==NULL)
        {
            printf("Memory reallocation failed.");
            return;
        }
        *pptr=temp;
        max_size++;
    }
    for (int index = current_size ; index > position ; index--)
    {
        (*pptr)[index]=(*pptr)[index-1];
    }
    calculate_and_save_distance(player_loc, &newTreasure);
    (*pptr)[position]=newTreasure;
    current_size++;
}
void delete_treasure(treasure **treasures_ptr, int position)
{
    if (position > current_size-1 || position < 0)
    {
        printf("Invalid position");
        return;
    }


        for (int index = position ; index < current_size ; index++)
        {
            if (index==current_size-1)
            {
                break;
            }
            (*treasures_ptr)[index]= (*treasures_ptr)[index+1];
        }
        current_size--;
        if (current_size==0)
        {
            free(*treasures_ptr);
            *treasures_ptr=NULL;
        }
    else
    {
        treasure *temp = (treasure*)realloc(*treasures_ptr, current_size*sizeof(treasure));
        if (temp==NULL)
        {
            printf("Memory reallocation after deletion not successful.");
            exit(1);
        }
        *treasures_ptr=temp;
    }

}
int search_treasure(treasure *treasures, char *name)
{
    for (int i = 0 ; i < current_size ; i++)
    {
        if (!(strcmp(treasures[i].name,name))) //strcmp returns 0 if the strings are same
        {
            return i;
        }
    }
    return -1;
}
void addDetails(int x, int y ,char name[50],  treasure *array, int index)
{
    array[index].loc.x=x;
    array[index].loc.y=y;
    strcpy(array[index].name, name);
    current_size++;
}

void add_bonus_treasure(treasure bonus, treasure **pptr, int *size){
    if (*size>=max_size){
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
    treasure *treasures = create_treasure_list(num_treasures);
    printf("Treasure list created successfully.\n");
    char menu_ans = 'y';
    while (menu_ans=='y')
    {
        printf("\t\t\t\t\tTreaaure Hunt Game\n");
        printf("\t\t\t\t\t******************\n");
        printf("1. Add Details.\n");
        printf("2. Add Bonus treasure.\n");
        printf("3. Transform player Location.\n");
        printf("4. Insert new treasure.\n");
        printf("5. Delete a treasure.\n");
        printf("6. Search the treasure.\n");
        printf("7. Display All treasures.\n");
        int choice;
        do
        {
            printf("Enter your choice?: \n");
            scanf("%d",&choice);
        }while (choice < 1 || choice > 7);
        if (choice == 1)
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
        }
        else if (choice == 2)
        {

                printf("Enter Name of Bonus treasure: \n");
                scanf("%s",name);
                printf("Enter the Value for x for Bonus Treasure: ");
                scanf("%d",&x);
                printf("Enter the Value for y for Bonus Treasure: ");
                scanf("%d",&y);
                bonus.loc.x=x;
                bonus.loc.y=y;
                calculate_and_save_distance(player_loc,&bonus);
                strcpy(bonus.name,name);
                add_bonus_treasure(bonus,&treasures,&current_size);

        }
        else if (choice == 3)
        {
            int transform[2][2];
            transform_player_location(transform,&player_loc);
            for (int i = 0 ; i<current_size ; i++)
            {
                calculate_and_save_distance(player_loc,&treasures[i]);
            }
        }
        else if (choice ==4)
        {
            treasure newTreasure;
            int new_x, new_y,position;
            printf("Enter Values for New Treasure: \n");
            do
            {
                printf("Enter the Value for x for New Treasure: \n");
                scanf("%d",&new_x);
            }while (new_x<0 || new_x>9);

            do
            {
                printf("Enter the Value for x for New Treasure: \n");
                scanf("%d",&new_y);

            }while (new_y<0 || new_y>9);

            printf("Enter the Value for Name for New Treasure: \n");
            scanf("%s",name);

            newTreasure.loc.x=new_x;
            newTreasure.loc.y=new_y;
            strcpy(newTreasure.name,name);

            printf("Enter the position that you want to Insert?: \n");
            scanf("%d",&position);

            insert_treasure(&treasures, newTreasure , position,player_loc);
        }
        else if (choice == 5)
        {
            int delPos=0;
            do
            {
                printf("Enter Position of treasure that you want to delete?: \n");
                scanf("%d",&delPos);
            }while (delPos<0 || delPos>current_size-1);
            delete_treasure(&treasures,delPos);
        }
        else if (choice == 6)
        {
            char findname[50];
            printf("Enter the Name of the treasure that you want to search: \n");
            scanf("%s",findname);
            int ans = search_treasure(treasures, findname);
            if (ans==-1)
            {
                printf("treasure not found!.\n");
            }
            else
            {
                printf("Treasure found at: %d\n",ans);
            }
        }
        else if (choice == 7)
        {
            printDetails(treasures);
        }
        else
        {
            break;
        }
        printf("Do you want to continue? y/n : \n");
        scanf(" %c",&menu_ans);
        if (menu_ans!='y')
        {
            printf("Code Exited.\n");
            break;
        }
    }
    free(treasures);
    return 0;
}






