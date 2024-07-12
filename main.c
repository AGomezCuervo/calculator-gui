#include <stdio.h>
#include "calc.h"

//TODO: Handle implicit multiplication eg. 2(2)
//TODO: Add history
int main(int argsc, char **argsv)
{
        Calculator *calculator = init_calculator();

        if (argsc != 2)
        {
                printf("USE: calc <expr>\n");
                dead(calculator, ERR_NO_INPUT);
        }

        double result = calculate_expr(calculator, argsv[1]);

        printf("%.2f\n", result);
        destroy_calculator(calculator);
}
