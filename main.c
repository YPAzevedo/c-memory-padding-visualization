// c
#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>
#include <string.h>

typedef struct Name {
    char* first;
    char* last;
} name_t;

typedef struct Human1 {
    char   first_initial;
    int    age;
    double height;
    name_t name;
} human1_t;

typedef struct Human2 {
    name_t name;
    double height;
    int    age;
    char   first_initial;
} human2_t;

struct FieldDesc {
    const char *name;
    char tag;
    size_t offset;
    size_t size;
};

#define FIELD(struct_t, field, tagchar) \
    (struct FieldDesc){#field, tagchar, offsetof(struct_t, field), sizeof(((struct_t*)0)->field)}

static void visualize(const char *title, size_t sz, const struct FieldDesc *fields, size_t nfields) {
    char mem[256] = {0};
    if (sz > sizeof mem) {
        printf("%s: struct too large to visualize (%zu bytes)\n", title, sz);
        return;
    }
    for (size_t i = 0; i < sz; i++) mem[i] = '.';

    for (size_t f = 0; f < nfields; f++) {
        for (size_t i = 0; i < fields[f].size; i++) {
            mem[fields[f].offset + i] = fields[f].tag;
        }
    }
    for (size_t i = 0; i < sz; i++) if (mem[i] == '.') mem[i] = 'P';

    printf("\n%s: size=%zu bytes\n", title, sz);
    printf("Offsets: ");
    for (size_t f = 0; f < nfields; f++) {
        printf("%s@%zu ", fields[f].name, fields[f].offset);
    }
    printf("\n");

    for (size_t i = 0; i < sz; i++) {
        printf("%2zu |", i);
    }
    printf("\n");
    for (size_t i = 0; i < sz; i++) {
        printf(" %c |", mem[i]);
    }
    printf("\n");
    printf("\nLegend: ");
    for (size_t f = 0; f < nfields; f++) printf("%c=%s ", fields[f].tag, fields[f].name);
    printf("P=padding\n");
}

int main(void) {
    struct FieldDesc human1_fields[] = {
        FIELD(human1_t, first_initial, 'F'),
        FIELD(human1_t, age,           'A'),
        FIELD(human1_t, height,        'H'),
        FIELD(human1_t, name,          'N'),
    };
    struct FieldDesc human2_fields[] = {
        FIELD(human2_t, name,          'N'),
        FIELD(human2_t, height,        'H'),
        FIELD(human2_t, age,           'A'),
        FIELD(human2_t, first_initial, 'F'),
    };

    visualize("Human1", sizeof(human1_t), human1_fields, sizeof(human1_fields)/sizeof(human1_fields[0]));
    visualize("Human2", sizeof(human2_t), human2_fields, sizeof(human2_fields)/sizeof(human2_fields[0]));

    // Quick compare summary
    printf("\nComparison:\n");
    printf("sizeof(human1_t) = %zu\n", sizeof(human1_t));
    printf("sizeof(human2_t) = %zu\n", sizeof(human2_t));
    printf("alignof(human1_t) = %zu\n", alignof(human1_t));
    printf("alignof(human2_t) = %zu\n", alignof(human2_t));
    printf("alignof(name_t)  = %zu\n", alignof(name_t));
    printf("alignof(double)  = %zu\n", alignof(double));
    printf("alignof(int)     = %zu\n", alignof(int));
    printf("alignof(char)    = %zu\n", alignof(char));

    return 0;
}