#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define SYS_GPIO   "/sys/class/gpio"

void delay(uint32_t del) {
     for(int i=0; i<del;  i++)
                for(uint32_t j=0; j<50000; j++);
}

int device_export(uint16_t pin)  {
    FILE *fp  =  fopen(SYS_GPIO "/export", "w");

    if(fp == NULL) {
       printf("%s/export\n",SYS_GPIO);
       perror("Error Opening export file,");
       return  EXIT_FAILURE;
    }

    fprintf(fp, "%d", pin);
    fclose(fp);

    return EXIT_SUCCESS;
}


int device_unexport(uint16_t pin)  {
    FILE *fp  =  fopen(SYS_GPIO "/unexport", "w");

    if(fp == NULL) {
       printf("%s/unexport\n",SYS_GPIO);
       perror("Error Opening unexport file");
       return  EXIT_FAILURE;
    }

    fprintf(fp, "%d", pin);
    fclose(fp);

    return EXIT_SUCCESS;
}

int device_direction(uint16_t pin, uint8_t* direction) {
    uint8_t* buf = (uint8_t*)calloc(64,sizeof(uint8_t));

    sprintf(buf, "%s/gpio%d/direction", SYS_GPIO, pin);
    FILE *fp = fopen(buf, "w");

    if(fp == NULL) {
       perror("Error Opening direction file");
       return  EXIT_FAILURE;
    }

    fprintf(fp, "%s", direction);
    fclose(fp);
    free(buf);
    return EXIT_SUCCESS;
}

int device_value(uint16_t pin, uint8_t value) {
    uint8_t* buf = (uint8_t*)calloc(64,sizeof(uint8_t));

    sprintf(buf, "%s/gpio%d/value", SYS_GPIO, pin);
    printf("%s = %c \n", buf, value);
    FILE *fp = fopen(buf, "w");

    if(fp == NULL) {
       perror("Error Opening value file");
       return  EXIT_FAILURE;
    }

    fprintf(fp, "%c", value);
    fclose(fp);
    free(buf);
    return EXIT_SUCCESS;
}


int main(void) {

    int ret =  device_export(78);
    if(ret == EXIT_FAILURE)
         exit(EXIT_FAILURE);

    ret = device_direction(78, "out");
    if(ret == EXIT_FAILURE)
         exit(EXIT_FAILURE);


    while(1) {
         ret = device_value(78, '0');
         if(ret == EXIT_FAILURE)
                exit(EXIT_FAILURE);
         delay(1000);

         ret = device_value(78, '1');
         if(ret == EXIT_FAILURE)
                exit(EXIT_FAILURE);
         delay(1000);

    }

    exit(EXIT_SUCCESS);
}
