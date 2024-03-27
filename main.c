#include "psf.c"

int main() {
    char operation;
    fscanf(stdin, "%c\n", &operation);
    ProductSum* sum1 = scan_product_sum(stdin);
    ProductSum* sum2 = scan_product_sum(stdin);

    ProductSum* sum;
    switch (operation) {
    case '+':
        sum = add(sum1, sum2);
        print_sum(sum);
        free_sum(sum);
        break;
    case '-':
        sum = subtract(sum1, sum2);
        print_sum(sum);
        free_sum(sum);
        break;
    case '=':
        if (equal_sums(sum1, sum2)) {
            printf("equal");
        } else {
            printf("not equal");
        }
        break;
    case '*':
        sum = multiply_sums(sum1, sum2);
        print_sum(sum);
        free_sum(sum);
        break;
    }
    printf("\n");

    free_sum(sum1);
    free_sum(sum2);

    return 0;
}