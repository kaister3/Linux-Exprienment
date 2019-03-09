#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 128

int main(){
    int testdevice;
    int i;
    char buf[BUF_SIZE];
    testdevice = open("/dev/testDev", O_RDWR | O_NONBLOCK);
    if(testdevice == -1){
        printf("Cannot open file\n");
        return -1;
    }
    
    for(;; i++){
        read(testdevice, buf, BUF_SIZE);//读取初始信息
        printf("Input : ");
        scanf("%s", buf);
        write(testdevice, buf, BUF_SIZE);
        read(testdevice, buf, BUF_SIZE);
        printf("Output : %s\n", buf);
        close(testdevice);
    }
    return 0;
}
