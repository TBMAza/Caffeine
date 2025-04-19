#include <stdio.h>
#include <stdlib.h>
int main() {
int a; scanf("%d", &a);
if(a==1) {
printf("one");
}
else if(a==2) {
printf("two");
}
else if(a==3) {
printf("three");
}
else if(a>=4&&a<=9) {
printf("something between four and nine");
}
else {
printf("two digits or more");
}
if(a==100) {
printf("another attempt");
}
return 0;
}
