// File Name: testio.c
// Author: pantasun
// Created Time: 2019年05月24日 星期五 16时01分26秒

#include<stdio.h>
#include <stdlib.h>
int main(int argc, char * argv[]){
    
    int a;
    scanf("%d", &a);
    char i = a;
    printf("i: int = %d, char = %c binary=%#x\n", i, i, i);
    printf("a: int = %d, char = %c binary=%#x\n", a, a, a);
    exit(0);
}
