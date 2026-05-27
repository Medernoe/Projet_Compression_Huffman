#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h> 
#include "compression.h"


int main() {
    FreqObject freq = count_frequency("abbraccaddabrra");

    printf("\n Classic Version : \n");

    print_frequency(&freq);

    order_frequency(&freq); 
    
    printf("\n Order Version : \n");
    
    print_frequency(&freq);

    free(freq.array);
    
    return 0;
}