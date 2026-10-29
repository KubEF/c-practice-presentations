#include "distance.h"
#include <stdlib.h>

// своя abs возвращала x и при x < 0, поэтому берём abs из <stdlib.h>
int distance(int a, int b) {
    return abs(b - a);
}
