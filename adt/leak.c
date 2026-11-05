#include "list.h"

int main(void){
    while (1){
        list *l = list_create();
        for (int i = 0; i < 10000; i++){
            list_push_back(l, 10);
        }
        list_destroy(l);
    }
    return 0;
}
