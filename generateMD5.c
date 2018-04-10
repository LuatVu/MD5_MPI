// to compile: gcc generateMD5.c -o generateMD5 -I./ MD5.c -lm
// to run ./generateMD5 <key_length> <key>

#include <MD5.h>

int main(int argc, char **argv){
    if(argc != 3) return -1;

    int key_length; 
    char *key;

    sscanf(argv[1], "%d", &key_length);
    key = (char *)malloc( (key_length +1 )*sizeof(char) );

    int i; 
    for(i=0; i<key_length; i++){
        key[i] = argv[2][i];
    }
    key[key_length] = '\0';
    int *MD5_hash = md5_int(key);

    for(i=0; i<16; i++){
        printf("%02x", MD5_hash[i]);
    }
    printf("\n");
    return 0;
}
