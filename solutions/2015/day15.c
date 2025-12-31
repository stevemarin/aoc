
/*

Sprinkles: capacity 2, durability 0, flavor -2, texture 0, calories 3
Butterscotch: capacity 0, durability 5, flavor -3, texture 0, calories 3
Chocolate: capacity 0, durability 0, flavor 5, texture -1, calories 8
Candy: capacity 0, durability -1, flavor 0, texture 5, calories 8

[
  2  0 -2  0  3
  0  5 -3  0  3
  0  0  5 -1  8
  0 -1  0  5  8
]

Sprinkles + Butterscotch + Chocolate + Candy = 100
Butterscotch > Candy * 5
5 * Chocolate > 3 * Butterscotch + 2 * Sprinkles
Candy > Chocolate * 5

Butterscotch > 25 * Chocolate

Sprinkles + Butterscotch + 6 * Butterscotch + 6 Sprinkles = 100

*/

#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    int max1 = 0, max2 = 0;
    for (int chocolate = 0; chocolate <= 100; chocolate++) {
        for (int candy = 0; candy + chocolate <= 100; candy++) {
            for (int butterscotch = 0; butterscotch + chocolate + candy <= 100; butterscotch++) {
                int sprinkles = 100 - chocolate - candy - butterscotch;
                assert(chocolate + candy + butterscotch + sprinkles == 100);

                int capacity = 2 * sprinkles;
                int durability = 5 * butterscotch - candy;
                int flavor = -2 * sprinkles - 3 * butterscotch + 5 * chocolate;
                int texture = -1 * chocolate + 5 * candy;
                int calories = 3 * sprinkles + 3 * butterscotch + 8 * chocolate + 8 * candy;

                capacity = capacity > 0 ? capacity : 0;
                durability = durability > 0 ? durability : 0;
                flavor = flavor > 0 ? flavor : 0;
                texture = texture > 0 ? texture : 0;

                int score = capacity * durability * flavor * texture;
                max1 = score > max1 ? score : max1;
                max2 = (calories == 500) && (score > max2) ? score : max2;
            }
        }
    }

    printf("best overall score: %i\n", max1);
    assert (max1 == 21367368);

    printf("best with 500 calories: %i\n", max2);
    assert (max2 == 1766400);

    return 0;
}