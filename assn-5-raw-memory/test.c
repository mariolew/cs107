/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2015年08月26日 星期三 15时31分41秒
 ************************************************************************/

#include<stdio.h>
struct binky{
    int a;
};
int main() {
    char *s = "hi";
    int three = 3;
    printf("Read interger %d as float: %f\n", three, three);

    struct binky b;
    printf("Printing four byte struct as float %f\n", *(float *)&b);

    char *haha = "ha ha ha ha";
    int res = *(int *)haha;
    printf("Read the first four characters in a string to an int %d\n", res);

    short new = 1;
    char ch = (char)new;
    printf("Endian is %s.\n", (ch == 0 ? "big" : "little"));

}
