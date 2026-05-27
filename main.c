#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h> 
#include "compression.h"
#include "arbrebinaire.h"
#include "lsc.h"


int main() {
    FreqObject freq = count_frequency("abbraccaddabrra");

    printf("\n Classic Version : \n");

    print_frequency(&freq);

    order_frequency(&freq); 
    
    printf("\n Order Version : \n");
    
    print_frequency(&freq);

    lsc* leaf_order = frequency_to_lsc(&freq); 

    printf("\nlsc version : \n"); 

    lsc_print(leaf_order);

    free(freq.array);

    lsc_dispose(&leaf_order);

    return 1;
}