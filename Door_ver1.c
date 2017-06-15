#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <softPwm.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

#define SERVO 1

int Input;
int main ()
{
    int fd;
    int data;

    if(wiringPiSetup() == -1)
        return 1;

    softPwmCreate(SERVO, 0, 200);

    if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
        return 1;
    }

    printf ("\nRaspberry Pi UART Test");
    softPwmWrite(SERVO, 24);
    while(1)
    {
        data = serialGetchar(fd);
        //printf ("\nPC > RPi = %c", (char)data);
        //serialPutchar(fd, data);
        //serialPuts(fd, "\n");
        //scanf("%s",data);
        if(data != -1)
            printf("data = %d / (char)data = %c\n",data,(char)data);
        if(data == 49)
        {
            printf("OPEN!!!!\n");
            printf("servo = 5\n");

            softPwmWrite(SERVO, 10);
            //usleep(1000 * 800);
 //           sleep(1);
            //printf("CLOSE\n");
            //softPwmWrite(SERVO,24);
            //softPwmWrite(SERVO, 24);
            //softPwmWrite(SERVO, 5);
            fflush(stdout);
        }
        else if(data == 48)
        {
            softPwmWrite(SERVO, 24);
            //usleep(500000);
            //printf("servo = 24\n");
            //softPwmWrite(SERVO, 24);
            //printf("24 end");
            fflush(stdout);
        }
        else if(data == 54)
        {
            serialClose(fd);
            return 0;
        }
        else
        {
            printf("data = %d / (char)data = %c",data,(char)data);
            printf("Another Value\n");
            fflush(stdout);

        }
    }
    return 0 ;
}

