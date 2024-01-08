#include <stdio.h>

int nums[] = {37,11,7,5};
int numbers = 4;

int lcm(int index, int mcm){
    if(index == numbers-1) return mcm;

    int b = nums[++index];

    int max = (mcm > b) ? mcm : b;
    while((max % mcm != 0) || (max % b != 0))
        max++;
    
    printf("found %d\n", max);

    return lcm(index, max);
}

int main(){

    printf("lcm: %d\n" , lcm(0, nums[0]));
    return 0;
}
