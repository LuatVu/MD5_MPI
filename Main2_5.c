
// --------------------------------   FINAL VERSION -----------------------------------------------------------\\

// to compile: mpicc -o Main2_5 Main2_5.c -I./ MD5.c -lm
// to run(4 is arbitrary value):     mpirun -np <number-process> Main2_5 <key-length> <message-digest>

#include <mpi.h>
#include <stdio.h>
#include <MD5.h>

#define tag_1 1
#define tag_2 2
#define tag_3 3
#define tag_4 4

void try(int k, int start, int finish, char *alphabet, int alpha_length,char *key, int key_length,int *msg);
char *rank0(int *msg,int key_length);
char *ranki(int *msg,int key_length);
int checkResult(int *result, int *msg);
void displayKey(char *key);

int main(int argc, char **argv){

    if(argc != 3) return -1;
   

    int rank;
    char hostname[50];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    gethostname(hostname, 50);

    printf("\n");
    printf("Rank: %d, host: %s\n", rank, hostname);
    printf("\n");
    
    // phan du lieu dau vao
    int key_length; 
     
    sscanf(argv[1],"%d", &key_length);
   
    int *MD5_hash = HexaToDecimal_Msg(argv[2]);


    if(rank == 0){
       
        rank0(MD5_hash, key_length);

    }else{

        ranki(MD5_hash, key_length);

    }   
            
    MPI_Finalize();

    return 0;
}

void displayKey(char *key){
    int index = 0;
    printf("\n");
    printf("Password: ");
    while( key[index] != '\0'){
        printf("%c", key[index++]);
    }
    printf("\n");
}

char *rank0(int *msg, int key_length){
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int alpha_length = sizeof(alphabet)-1;// eliminate element '\0'

    int np, ne; //np: number of process, ne: number of element.
    MPI_Status status;
    char *key = (char *)malloc( (key_length + 1)*sizeof(char) );

    MPI_Comm_size(MPI_COMM_WORLD, &np);
    ne = alpha_length / (np-1); // khong tinh tien trinh 0, tien trinh 0 
                                // su dung khi con thua khong gian

    int i;
    for(i=1; i<np;i++){
        int start = (i-1)*ne; // start bat dau tu 0.

        MPI_Send(&ne, 1, MPI_INT, i, tag_1, MPI_COMM_WORLD);
        MPI_Send(&start, 1, MPI_INT, i, tag_2, MPI_COMM_WORLD);
        MPI_Send(&alpha_length, 1, MPI_INT, i, tag_3, MPI_COMM_WORLD);
        MPI_Send(alphabet, alpha_length, MPI_CHAR, i, tag_4, MPI_COMM_WORLD);
    }

    int start_last = (np-1 - 1)*ne +ne;
    if(start_last <= (alpha_length-1) ){
        try(0, start_last, alpha_length-1, alphabet, alpha_length, key,key_length, msg);
        free(key);
    }
    return NULL;
}

char *ranki(int *msg, int key_length){
    char *alphabet;
    int alpha_length;

    char *key = (char *)malloc((key_length +1 )*sizeof(char));

    MPI_Status status;
    int ne, start, finish;

    MPI_Recv(&ne, 1, MPI_INT, 0, tag_1, MPI_COMM_WORLD, &status);
    MPI_Recv(&start, 1, MPI_INT, 0, tag_2, MPI_COMM_WORLD, &status);
    MPI_Recv(&alpha_length, 1, MPI_INT, 0, tag_3, MPI_COMM_WORLD, &status);

    alphabet = (char *)malloc( (alpha_length+1) *sizeof(char));
    MPI_Recv(alphabet, alpha_length+1, MPI_CHAR, 0,tag_4, MPI_COMM_WORLD, &status);
    alphabet[alpha_length] = '\0'; //bonus.

    finish = start + ne -1;
    try(0, start, finish, alphabet, alpha_length, key, key_length, msg);
    free(key);
    return NULL;
}


int checkResult(int *result, int *msg){  
    int i;
    for(i =0; i<16; i++){
        if( result[i] != msg[i] ){
            return 0;
        }
    }
    return 1;
}

void try(int k, int start, int finish, char *alphabet, int alpha_length, char *key, int key_length, int *msg){
    int *MD5_hash;    
    int i;
    
    if(k==0){
        for(i= start; i<= finish; i++){
            key[k] = alphabet[i];
            try(k+1, start, finish, alphabet, alpha_length, key, key_length, msg);
        }
    }else{
        for(i = 0; i < alpha_length; i++){
            key[k] = alphabet[i];

            if(k == (key_length - 1)){
                key[key_length] = '\0';
                MD5_hash = md5_int(key);                
                if(checkResult(MD5_hash, msg)){	
                    free(MD5_hash);

                    displayKey(key);
                    return;
                }

                free(MD5_hash);
            }else{
                try(k+1, start, finish, alphabet, alpha_length, key, key_length, msg);
            }
        }
    }
}
