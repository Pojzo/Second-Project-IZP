#include <stdio.h>
#include <stdlib.h>


struct Test {
    int a;
};

struct Test test() {
    struct Test test;
    test.a = 5;
    return test;
}


int main() {
    struct Test test = test();
    return 0;
}
