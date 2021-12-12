/* 
    Function that look for longest line in file, ad returs its lenght.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Tuple {
    int lon, num;
};

struct Tuple Filevalues (FILE *const filename) {
    char str[100000],longest[100000];
    int len, cont;
    len = 0;
    cont = 0;
    while(fgets(str,sizeof(str),filename) != NULL) {
        if(len < strlen(str)) {
            len = strlen(str);      
        }
        if(str[0] == 'n' || str[0] == 'N'){
            cont++;
        }
    }
    struct Tuple tuple = {len, cont};
    return tuple;
}