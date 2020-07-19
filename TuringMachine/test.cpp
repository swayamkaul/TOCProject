/* This C file implements a Turing Machine
 * author: Kevin Zhou
 * Computer Science and Electronics
 * University of Bristol
 * Date: 21st April 2010
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tapes
{
    struct tapes *left;
    struct tapes *right;
    char content;
} Tape;

typedef enum
{
    LEFT,
    RIGHT
} Direction;

typedef struct transition
{
    char current_state;
    char tape_symbol;
    char new_state;
    char new_tape_symbol;
    Direction dir;
} Transition;

typedef struct list
{
    Transition *content;
    struct list *next;
} List;

typedef struct tm
{
    char *input_alpha;
    char *input;
    char *tape_alpha;
    char start;
    char accept;
    char reject;
    List *transition;
} TM;

Tape *insert_tape(Tape *t, Direction dir, char c)
{
    Tape *head = t;
    Tape *new1 = calloc(1, sizeof(Tape));
    ;
    new1->content = c;
    if (dir == LEFT)
    {
        while (t->left != NULL)
        {
            t = t->left;
        }
        new1->right = t;
        new1->left = NULL;
        t->left = new1;
        return new1;
    }
    if (dir == RIGHT)
    {
        while (t->right != NULL)
        {
            t = t->right;
        }
        new1->left = t;
        new1->right = NULL;
        t->right = new1;
    }
    return head;
}

Tape *create_tape(char *input)
{
    int i = 1;
    Tape *t = calloc(1, sizeof(Tape));
    t->content = input[0];
    while (1)
    {
        if (input[i] == '\0')
            break;
        t = insert_tape(t, RIGHT, input[i]);
        i++;
    }
    return t;
}

/* turn the input string into Transition fields */
Transition *get_transition(char *s)
{
    Transition *t = calloc(1, sizeof(Transition));
    Direction dir;
    t->current_state = s[0];
    t->tape_symbol = s[1];
    t->new_state = s[2];
    t->new_tape_symbol = s[3];
    dir = (s[4] == 'R') ? RIGHT : LEFT;
    t->dir = dir;
    return t;
}

/* turn the string into transitions and add into list */
List *insert_list(List *l, char *elem)
{
    List *t = calloc(1, sizeof(List));
    List *head = l;
    while (l->next != NULL)
        l = l->next;
    t->content = get_transition(elem);
    t->next = NULL;
    l->next = t;
    return head;
}

/* insert a transition into a list */
List *insert_list_transition(List *l, Transition *tr)
{
    List *t = calloc(1, sizeof(List));
    List *head = l;
    while (l->next != NULL)
        l = l->next;
    t->content = tr;
    t->next = NULL;
    l->next = t;
    return head;
}

void print_tape(Tape *t, char blank)
{
    char c;
    while (1)
    {
        if (t->content != blank)
            break;
        t = t->right;
    }
    while (1)
    {
        if (t == NULL)
            break;
        c = t->content;
        if (t->content != blank)
            putchar(c);
        t = t->right;
    }
    putchar('\n');
}

void print_transition(Transition *t)
{
    char s1[] = "Left";
    char s2[] = "Right";
    if (t == NULL)
    {
        printf("NULL Transfer");
        return;
    }
    printf("current:%c tape:%c new state:%c new tape:%c direction %s\n", t->current_state, t->tape_symbol, t->new_state, t->new_tape_symbol, (t->dir == LEFT) ? s1 : s2);
}

/*test if the char c is in the string s */
int contains(char c, char *s)
{
    int i = 0;
    while (1)
    {
        if (c == s[i])
            return 1;
        if (s[i] == '\0')
            return 0;
        i++;
    }
}

/* test if the input is a valid input */
int is_valid_input(char *input_alpha, char *input)
{
    int i = 0;
    char c;
    while (1)
    {
        c = input[i];
        if (c == '\0')
            break;
        if (!contains(c, input_alpha))
            return 0;
        i++;
    }
    return 1;
}

TM *createTM(char *input)
{

    TM *m = calloc(1, sizeof(TM));
    List *tr = calloc(1, sizeof(List));
    char *buffer;
    /*read input alphabet of PDA*/
    buffer = strtok(input, ":");
    if (buffer == NULL)
    {
        printf("Error in reading input alphabet!\n");
        exit(1);
    }
    m->input_alpha = buffer;

    /*read tape alphabet*/
    buffer = strtok(NULL, ":");

    if (buffer == NULL)
    {
        printf("Error in reading tape alphabet!\n");
        exit(1);
    }
    m->tape_alpha = buffer;

    /*read input sequence*/
    buffer = strtok(NULL, ":");
    if (buffer == NULL)
    {
        printf("Error in reading input sequence!\n");
        exit(1);
    }

    if (!is_valid_input(m->input_alpha, buffer))
    {
        printf("Error! Input contains some invalid characters that don't match the input alphabet!\n");
        exit(1);
    }

    m->input = buffer;
    buffer = strtok(NULL, ":");
    m->start = buffer[0];
    buffer = strtok(NULL, ":");
    m->accept = buffer[0];
    buffer = strtok(NULL, ":");
    m->reject = buffer[0];

    /*read tape transition*/
    while (1)
    {
        buffer = strtok(NULL, ":");
        if (buffer == NULL)
            break;
        tr = insert_list(tr, buffer);
    }

    m->transition = tr->next;
    return m;
}

Transition *find_transition(List *list, char state, char tape_symbol)
{
    Transition *t;
    while (1)
    {
        if (list == NULL)
            return NULL;
        t = list->content;
        if (t->current_state == state && t->tape_symbol == tape_symbol)
            return t;
        list = list->next;
    }
}

Tape *move(Tape *t, Direction dir, char blank)
{
    if (dir == LEFT)
    {
        if (t->left == NULL)
        {
            t = insert_tape(t, LEFT, blank);
        }
        return t->left;
    }
    if (dir == RIGHT)
    {
        if (t->right == NULL)
        {
            t = insert_tape(t, RIGHT, blank);
        }
        return t->right;
    }
    return NULL;
}

void simulate(TM *m)
{
    /* first symbol in input symbol used to represent the blank symbol */
    const char blank = m->tape_alpha[0];
    char current_state = m->start;
    Tape *tape = create_tape(m->input);
    Tape *current_tape = tape;
    char current_tape_symbol;
    Transition *current_transition;
    while (1)
    {
        if (current_state == m->accept)
        {
            printf("Accept\n");
            print_tape(tape, blank);
            break;
        }
        if (current_state == m->reject)
        {
            printf("Reject\n");
            print_tape(tape, blank);
            break;
        }
        current_tape_symbol = (current_tape == NULL || current_tape->content == '\0') ? blank : current_tape->content;
        current_transition = find_transition(m->transition, current_state, current_tape_symbol);
        current_state = current_transition->new_state;
        current_tape->content = current_transition->new_tape_symbol;
        current_tape = move(current_tape, current_transition->dir, blank);
    }
}

int main(void)
{
    char s[300];
    TM *p;
    scanf("%s", s);
    p = createTM(s);
    simulate(p);
    return 0;
}