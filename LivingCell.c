#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N   7  //the number of substances inside the cell.
#define K   5  //K number of types of reactions that will occur at interior of the cell.
#define SIZE 512

int condition_counter;

typedef struct _CellTag {
    char name[64];
    int reactant1;
    int reactant2;
    int inhibitor;
    int product1;
    int product2;
    int if_enable;
    int reaction_conter;
} Cell;

int     check_condition(int* cnd, int size);
int     check_cell_numbers(const Cell* mycell, int size);
void    print_condition(const int* condition, size_t size);
int     inhibitor_condition_cmp(int inhibitor, int react1, int react2, const int* condition, int cnt);
int     print_reactions(const Cell* cll);
void    set_reaction_condition(Cell* mycell, int** condition, int* cnt);
int     sorting_algo(const void* p1, const void* p2);


int main(int argc, char* argv[])
{
    int i;
    char number_of_substances[SIZE];
    Cell mycell[K];
    char* str;
    int cnt = 0;
    int number_reactions;
    int number_of_simuiation_steps;
    int* condition;

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments...\n");
        fprintf(stderr, "usage: %s <max number of simulation steps>", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((condition = (int*)malloc(sizeof(int) * N)) == NULL) {
        fprintf(stderr, "there is no enough memory!\n");
        exit(EXIT_FAILURE);
    }

    memset(condition, N, sizeof(int) * N);
    
    for (i = 0; i < K; ++i)
        mycell[i].reaction_conter = 0;

    printf("Please enter the initial condition:\n");
    fgets(number_of_substances, SIZE, stdin);

    for (i = 0, str = strtok(number_of_substances, " "); str != NULL; ++i, str = strtok(NULL, " ")) {
        condition[i] = atoi(str);
        ++cnt;
    }

    if (check_condition(condition, cnt)) {
        exit(EXIT_FAILURE);
    }

    number_reactions = K;

    printf("please enter the reactions:\n");
    for (i = 0; i < number_reactions; ++i) {
        scanf("%s%d%d%d%d%d", mycell[i].name, &mycell[i].reactant1, &mycell[i].reactant2, &mycell[i].inhibitor, &mycell[i].product1, &mycell[i].product2);
    }

    if (check_cell_numbers(mycell, K)) {
        fprintf(stderr, "the number of substances can't be more %d\n", N);
        exit(EXIT_FAILURE);
    }

    printf("\n------------------------------------------------\n");

    number_of_simuiation_steps = atoi(argv[1]);


    while (number_of_simuiation_steps--) {
        printf("condition %d\n", condition_counter + 1);

        print_condition(condition, cnt);
        set_reaction_condition(mycell, &condition, &cnt);

        if (!print_reactions(mycell)) {
            printf("no more reactions happen\n");
            break;
        }

        printf("Please hit enter button to go to next condition...\n");

        for (i = 0; i < K; ++i)
            mycell[i].if_enable = 0;
        getchar();
    }

    free(condition);

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("SUMMARY");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    for (i = 0; i < K; ++i)
        printf("reaction %s happens %d times\n", mycell[i].name, mycell[i].reaction_conter);

    if (number_of_simuiation_steps < 0) {
        printf("Max simulation steps has been reached...\n");
    }

    return 0;
}

int check_condition(int* cnd, int size)
{
    int i;

    for (i = 0; i < size; ++i)
        if (cnd[i] >= N) {
            fprintf(stderr, "the number of initial config can't be more %d...\n", N);
            return 1;
        }

    return 0;
}

int check_cell_numbers(const Cell* mycell, int size)
{
    int i;
    for (i = 0; i < size; ++i) {
        if (mycell[i].reactant1 >= N)
            return 1;
        if (mycell[i].reactant2 >= N)
            return 1;
        if (mycell[i].inhibitor >= N)
            return 1;
        if (mycell[i].product1 >= N)
            return 1;
        if (mycell[i].product2 >= N)
            return 1;
    }

    return 0;
}

void print_condition(const int* condition, size_t size)
{
    size_t i;

    int* temp = malloc(sizeof(int) * size);

    if (temp == NULL)
        return;

    memcpy(temp, condition, sizeof(int) * size);

    qsort(temp, size, sizeof(int), sorting_algo);

    putchar('\n');

    printf("%d ", temp[0]);

    for (i = 1; i < size; ++i) {
        if (temp[i - 1] != temp[i]) {
            printf("%d ", temp[i]);
        }
    }

    free(temp);

    putchar('\n');
}

int inhibitor_condition_cmp(int inhibitor, int react1, int react2, const int* condition, int cnt)
{
    int i;
    int inhib_check = 0;
    int react1_check = 0;
    int react2_check = 0;

    for (i = 0; i < cnt; ++i)
        if (inhibitor == condition[i])
            inhib_check = 1;

    if (inhib_check)
        return 0;

    for (i = 0; i < cnt; ++i)
        if (react1 == condition[i])
            react1_check = 1;

    for (i = 0; i < cnt; ++i)
        if (react2 == condition[i])
            react2_check = 1;

    if (!react1_check || !react2_check)
        return 0;

    return 1;
}

int print_reactions(const Cell* cll)
{
    int i;
    int cnt = 0;


    for (i = 0; i < K; ++i) {
        if (cll[i].if_enable) {
            ++cnt;
            printf("reaction enabled: %s %d %d %d %d %d\n", cll[i].name, cll[i].reactant1, cll[i].reactant2, cll[i].inhibitor, cll[i].product1, cll[i].product2);
        }
    }

    ++condition_counter;

    printf("\nIn this condition %d times reactions happened...\n", cnt);

    if (cnt == 0)
        return 0;



    printf("\n------------------------------------------------\n");

    return cnt;
}

void set_reaction_condition(Cell* cll, int** condition, int* psize)
{
    int cnt = 0;
    int i;

    for (i = 0; i < K; ++i) {
            if (inhibitor_condition_cmp(cll[i].inhibitor, cll[i].reactant1, cll[i].reactant2, *condition, *psize)) {
                cll[i].if_enable = 1;
                ++cll[i].reaction_conter;
                ++cnt;
            }
            else
                cll[i].if_enable = 0;
    }

    cnt = 0;

    for (i = 0; i < K; ++i) {
        if (cll[i].if_enable) {
                (*condition)[cnt++] = cll[i].product1;
                (*condition)[cnt++] = cll[i].product2;
        }
    }

    *psize = cnt;

}

int sorting_algo(const void* p1, const void* p2)
{
    return *(int*)p1 - *(int*)p2;
}
