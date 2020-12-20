#include "string.h"
#include "system.h"

static char * pad() { // need something in the string pool before "system.c"
    return ".";
}

#define MAX_ELEMENT 0xC00
#define MAX_ELEMENT_SIZE 0x40

void fsort(char ** table, size_t size);

static char * tmp0[MAX_ELEMENT]; // pointers to all items
static char tmp1[0x100]; // temporary copy of an item being swapped
static void * comp; // comparison function, passed to fsort indirectly
/*
void qqsort(char * list, size_t nel, size_t size, void * compare) {
    assert(nel < MAX_ELEMENT, "要素数が多すぎてバッファが足りません"); // "Too many elements and not enough buffer"
    assert(size < MAX_ELEMENT_SIZE, "１要素のサイズが大きくてバッファが足りません"); // "One element is too big and there is not enough buffer"
    comp = compare;
    if (nel <= 1) return;
    char * p = list;
    for (int i = 0; i < nel; i++) {
        tmp0[i] = p;
        p += size; 
    }
    fsort(tmp0, nel);

    char * item = list;
    for (int i = 0; i < nel; i++) {
        if (tmp0[i] != 0 && tmp0[i] != item) {
            int j = i;
            char * p = item;
            memcpy(tmp1, item, size);
            do {
                memcpy(p, tmp0[j], size);
                p = tmp0[j];
                int n = ((unsigned int) (p - list) / size) * 4;
                j = n;
                tmp0[j] = 0;
            } while (tmp0[j] != item);
            memcpy(p, tmp1, size);
        }
        item += size;
    }
}
*/

/*
void qqsort(char * list, size_t nel, size_t size, char * compare) {
    assert(nel < MAX_ELEMENT, "要素数が多すぎてバッファが足りません"); // "Too many elements and not enough buffer"
    assert(size < MAX_ELEMENT_SIZE, "１要素のサイズが大きくてバッファが足りません"); // "One element is too big and there is not enough buffer"
    comp = compare;
    if (nel > 1) {
        char * p = list;
        for (int i = 0; i < nel; i++) {
            tmp0[i] = p;
            p += size; 
        }
        fsort(tmp0, nel);
        char * item = list;
        for (int i = 0; i < nel; i++) {
            if (tmp0[i] != 0 && tmp0[i] != item) {
                char ** r22 = &tmp0[i];
                char * r21 = item;
                memcpy(tmp1, item, size);
                do {
                    memcpy(r21, *r22, size);
                    r21 = *r22;
                    unsigned int n = ((unsigned int) (r21 - list) / size) * 4;
                    *r22 = 0;
                    r22 = tmp0 + n;
                } while (*r22 != item);
                memcpy(r21, tmp1, size);
                *r22 = 0;
            }
            item += size;
        }
    }
}
*/

/*
void qqsort(char * list, size_t nel, size_t size, char * compare) {
    assert(nel < MAX_ELEMENT, "要素数が多すぎてバッファが足りません"); // "Too many elements and not enough buffer"
    assert(size < MAX_ELEMENT_SIZE, "１要素のサイズが大きくてバッファが足りません"); // "One element is too big and there is not enough buffer"
    comp = compare;
    if (nel > 1) {
        for (int i = 0; i < nel; i++) {
            tmp0[i] = compare;
            compare += size; 
        }
        fsort(tmp0, nel);
        char * item = list;
        char ** r24 = tmp0;
        for (int i = 0; i < nel; i++) {
            if (*r24 != 0 && *r24 != item) {
                char ** r22 = r24;
                char * r21 = item;
                memcpy(tmp1, item, size);
                do {
                    memcpy(r21, *r22, size);
                    unsigned int n = ((unsigned int) (*r22 - list) / size) * 4;
                    *r22 = 0;
                    r22 = tmp0 + n;
                } while (*r22 != item);
                memcpy(r21, tmp1, size);
            }
            item += size;
            r24++;
        }
    }
}
*/