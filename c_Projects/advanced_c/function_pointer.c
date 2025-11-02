#include <stdio.h>

void foo(int a, char *c) {
    printf("%s %d\n",c, a);
}

int main() {
    int a = 1;
    int *b = &a;
    printf("%p\n", b);
    void (*clark) (int, char *) = foo;
    clark(1, "hello");
    return 0;
}
