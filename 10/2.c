#include <stdlib.h>

typedef struct RandomGeneratorOperations RandomGeneratorOperations;
typedef struct RandomGenerator RandomGenerator;

int GenerateRandom(RandomGenerator *generator);
void RandomGeneratorDestructor(RandomGenerator *generator);
RandomGenerator *random_create(int seed);

enum {
    MULTIPLICATOR = 1103515245,
    INCREMENT = 12345,
    MOD = 1u << 31,
};

typedef struct RandomGeneratorOperations
{
    int (*next) (RandomGenerator *);
    void (*destroy) (RandomGenerator *);
} RandomGeneratorOperations;

typedef struct RandomGenerator
{
    unsigned long long state;
    const RandomGeneratorOperations *ops;
} RandomGenerator;

const RandomGeneratorOperations operations = { 
    .next = GenerateRandom,
    .destroy = RandomGeneratorDestructor,
};

RandomGenerator *random_create(int seed) {
    RandomGenerator *generator =  calloc(1, sizeof(RandomGenerator));
    if (generator == NULL) {
        return generator;
    }
    generator->ops = &operations;
    generator->state = seed;
    return generator;
}

int GenerateRandom(RandomGenerator *generator) {
    generator->state = (generator->state * MULTIPLICATOR + INCREMENT) % MOD;
    return generator->state;
}

void RandomGeneratorDestructor(RandomGenerator *generator) {
    free(generator);
}