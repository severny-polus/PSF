#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Произведение переменных. Реализует сортированный односвязный список.
typedef struct VariableProduct {
    char                    variable;
    struct VariableProduct* tail;
} VariableProduct;

// Создать новый объект произведения
VariableProduct* new_product(char variable, VariableProduct* tail) {
    VariableProduct* p = malloc(sizeof(VariableProduct));
    p->variable        = variable;
    p->tail            = tail;
    return p;
}

// Освободить память всего списка
void free_product(VariableProduct* p) {
    if (p == NULL) {
        return;
    }
    VariableProduct* next = p->tail;
    free(p);
    free_product(next);
}

// Создать копию списка
VariableProduct* copy_product(VariableProduct* p) {
    if (p == NULL) {
        return NULL;
    }
    return new_product(p->variable, copy_product(p->tail));
}

// Напечатать произведение переменных
// * не обрабатывает NULL
void print_product(VariableProduct* product) {
    printf("%c", product->variable);
    if (product->tail != NULL) {
        printf("*");
        print_product(product->tail);
    }
}

// Сравнение произведений переменных на равенство
bool equal_products(VariableProduct* a, VariableProduct* b) {
    if (a == NULL && b == NULL) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }
    if (a->variable != b->variable) {
        return false;
    }
    return equal_products(a->tail, b->tail);
}

// Добавить к произведению ещё одну переменную.
// * параметр p может быть изменён
VariableProduct* multiply_assign(VariableProduct* p, char variable) {
    if (p == NULL || variable < p->variable) {
        return new_product(variable, p);
    }
    p->tail = multiply_assign(p->tail, variable);
    return p;
}

// Перемножить произведения переменных
VariableProduct* multiply(VariableProduct* a, VariableProduct* b) {
    VariableProduct* c = NULL;
    while (a != NULL) {
        c = multiply_assign(c, a->variable);
        a = a->tail;
    }
    while (b != NULL) {
        c = multiply_assign(c, b->variable);
        b = b->tail;
    }
    return c;
}

// Сумма произведений переменных с целочисленными коэффициентами. Реализует список.
typedef struct ProductSum {
    int                multiplier;
    VariableProduct*   product;
    struct ProductSum* tail;
} ProductSum;

// Создать новый объект суммы.
ProductSum* new_sum(int multiplier, VariableProduct* product, ProductSum* tail) {
    ProductSum* s = malloc(sizeof(ProductSum));
    s->multiplier = multiplier;
    s->product    = product;
    s->tail       = NULL;
    return s;
}

// Освободить память всего списка суммы.
void free_sum(ProductSum* s) {
    if (s == NULL) {
        return;
    }
    free_product(s->product);
    ProductSum* next = s->tail;
    free(s);
    free_sum(next);
}

// Напечатать сумму
void print_sum(ProductSum* sum) {
    if (sum == NULL) {
        printf("0");
        return;
    }

    if (sum->multiplier == -1) {
        printf("-");
    }
    if (abs(sum->multiplier) != 1 || sum->product == NULL) {
        printf("%d", sum->multiplier);
    }
    if (sum->product != NULL) {
        if (abs(sum->multiplier) != 1) {
            printf("*");
        }
        print_product(sum->product);
    }

    sum = sum->tail;

    while (sum != NULL) {
        if (sum->multiplier > 0) {
            printf(" + ");
        } else {
            printf(" - ");
        }

        if (abs(sum->multiplier) != 1 || sum->product == NULL) {
            printf("%d", abs(sum->multiplier));
        }
        if (sum->product != NULL) {
            if (abs(sum->multiplier) != 1) {
                printf("*");
            }
            print_product(sum->product);
        }

        sum = sum->tail;
    }
}

// Добавить к сумме s ещё одно слагаемое. При возможности упростить.
// * может изменить данные s или освободить s и product
ProductSum* add_assign(ProductSum* s, int multiplier, VariableProduct* product) {
    if (s == NULL) {
        return new_sum(multiplier, product, NULL);
    }
    if (equal_products(s->product, product)) {
        s->multiplier += multiplier;
        free_product(product);
        if (s->multiplier == 0) {
            ProductSum* next = s->tail;
            s->tail          = NULL;
            free_product(s->product);
            free(s);
            return next;
        }
        return s;
    }
    s->tail = add_assign(s->tail, multiplier, product);
    return s;
}

// Сложить две суммы.
ProductSum* add(ProductSum* a, ProductSum* b) {
    ProductSum* c = NULL;
    while (a != NULL) {
        c = add_assign(c, a->multiplier, copy_product(a->product));
        a = a->tail;
    }
    while (b != NULL) {
        c = add_assign(c, b->multiplier, copy_product(b->product));
        b = b->tail;
    }
    return c;
}

// Вычесть из одной суммы другую.
ProductSum* subtract(ProductSum* a, ProductSum* b) {
    ProductSum* c = NULL;
    while (a != NULL) {
        c = add_assign(c, a->multiplier, copy_product(a->product));
        a = a->tail;
    }
    while (b != NULL) {
        c = add_assign(c, -b->multiplier, copy_product(b->product));
        b = b->tail;
    }
    return c;
}

// Проверка двух сумм на равенство
bool equal_sums(ProductSum* a, ProductSum* b) {
    ProductSum* c = subtract(a, b);
    bool equal = c == NULL;
    free_sum(c);
    return equal;
}

// Произведение сумм
ProductSum* multiply_sums(ProductSum* a, ProductSum* b) {
    ProductSum* c       = NULL;
    ProductSum* b_first = b;
    while (a != NULL) {
        while (b != NULL) {
            c = add_assign(c, (a->multiplier * b->multiplier), multiply(a->product, b->product));
            b = b->tail;
        }
        a = a->tail;
        b = b_first;
    }
    return c;
}

// Построить сумму из считанной строки. При возможности упроcтить.
ProductSum* parse_sum(char* input) {

    int              multiplier_sign = 1;
    int              multiplier_abs  = 0;
    VariableProduct* product         = NULL;
    ProductSum*      sum             = NULL;

    for (int i = 0; input[i] != '\0'; ++i) {
        char c = input[i];

        switch (c) {
        case ' ':
            continue;
        case '*':
            continue;
        }

        if (c == '+' || c == '-') {
            if (multiplier_abs != 0) {
                sum             = add_assign(sum, multiplier_abs * multiplier_sign, product);
                multiplier_sign = 1;
                multiplier_abs  = 0;
                product         = NULL;
            }
            if (c == '-') {
                multiplier_sign = -1;
            }
            continue;
        }

        if ('0' <= c && c <= '9') {
            multiplier_abs = multiplier_abs * 10 + (int)(c - '0');
            continue;
        }

        if ('a' <= c && c <= 'z') {
            if (multiplier_abs == 0) {
                multiplier_abs = 1;
            }
            product = multiply_assign(product, c);
            continue;
        }
    }

    if (multiplier_abs != 0) {
        sum = add_assign(sum, multiplier_abs * multiplier_sign, product);
    }

    free(input);
    return sum;
}

// Считывает PS-форму из потока
ProductSum* scan_product_sum(FILE* file) {
    char*  line = NULL;
    size_t size = 0;
    getline(&line, &size, file);
    return parse_sum(line);
}
