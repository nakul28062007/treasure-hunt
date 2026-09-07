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
    };
    struct Node *head=NULL;
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
        return node;
    }
    void insert_at_end(treasure t)
    {
        struct Node *newNode = create_node(t);
       if (head==NULL)
       {
           head =newNode;
           return;
       }
        struct Node *temp= head;
        while (temp->next!=NULL)
        {
            temp=temp->next;
        }
        temp->next=newNode;
    }
    void add_bonus_treasure(treasure bonus_treasure){
        insert_at_end(bonus_treasure);
     }
    void transform_player_location(int matrix[2][2])
    {
        struct Node *temp=head;
        int position = 0;
        while (temp!=NULL)
        {
            location player_loc = temp->treasure.loc;
            int vector[2]={player_loc.x, player_loc.y};
            int result[2]={0,0};
            for (int m = 0 ; m<2 ; m++)
            {
                for (int n = 0 ; n<2 ; n++)
                {
                    result[m]+=matrix[m][n]*vector[n];
                }
            }

            player_loc.x=result[0];
            player_loc.y=result[1];
            calculate_and_save_distance(&player_loc,position);
            position++;
            temp=temp->next;
        }
    };
    int insert_treasure(treasure newTreasure, int position)
    {
        if (position > current_size)
        {
            printf("Position greater than last index!. DO you want to insert in the end?");
            char ans;
            scanf(" %c",&ans);
            if (ans!='y') return -1;
            insert_at_end(newTreasure);
            return current_size-1;
        }
        if (position < 0)
        {
            printf("Invalid position.");
            return -1;
        }
        if (head==NULL)
        {
            printf("List is emty. Adding treasure to index 0.");
            insert_at_end(newTreasure);
            return current_size-1;
        }
        struct Node *newNode=create_node(newTreasure);
        if (position==0)
        {
            newNode->next= head;
            head=newNode;
            return position;
        }
        int index_count = 0;
        struct Node *iterator=head;
        while (iterator!=NULL)
        {
            if (index_count==position-1)
            {
                newNode->next=iterator->next;
                iterator->next=newNode;
                return position;
            }
            index_count++;
            iterator=iterator->next;
        }
        return -1;
    }
    void delete_treasure(int position)
    {
        int index_count = 0;
        struct Node *iterator = head,*temp;
        if (position==0)
        {
            struct Node *toDelete=head;
            head=head->next;
            current_size--;
            free(toDelete);
            return;
        }
        while (iterator!=NULL)
        {
            if (index_count==position-1)
            {
                temp = iterator;
            }
            if (index_count==position)
            {
                temp->next=iterator->next;
                free(iterator);
                current_size--;
                break;
            }
            index_count++;
            iterator=iterator->next;
        }
    }
    int search_treasure(char *name)
    {
        struct Node *iterator = head;
        int index_count= 0;
        while (iterator!=NULL)
        {
            if (!(strcmp(iterator->treasure.name,name))) //strcmp returns 0 if the strings are same
            {
                return index_count;
            }
            index_count++;
            iterator=iterator->next;
        }
        return -1;
    }
    void get_nearest_treasure()
    {
        if (current_size==0 || head == NULL)
        {
            printf("list is empty. Cannout GET Nearest treasure.");
            return;
        }
        struct Node *iterator = head,*temp;
        int min_distance = iterator->treasure.distance; //assume
        int index_count = 0;
        while (iterator!=NULL)
        {
            if (iterator->treasure.distance<=min_distance) temp=iterator;
            iterator=iterator->next;
        }
        printf("\nNearest Treasure is: %s\n",temp->treasure.name);
        printf("distance: %d occurs at (%d,%d).\n",temp->treasure.distance,temp->treasure.loc.x,temp->treasure.loc.y);
    }
    void printDetails()
    {
        struct Node *iterator = head;
        while (iterator!=NULL)
        {
            printf("Treasure Name: %s\n",iterator->treasure.name);
            printf("Treasure location is (%d,%d)\n",iterator->treasure.loc.x,iterator->treasure.loc.y);
            printf("distance: %d\n",iterator->treasure.distance);
            printf("\n");
            iterator=iterator->next;
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
                    insert_at_end(new_treasure);
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
                transform_player_location(transform);
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
                int actual_position = insert_treasure(newTreasure, position);
                if (actual_position != -1) calculate_and_save_distance(&player_loc, actual_position);            }
            else if (choice == 5){
                int delPos=0;
                do
                {
                    printf("Enter Position of treasure that you want to delete?: \n");
                    scanf("%d",&delPos);
                }while (delPos<0 || delPos>current_size-1);
                delete_treasure(delPos);

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
            else if (choice == 7)
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


