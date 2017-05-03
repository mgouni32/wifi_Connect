#include <errno.h>
#include <wiringPi.h>
#include <mcp3004.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>
#include <time.h>
//includes from 12/28/16 - hardware mac address
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
//end includes from 12/28/16


#define BASE 100
#define SPI_CHAN 0

//#define MQTT_HOSTNAME "localhost"
#define MQTT_HOSTNAME "mqtt"
#define MQTT_PORT 1883

void on_publish(struct mosquitto *mosq, void *userdata, int mid)
{
    mosquitto_disconnect(mosq);
}

int main(int argc, char *argv[])

{
    int chan = 0;
    int chan2 = 1;
    int x;
    int y;
    float v1;
    float v2;
    float c1;
    float c2;
    unsigned long long msepoch;
    char cont = 'y';
    
    printf ("Loading ADC.\n") ;
    
    if (wiringPiSetup () == -1){
        printf("Error loading WiringPi. Aborting.");
        exit (1);
    }
    
    mcp3004Setup (BASE, SPI_CHAN); // 3004 and 3008 are the same 4/8 channels
    
    //x = analogRead (BASE + chan);
    //y = analogRead (BASE + chan2);
    //printf("%d %d\n", x, y);
    //v1 = 3.3 * ((float)x/1024);
    //v2 = 3.3 * ((float)y/1024);
    //printf("%4.3f %4.3f\n", v1, v2);
    
    //printf("Read values again [y/n]: \n");
    //scanf("%c", &cont);
    
    struct mosquitto *mosq = NULL;
    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, true, NULL);
    mosquitto_connect(mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
    //mosquitto_publish_callback_set(mosq, on_publish);
    char flowval[10];
    char presval[10];
    char calpresval[10];
    char calflowval[10];
    char epochval[15];
    char hostname[50];
    char *devuuid;
    struct timespec tim, tim2;
    struct timeval tepoch;
    
    //12/28/16 addition
    //load cal file
    float flowcalslope;
    float flowcaloffset;
    float prescalslope;
    float prescaloffset;
    FILE *calfp;
    calfp = fopen ("/wiringPi/cal/localcal.txt", "r");
    if(calfp == NULL){
        perror("Could not open input file");
        return -1;
    }
    fscanf(calfp, "%f %f %f %f", &flowcalslope, &flowcaloffset, &prescalslope, &prescaloffset);
    fclose(calfp);
    //end load cal file
    int macfd;
    struct ifreq ifr;
    char *iface = "wlan0";
    unsigned char *mac;
    
    macfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
    
    ioctl(macfd, SIOCGIFHWADDR, &ifr);
    
    close(macfd);
    
    mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    //12/28/16 end addition
    
    tim.tv_sec = 0;
    tim.tv_nsec = 500000000L;
    
    //removed in 12/28/16 edit, replacing container UUID with MAC
    //gethostname(hostname, 50);
    //devuuid = strchr(hostname, '-')+1;
    //end 12/28/16 removal
    
    //int l = strlen(hostname)+1-(devuuid-hostname);
    //int p = devuuid-hostname+1;
    //sprintf("%d\n",l);
    //char uuid[l];
    //printf ("%d %d\n",devuuid-hostname, strlen(hostname));
    //int i;
    //for (i = 0; i < l; i++){
    //    uuid[i]=hostname[p];
    //    printf("%c ", hostname[p]);
    //    p++;
    //}
    //printf("%c\n", hostname[19]);
    
    while (cont == 'y' || cont == 'Y'){
        x = analogRead (BASE + chan);
        y = analogRead (BASE + chan2);
        //printf("%d %d\n", x, y);
        v1 = 5.26 * ((float)x/1024);
        v2 = 5.26 * ((float)y/1024);
        //add transform from cal file edit 1-18-17
        //adding new MQTT RAW stream 3-10-17
        c1 = flowcalslope * v1 + flowcaloffset;
        c2 = prescalslope * v2 + prescaloffset;
        //end 3-10-17 edit
        //end 1-18-17 edit
        //printf("%4.3f %4.3f\n", v1, v2);
        gettimeofday(&tepoch);
        msepoch = (unsigned long long) tepoch.tv_sec * 1000 + (unsigned long long) tepoch.tv_usec / 1000;
        sprintf(flowval, "%f", v1);
        sprintf(presval, "%f", v2);
        //adding new MQTT stream 3-10-17
        sprintf(calflowval, "%f", c1);
        sprintf(calpresval, "%f", c2);
        //end 3-10-17 edit
        sprintf(epochval, "%llu", msepoch);
        char *json;
        //adding new MQTT stream 3-10-17
        char *caljson;
        //end 3-10-17 edit
        //original publish (prior to 11/16/16)
        
        //12/28/16 removal
        //int sizejson = asprintf(&json, "{\"identifier\": \"%s\", \"flow\": %s, \"pressure\": %s, \"timeStamp\": %s}", devuuid, flowval, presval, epochval);
        //end 12/28/16 removal
        
        //12/28/16 addition
        int sizejson = asprintf(&json, "{\"identifier\": \"%.2x%.2x%.2x%.2x%.2x%.2x\", \"flow\": %s, \"pressure\": %s, \"timeStamp\": %s}", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], flowval, presval, epochval);
        //end 12/28/16 addition
        //adding new MQTT stream 3-10-17
        int calsizejson = asprintf(&caljson, "{\"identifier\": \"%.2x%.2x%.2x%.2x%.2x%.2x\", \"flow\": %s, \"pressure\": %s, \"timeStamp\": %s}", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], calflowval, calpresval, epochval);
        //end 3-10-17 edit
        
        //changed format for easy parsing
        //int sizejson = asprintf(&json, "%s\\;%s\\;%s\\;%s", devuuid, epochval, flowval, presval);
            //struct mosquitto *mosq = NULL;
            //mosquitto_lib_init();
            //mosq = mosquitto_new(NULL, true, NULL);
            //mosquitto_connect(mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
        mosquitto_publish(mosq, NULL, "adc", sizejson, json, 0, false);
        //adding new MQTT stream 3-10-17
        mosquitto_publish(mosq, NULL, "caladc", calsizejson, caljson, 0, false);
        //end 3-10-17 edit
        free(json);
        //adding new MQTT stream 3-10-17
        free(caljson);
        //end 3-10-17 edit
        nanosleep(&tim, &tim2);
        //sleep(1);
            //mosquitto_disconnect(mosq);
            //mosquitto_destroy(mosq);
    }
    
    //mosquitto_publish(mosq, NULL, "adc/pressure", 5, v2, 0, false);
    //mosquitto_loop_forever(mosq, -1, 1);  // this calls mosquitto_loop() in a loop, it will exit once the client disconnects cleanly
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    
    return 0 ;
}
