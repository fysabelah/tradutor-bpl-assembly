#include <stdio.h>

// Função definidas em BPL
int f1();
int f2(int);
int f3(int*, int);
int f4(int);
int f5(int, int *);
int f7(int, int *);
int f8();

int main() {

    int vet[] = {1, 2, 3};

    printf("%d\n", f1());
    printf("%d\n", f2(3));
    printf("%d\n", f3(vet, 3));
    printf("%d\n", f4(1));
    printf("%d\n", f5(3, vet));
    printf("%d\n", f7(3, vet));
    printf("%d\n", f8());

    return 0;
}