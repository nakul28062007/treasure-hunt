#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//defnitions
#define NUM_TREASURES 5
#define GRID_SIZE 10
#define NAME_SIZE 50

//data structures
typedef struct Location
{
    int x;
    int y;
} location;


typedef struct Treasure
{
    char name[NAME_SIZE];
    int distance;
    location loc;
} treasure;

//linked-list
typedef struct Node
{
    location player_loc;
    bool collected;
    struct Node *next;
} Node;


//global stacks
treasure treasure_stack[NUM_TREASURES];
int treasure_top = -1;


/* Backup of the original treasures.
   Used by RESET to reconstruct the treasure stack. */
treasure original_treasures[NUM_TREASURES];


/* Array stack containing collected treasures */
treasure collected_treasures[NUM_TREASURES];
int collected_top = -1;
Node *position_top = NULL;
location player_location;
bool game_started = false;

Node *create_node(location player_loc, bool collected)
{
    Node *node = (Node *)malloc(sizeof(Node));

    if (node == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    node->player_loc = player_loc;
    node->collected = collected;
    node->next = NULL;

    return node;
}
void push_player_position(location player_loc, bool collected)
{
    Node *new_node = create_node(player_loc, collected);

    new_node->next = position_top;
    position_top = new_node;
}
bool pop_player_position(location *player_loc, bool *collected)
{
    if (position_top == NULL)
    {
        return false;
    }

    Node *temp = position_top;

    *player_loc = temp->player_loc;
    *collected = temp->collected;

    position_top = position_top->next;

    free(temp);

    return true;
}
bool peek_player_position(location *player_loc)
{
    if (position_top == NULL)
    {
        return false;
    }

    *player_loc = position_top->player_loc;

    return true;
}
void clear_position_stack(void)
{
    Node *temp;

    while (position_top != NULL)
    {
        temp = position_top;
        position_top = position_top->next;
        free(temp);
    }
}
bool is_treasure_stack_empty(void)
{
    return treasure_top == -1;
}
bool is_treasure_stack_full(void)
{
    return treasure_top == NUM_TREASURES - 1;
}
bool push_treasure(treasure t)
{
    if (is_treasure_stack_full())
    {
        return false;
    }

    treasure_top++;
    treasure_stack[treasure_top] = t;

    return true;
}
bool pop_treasure(treasure *t)
{
    if (is_treasure_stack_empty())
    {
        return false;
    }

    *t = treasure_stack[treasure_top];
    treasure_top--;

    return true;
}
bool peek_treasure(treasure *t)
{
    if (is_treasure_stack_empty())
    {
        return false;
    }

    *t = treasure_stack[treasure_top];

    return true;
}
bool is_collected_stack_empty(void)
{
    return collected_top == -1;
}
bool push_collected_treasure(treasure t)
{
    if (collected_top == NUM_TREASURES - 1)
    {
        return false;
    }

    collected_top++;
    collected_treasures[collected_top] = t;

    return true;
}
bool pop_collected_treasure(treasure *t)
{
    if (is_collected_stack_empty())
    {
        return false;
    }

    *t = collected_treasures[collected_top];
    collected_top--;

    return true;
}
int calculate_distance(location player_loc, location treasure_loc)
{
    return abs(player_loc.x - treasure_loc.x)
         + abs(player_loc.y - treasure_loc.y);
}
void update_next_treasure_distance(void)
{
    if (is_treasure_stack_empty())
    {
        return;
    }

    treasure_stack[treasure_top].distance =
        calculate_distance(
            player_location,
            treasure_stack[treasure_top].loc
        );
}
bool found_treasure(location player_loc)
{
    if (is_treasure_stack_empty())
    {
        return false;
    }

    return player_loc.x == treasure_stack[treasure_top].loc.x &&
           player_loc.y == treasure_stack[treasure_top].loc.y;
}
bool calculate_player_position(char move, location *new_position)
{
    location current_position;

    if (!peek_player_position(&current_position))
    {
        return false;
    }

    move = (char)tolower((unsigned char)move);

    *new_position = current_position;

    switch (move)
    {
        case 'w':
            if (current_position.y >= GRID_SIZE - 1)
            {
                return false;
            }

            new_position->y++;
            break;

        case 's':
            if (current_position.y <= 0)
            {
                return false;
            }

            new_position->y--;
            break;

        case 'a':
            if (current_position.x <= 0)
            {
                return false;
            }

            new_position->x--;
            break;

        case 'd':
            if (current_position.x >= GRID_SIZE - 1)
            {
                return false;
            }

            new_position->x++;
            break;

        default:
            return false;
    }

    return true;
}
void collect_treasure(void)
{
    treasure collected;

    if (pop_treasure(&collected))
    {
        push_collected_treasure(collected);

        printf("\nTreasure collected: %s\n", collected.name);
    }
}
void move_player(void)
{
    char move;
    location new_position;
    bool found;

    if (!game_started)
    {
        printf("Please start the game first.\n");
        return;
    }

    printf("Press W/A/S/D to move: ");
    scanf(" %c", &move);

    if (!calculate_player_position(move, &new_position))
    {
        printf("Invalid move. Player cannot move outside the 10x10 grid.\n");
        return;
    }

    /*
       Check the NEW position before adding it to history.
    */
    found = found_treasure(new_position);

    /*
       Store whether this particular position collected
       a treasure. This is what makes Undo possible.
    */
    push_player_position(new_position, found);

    player_location = new_position;

    if (found)
    {
        collect_treasure();
    }

    update_next_treasure_distance();

    printf("Player moved to (%d,%d)\n",
           player_location.x,
           player_location.y);
}
void undo_move(void)
{
    location undone_position;
    bool was_collected;
    treasure restored_treasure;

    if (!game_started)
    {
        printf("Please start the game first.\n");
        return;
    }

    /*
       The first node is the starting position.
       Therefore, if there is only one node,
       there is nothing to undo.
    */
    if (position_top == NULL || position_top->next == NULL)
    {
        printf("No move to undo.\n");
        return;
    }

    /*
       Remove the current position.
    */
    pop_player_position(&undone_position, &was_collected);

    /*
       The next node is now the current position.
    */
    peek_player_position(&player_location);

    /*
       If the undone move collected a treasure,
       restore that treasure.

       Since collected treasures are also a stack,
       the treasure that was collected most recently
       must be at the top.
    */
    if (was_collected)
    {
        if (pop_collected_treasure(&restored_treasure))
        {
            push_treasure(restored_treasure);

            printf("Treasure dropped: %s\n",
                   restored_treasure.name);
        }
    }

    update_next_treasure_distance();

    printf("Player returned to (%d,%d)\n",
           player_location.x,
           player_location.y);
}
void display_treasure(treasure t)
{
    printf("\n");
    printf("Name     : %s\n", t.name);
    printf("Location : (%d,%d)\n", t.loc.x, t.loc.y);
    printf("Distance : %d\n", t.distance);
    printf("\n");
}
void display_next_treasure(void)
{
    treasure next_treasure;

    if (!game_started)
    {
        printf("Please start the game first.\n");
        return;
    }

    if (!peek_treasure(&next_treasure))
    {
        printf("All treasures have been collected!\n");
        return;
    }

    update_next_treasure_distance();

    /*
       Get the updated treasure after calculating distance.
    */
    peek_treasure(&next_treasure);

    printf("\n===== NEXT TREASURE =====\n");
    display_treasure(next_treasure);
}
void display_score(void)
{
    int score;

    if (!game_started)
    {
        printf("Please start the game first.\n");
        return;
    }

    score = collected_top + 1;

    printf("\n===== SCORE =====\n");
    printf("Treasures collected : %d/%d\n",
           score,
           NUM_TREASURES);

    if (score == NUM_TREASURES)
    {
        printf("All treasures collected!\n");
    }

    printf("=================\n");
}
void display_map(void)
{
    if (!game_started)
    {
        printf("Please start the game first.\n");
        return;
    }

    printf("\n========== MAP ==========\n");

    for (int y = GRID_SIZE - 1; y >= 0; y--)
    {
        printf("%d | ", y);

        for (int x = 0; x < GRID_SIZE; x++)
        {
            bool player_here =
                player_location.x == x &&
                player_location.y == y;

            bool treasure_here = false;

            for (int i = 0; i <= treasure_top; i++)
            {
                if (treasure_stack[i].loc.x == x &&
                    treasure_stack[i].loc.y == y)
                {
                    treasure_here = true;
                    break;
                }
            }

            if (player_here)
            {
                printf("P ");
            }
            else if (treasure_here)
            {
                printf("T ");
            }
            else
            {
                printf(". ");
            }
        }

        printf("\n");
    }

    printf("    --------------------\n");
    printf("     0 1 2 3 4 5 6 7 8 9\n");

    printf("\nP = Player\n");
    printf("T = Remaining Treasure\n");
    printf(". = Empty\n");

    printf("=========================\n");
}
void reset_treasure_stack(void)
{
    treasure_top = -1;

    for (int i = 0; i < NUM_TREASURES; i++)
    {
        push_treasure(original_treasures[i]);
    }
}
void reset_game(void)
{
    clear_position_stack();
    collected_top = -1;
    reset_treasure_stack();
    push_player_position(player_location, false);
    update_next_treasure_distance();
    printf("\nGame has been reset.\n");
    printf("Player returned to starting position (%d,%d).\n",
           player_location.x,
           player_location.y);
}
int read_coordinate(char coordinate_name)
{
    int value;

    do
    {
        printf("Enter %c coordinate (0-%d): ",
               coordinate_name,
               GRID_SIZE - 1);

        scanf("%d", &value);

        if (value < 0 || value >= GRID_SIZE)
        {
            printf("Invalid coordinate. Enter a value from 0 to %d.\n",
                   GRID_SIZE - 1);
        }

    } while (value < 0 || value >= GRID_SIZE);

    return value;
}
void start_game(void)
{
    int x;
    int y;
    if (game_started)
    {
        printf("Game has already started.\n");
        return;
    }
    printf("\n========== START GAME ==========\n");
    for (int i = 0; i < NUM_TREASURES; i++)
    {
        printf("\nTreasure %d\n", i + 1);

        x = read_coordinate('X');
        y = read_coordinate('Y');

        printf("Enter treasure name: ");
        scanf("%49s", original_treasures[i].name);

        original_treasures[i].loc.x = x;
        original_treasures[i].loc.y = y;
        original_treasures[i].distance = 0;
    }
    treasure_top = -1;

    for (int i = 0; i < NUM_TREASURES; i++)
    {
        push_treasure(original_treasures[i]);
    }
    printf("\nEnter starting player position:\n");
    player_location.x = read_coordinate('X');
    player_location.y = read_coordinate('Y');
    clear_position_stack();

    collected_top = -1;
    bool found = found_treasure(player_location);

    push_player_position(player_location, found);

    if (found)
    {
        collect_treasure();
    }
    update_next_treasure_distance();
    game_started = true;
    printf("\nGame started!\n");
    printf("Player position: (%d,%d)\n",
           player_location.x,
           player_location.y);
    if (found)
    {
        printf("You started on a treasure!\n");
    }

    printf("================================\n");
}
void display_menu(void)
{
    printf("\n");
    printf("\t\t\tTREASURE HUNT GAME\n");
    printf("\t\t\t******************\n");
    printf("1. Start Game\n");
    printf("2. Move Player (W/A/S/D)\n");
    printf("3. Check Next Treasure\n");
    printf("4. Undo (Z)\n");
    printf("5. Reset (R)\n");
    printf("6. Display Score\n");
    printf("7. Display Map\n");
    printf("8. Exit\n");
    printf("\n");
}
int main()
{
    int choice;

    while (true)
    {
        display_menu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                start_game();
                break;

            case 2:
                move_player();
                break;

            case 3:
                display_next_treasure();
                break;

            case 4:
                undo_move();
                break;

            case 5:
                if (!game_started)
                {
                    printf("Please start the game first.\n");
                }
                else
                {
                    reset_game();
                }
                break;

            case 6:
                display_score();
                break;

            case 7:
                display_map();
                break;

            case 8:
                clear_position_stack();
                printf("Exiting Treasure Hunt. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Please enter 1-8.\n");
        }
    }
}
