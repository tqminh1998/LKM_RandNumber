#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEV_DIR "/dev/randmodule"

int main(){
    int fd;
    fd = open(DEV_DIR, O_RDWR); 

    if (fd < 0){
        printf("can't open device :'(");
        return 0;
    }
    int a=0;
    int ret = read(fd, &a, 256);
    
    printf("Random number: %d",a);
    return 0;
}