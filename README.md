# MD5_MPI
use brute force to decrype MD5 code. Technique is used distributed computing, Technology use MPI, rockCluster

Project Structure contains: 
    1, generateMD5.c  // using to generate MD5 Hash. 
                    // to compile: gcc generateMD5.c -o generateMD5 -I./ MD5.c -lm
                    // to run: ./generateMD5 [key_lenght] [key]
                    // [] is argument user pass.
                    // Ouput is MD5 Hash code 32 character (Hexa format).

    2, Main2_5.c    // main program. 
                    // to compile: user only typing:    make
                    // when user type 'make' makefile will compile Main2_5.c to Main2_5 executed file
                    // to run, has to type: 
                        // + run multi process on computer: 
                                   mpirun -np [number_process] Main2_5 [key_length] [message_digest]
                        // + run multi process on multi computer: 
                                suppose you have 3 three compute node: compute-0-0, compute-0-1, compute-0-2 and one cluster
                                first, you should create a file, file name is 'machines'. And create file content for 'machine' file same follow: 
                                                                                compute-0-0
                                                                                compute-0-1
                                                                                compute-0-2
                                                                                cluster
                                For detail step by step, you can access follow link: http://www.rocksclusters.org/assets/usersguides/roll-documentation/hpc/6.2/

                                command to run on multi computer:
                                mpirun -np [number_process] -machinefile machines Main2_5 [key_length] [message_digest]

    3, makefile // to compile main program
    4, Readme.md
    5, MD5.c
    6, MD5.h
    


                             
