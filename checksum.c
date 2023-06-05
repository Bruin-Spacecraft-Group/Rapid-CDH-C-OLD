#include "HAL.h"
#include "FlightStateMachine.h"
#include <stdio.h>
#include <stdlib.h>


struct char_ptr_with_size {
    int size;
    char* data;
}

char_ptr_with_size calculate_checksum(char[] y, int dataSize) {
    unsigned int x = 0;
    for (int i = 0; i<dataSize; i++)
    {
        x = x + (y[i] << ((dataSize-i)*8));
    }
    unsigned int result = 0;
    unsigned int current_index = 3; //start index at 3, count up but only use numbers with Hamming weight >=2
    unsigned int number_to_skip = 4; //rather than take the log of every number (or computing a bitwise and), keep track of which number is next to skip
    while(x)
    {
        unsigned int add = x & 1; //if rightmost bit is set or not in original message
        result += add*current_index;
        current_index++;
        if (current_index == number_to_skip)
        {
            current_index++;
            number_to_skip *= 2;
        }
        x /= 2; //discard rightmost bit
        
    }
    result = result+3;
    unsigned int last_2_bits = result % 4; //check if last 2 bits are 00 or 11
    if (last_2_bits == 0)
        result++;
    else if (last_2_bits % 4 == 3)
        result--;

    char_ptr_with_size final_res;
    final_res.size = 1;
    if (result >> 24 != 0)
        final_res.size = 4;
    else if (result >> 16 != 0)
        final_res.size  = 3;
    else if(result >> 8 != 0)
        final_res.size = 2;
    
    final_res.data = (char*)malloc(final_res.size()*sizeof(char));
    for (int i = 0; i<final_res.size; i++)
    {
        final_res.data[i] = result << ((final_res.size-i)*8);
    }
    return final_res;   
}