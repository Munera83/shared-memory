#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/wait.h>

void write_to(){
printf("Writing to shared memory...\n");
printf("Current process id: %ld\n",(long)getpid());
const int SIZE=4096;
const char* name="OS";

char* data;
printf("Enter data: ");
fgets(data,10,stdin);//fill varible data with 

int shm_fd=shm_open(name,O_CREAT|O_RDWR, 0666);//create shared memory and make it read+write
ftruncate(shm_fd,SIZE);//specify size 
void* ptr=mmap(0,SIZE,PROT_WRITE,MAP_SHARED,shm_fd,0);//map shared memory to ptr

sprintf(ptr,"%s",data);//add data to shared memory
ptr+=strlen(data);
}

void read_from(){
printf("\nReading from shared memory...\n");
printf("Current process id: %ld\n",(long)getpid());
const int SIZE=4096;
const char* name="OS";


int shm_fd=shm_open(name,O_RDONLY, 0666);//open shared memory

void* ptr=mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);//map memory to shared memory object
printf("Read data: ");
printf("%s",(char*)ptr);
shm_unlink(name);//delete shared memory object
}


int main(){

printf("Main process id: %ld\n",(long)getpid());

pid_t p=fork();
if (p==0){//child proccess
   write_to();
    exit(11);}
else 
    wait(NULL);

pid_t k=fork();
if (k==0){
   read_from();
   exit(12);}
else
   wait(NULL);

printf("\nbye\n");
return 0;
}
