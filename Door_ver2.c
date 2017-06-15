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
int fd;
int Input;
int check = 0;
int out = 0;

void* moto()
{
    int data;
    
    printf("IN Motor \n");

    while(out == 0)
    {
        printf("while in motor\n");

        data = serialGetchar(fd);

        if(check == 0)
        {
            printf("if in motor \n");

            if(data == 54)
            {
                serialClose(fd);
                out =1;
            }


            if(data != -1 && data != 54 && data == 49)
            {
                printf("check chage");
                //Input = data;
                check = 1;
                return data;
            }
        }
    }
}
int main ()
{
    int ThreadIn;
    void* s;
    pthread_t Motor;


    if(wiringPiSetup() == -1)
        return 1;

    if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0)
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
        out = 1;
    }


    softPwmCreate(SERVO, 0, 200);

    printf ("\nRaspberry Pi UART Test");
    softPwmWrite(SERVO, 24);
       
    ThreadIn = pthread_create(&Motor,NULL,moto,0);
    printf("ThreadIn = %d   ",ThreadIn);

    while(out == 0)
    {
        pthread_join(Motor,(void **)&Input);
        
        printf("Input : %d\n", Input);

        if(check==1)
        {
            if(Input == 49)
            {
                printf("OPEN    \n");
                softPwmWrite(SERVO, 10);
                sleep(2);
                printf("CLOSE\n");
                softPwmWrite(SERVO,24);
                fflush(stdout);
                check = 0;
                ThreadIn = pthread_create(&Motor,NULL,moto,0);
            }
            //else

            /*
               if(data != -1)
               {
               Input = data;
               check = 0;
               ThreadIn = pthread_create(&Motor,NULL,moto,0);
               }
             */
        }
        printf("here is main\n");
        sleep(2);
    }
    return 0 ;
}

