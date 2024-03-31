#include "psf.c"
#include <stdio.h>
#include <string.h>
#include <time.h>

void assert_equal_sums(ProductSum* expected, ProductSum* got) {
    if (equal_sums(expected, got)) {
        printf("OK");
    } else {
        printf("ERR\n  expected: ");
        print_sum(expected);
        printf("\n       got: ");
        print_sum(got);
    }
    printf("\n");
    free_sum(expected);
    free_sum(got);
}

void assert_equal_strings(char* expected, char* got) {
    if (strcmp(expected, got) == 0) {
        printf("OK\n");
    } else {
        printf("ERR\n  expected: %s", expected);
        printf("       got: %s", got);
    }
}

// Запускает тест
void run_test(FILE* in, FILE* out) {
    char operation;
    fscanf(in, "%c\n", &operation);
    if (in == NULL) {
        printf("in is NULL");
    }
    ProductSum* sum1 = scan_product_sum(in);
    ProductSum* sum2 = scan_product_sum(in);

    ProductSum* expect;
    ProductSum* sum;
    char*       line = NULL;
    size_t      size = 0;
    switch (operation) {
    case '+':
        sum    = add(sum1, sum2);
        expect = scan_product_sum(out);
        assert_equal_sums(expect, sum);
        break;
    case '-':
        sum    = subtract(sum1, sum2);
        expect = scan_product_sum(out);
        assert_equal_sums(expect, sum);
        break;
    case '=':
        getline(&line, &size, out);
        if (equal_sums(sum1, sum2)) {
            assert_equal_strings(line, "equal\n");
        } else {
            assert_equal_strings(line, "not equal\n");
        }
        free(line);
        break;
    case '*':
        sum    = multiply_sums(sum1, sum2);
        expect = scan_product_sum(out);
        assert_equal_sums(expect, sum);
        break;
    }

    free_sum(sum1);
    free_sum(sum2);
}

// Запускает все тесты
void run_tests(int n_tests) {
    time_t start, end;
    time(&start);
    for (int i = 0; i < n_tests; ++i) {
        char path[30];
        sprintf(path, "tests/%d.test", i + 1);
        FILE* in = fopen(path, "r");
        sprintf(path, "tests/%d.ans", i + 1);
        FILE* out = fopen(path, "r");

        printf("test %d: ", i + 1);
        run_test(in, out);

        fclose(in);
        fclose(out);
    }
    time(&end);
    double diff = difftime(end, start);
    printf("time elapsed: %fs\n", diff);
}

int main() {
    int n_tests = 9;
    run_tests(n_tests);
    return 0;
}
