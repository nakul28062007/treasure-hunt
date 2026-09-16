#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int current_size = 0;
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
struct Node
{
    treasure treasure;
    struct Node *next;
    struct Node *prev;
};
struct Node *head=NULL,*tail=NULL;
struct Node* create_node(treasure t)
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    if (node==NULL)
    {
        printf("memory allocation failed!.\n");
        exit(1);
    }
    current_size++;
    node->treasure=t;
    node->next=NULL;
    node->prev=NULL;
    return node;
}
void calculate_and_save_distance(location *player_loc, int position)
{
    int count = 0;
    struct Node *temp = head;
    while (temp!=NULL)
    {
        if (count==position)
        {
            temp->treasure.distance = abs(player_loc->x-temp->treasure.loc.x) + abs(player_loc->y-temp->treasure.loc.y);
            break;
        }
        count++;
        temp = temp->next;
    }

}
void insert_at_beg(treasure t)
{
    struct Node *newNode = create_node(t);
    if (head==NULL)
    {
        head=tail=newNode;
        return;
    }
    head->prev = newNode;
    newNode->next=head;
    head = newNode;
}
void insert_at_end(treasure t)
{
    struct Node *newNode = create_node(t);
    tail->next=newNode;
    newNode->prev=tail;
    tail=newNode;

}
int insert_at_pos(treasure t, int pos)
{
    if (pos > current_size)
    {
        printf("Position greater than last index!.\n");
        if (head==NULL)
        {
            printf("List is empty. Adding to index 0.\n");
            insert_at_beg(t);
            return pos;
        }
        printf("DO you want to Add to last index?\n");
        char ans;
        scanf(" %c",&ans);
        if (ans!='y') return -1;
        insert_at_end(t);
        return current_size-1;
    }
    if (pos < 0)
    {
        printf("Invalid position.");
        return -1;
    }
    if (head==NULL && tail==NULL)
    {
        printf("List is empty. Adding treasure to index 0.\n");
        struct Node *newNode = create_node(t);
        head = tail = newNode;
        return current_size-1;
    }
    if (pos==0)
    {
        insert_at_beg(t);
        return pos;
    }
    struct Node *newNode = create_node(t);
    int index_count = 0;
    struct Node *iterator=head;
    while (iterator!=NULL)
    {
        if (index_count==pos-1)
        {
            struct Node *old_next = iterator->next;   // capture BEFORE overwriting
            newNode->prev=iterator;
            newNode->next=iterator->next;
            iterator->next=newNode;
            if (old_next != NULL)
                old_next->prev = newNode;
            else
                tail = newNode;
            return index_count;
        }
        index_count++;
        iterator=iterator->next;
    }
    return -1;
}
void delete_from_beg()
{
    if (current_size==1)
    {
        printf("List size 1. Deleting list.");
        free(head);
        head=NULL;
        tail=NULL;
        current_size--;
        return;
    }
    struct Node *temp = head;
    head = temp->next;
    head->prev=NULL;
    current_size--;
    free(temp);
}
void delete_from_end()
{
    if (current_size==1)
    {
        printf("List size 1. Deleting list.");
        free(head);
        head=NULL;
        tail = NULL;
        current_size--;
        return;
    }
    struct Node *temp = tail;
    tail = temp->prev;
    temp->prev->next=NULL;
    current_size--;
    free(temp);
}
void delete_from_pos(int pos)
{
    if (head==NULL)
    {
        printf("List is empty.");
        return;
    }
    if (pos==0)
    {
        delete_from_beg();
        return;
    }
    if (pos==current_size-1)
    {
        delete_from_end();
        return;
    }
    struct Node *temp_from_head = head;
    struct Node *temp_from_tail = tail;
    int head_counter = 0;
    int tail_counter = current_size-1;
    while (temp_from_head!=NULL || temp_from_tail !=NULL)
    {
        if (head_counter > tail_counter) break;
        if (head_counter==pos)
        {
            temp_from_head->prev->next = temp_from_head->next;
            temp_from_head->next->prev = temp_from_head->prev;
            current_size--;
            free(temp_from_head);
            return;
        }
        if (tail_counter==pos)
        {
            temp_from_tail->prev->next = temp_from_tail->next;
            temp_from_tail->next->prev = temp_from_tail->prev;
            current_size--;
            free(temp_from_tail);
            return;
        }
        head_counter++;
        tail_counter--;
        temp_from_head = temp_from_head->next;
        temp_from_tail= temp_from_tail->prev;
    }
}
void add_bonus_treasure(treasure bonus_treasure){
    if (head==NULL)
    {
        printf("List is empty. Adding bonus treasure to index 0.\n");
        insert_at_beg(bonus_treasure);
        return;
    }
    insert_at_end(bonus_treasure);
}
void transform_player_location(int matrix[2][2], location *player_loc)
{
    struct Node *temp=head;
    int position = 0;
    while (temp!=NULL)
    {
        location old_player_loc = temp->treasure.loc;
        int vector[2]={old_player_loc.x, old_player_loc.y};
        int result[2]={0,0};
        for (int m = 0 ; m<2 ; m++)
        {
            for (int n = 0 ; n<2 ; n++)
            {
                result[m]+=matrix[m][n]*vector[n];
            }
        }

        temp->treasure.loc.x = result[0];
        temp->treasure.loc.y = result[1];
        calculate_and_save_distance(player_loc,position);
        position++;
        temp=temp->next;
    }
};
int search_treasure(char *name)
{
    if (head==NULL)
    {
        return -1;
    }
    struct Node *iterator_from_head = head;
    int index_counter_from_head= 0;
    while (iterator_from_head!=NULL)
    {
        if (!(strcmp(iterator_from_head->treasure.name,name)))
        {
            return index_counter_from_head;
        }
        iterator_from_head=iterator_from_head->next;
        index_counter_from_head++;
    }
    return -1;
}
void printDetails()
{
    if (head==NULL)
    {
        printf("List is empty.\n");
        return;
    }
    struct Node *temp = head;
    while (temp!=NULL)
    {
        printf("Treasure Nme: %s\n",temp->treasure.name);
        printf("treasure coordinates: (%d,%d)\n",temp->treasure.loc.x,temp->treasure.loc.y);
        printf("Distamce from you: %d\n",temp->treasure.distance);
        temp=temp->next;
    }

}
int main()
{
    int x=0,y=0;
    char name[50];
    location player_loc; //to store player location.
    char menu_ans = 'y';
    int i = 0; //to track the count of treasure.
    while (menu_ans=='y')
    {
        printf("\t\t\t\t\tTreasure Hunt Game\n");
        printf("\t\t\t\t\t******************\n");
        printf("1. Add Treasure.\n");
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
        if (choice==1)
        {
            char ch1 = 'y';
            while (ch1=='y')
            {
                i++;
                do
                {
                    printf("Enter the Value for x for Treasure %d: ",i);
                    scanf("%d",&x);
                }while (x<0 || x>9);
                do
                {
                    printf("Enter the Value for y for Treasure %d: ",i);
                    scanf("%d",&y);
                }while (y<0 || y>9);
                printf("Enter the Value for Name for Treasure %d: ",i);
                scanf("%s",name);
                location loc={x,y};
                treasure new_treasure;
                strcpy(new_treasure.name,name);
                new_treasure.loc=loc;
                insert_at_pos(new_treasure,current_size);
                printf("Do you wish to add more treasures? y/n: ");
                scanf(" %c",&ch1);
                if (ch1!='y')
                {
                    break;
                }
            }
            printf("Enter Your Current positon X: \n");
            scanf("%d", &player_loc.x);
            printf("Enter Your Current positon Y: \n");
            scanf("%d", &player_loc.y);
            for (int j = 0; j<=current_size-1 ; j++) calculate_and_save_distance(&player_loc, j);
        }
        else if (choice==2)
        {
            treasure bonus_treasure;
            printf("Enter Name of Bonus treasure: \n");
            scanf("%s",name);
            printf("Enter the Value for x for Bonus Treasure: ");
            scanf("%d",&x);
            printf("Enter the Value for y for Bonus Treasure: ");
            scanf("%d",&y);
            bonus_treasure.loc.x=x;
            bonus_treasure.loc.y=y;
            strcpy(bonus_treasure.name,name);
            add_bonus_treasure(bonus_treasure);
            calculate_and_save_distance(&player_loc,current_size-1);
        }
        else if (choice ==3)
        {
            int transform[2][2];
            for (int i = 0 ; i<2  ;i++)
            {
                for (int j = 0 ; j<2 ; j++)
                {
                    printf("Enter the value of matrix at (%d,%d): ",i,j);
                    scanf("%d",&transform[i][j]);
                }
            }
            transform_player_location(transform, &player_loc);
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
                printf("Enter the Value for y for New Treasure: \n");
                scanf("%d",&new_y);

            }while (new_y<0 || new_y>9);

            printf("Enter the Value for Name for New Treasure: \n");
            scanf("%s",name);

            newTreasure.loc.x=new_x;
            newTreasure.loc.y=new_y;
            strcpy(newTreasure.name,name);

            printf("Enter the position that you want to Insert?: \n");
            scanf("%d",&position);
            int actual_position = insert_at_pos(newTreasure, position);
            if (actual_position != -1) calculate_and_save_distance(&player_loc, actual_position);            }
        else if (choice == 5){
            int delPos=0;
            do
            {
                printf("Enter Position of treasure that you want to delete?: \n");
                scanf("%d",&delPos);
            }while (delPos<0 || delPos>current_size-1);
            delete_from_pos(delPos);

        }
        else if (choice == 6)
        {
            char findname[50];
            printf("Enter the Name of the treasure that you want to search: \n");
            scanf("%s",findname);
            int ans = search_treasure(findname);
            if (ans==-1)
            {
                printf("treasure not found!.\n");
            }
            else
            {
                printf("Treasure found at: %d\n",ans);
            }
        }
        else if (choice==7)
        {
            printDetails();
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
}