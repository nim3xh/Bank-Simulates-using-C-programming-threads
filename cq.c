#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> //Header file for sleep
#include<time.h>
#include<pthread.h> //Header file for threads
#include<string.h>

typedef struct customer{
    int cusID; //customer Id
    char ServiceType; //W,D,I
    char arr_time[50]; //Customer Arrival Time
}cus;

typedef struct teller{
    int tellerID; //Teller ID
    pthread_t thread; //Teller thread
    //int cus_count; //no of customers that came to teller
}tel;

//Global Variables
int m=5;//size of customer queue
int tc,tw,td,ti; //time durations
int c_queue[1000];
int cq_front=-1;
int cq_rear=-1;
FILE *c_file;
FILE *r_log;
time_t currentTime;
char time_string[50];
cus c;
tel t1 = {1, 0};
tel t2 = {2, 0};
tel t3 = {3, 0};
tel t4 = {4, 0};
pthread_mutex_t cq_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cq_cond=PTHREAD_COND_INITIALIZER;
int t1Served=0,t2Served=0,t3Served=0,t4Served=0; //to get teller served count
int totalCustomers = 0; //for count total number of customers that insert to c_queue


//Function prototypes
void *cq(void *arg);
void *customer(void *args);
void enQueue(int cusID);
void deQueue();
void *teller(void *args);
void ServedCustomers();
void termination();
void tellerDetails();
void arrivalTime();
void customerCompletion(int tID,int cusID);
void customerResponse(int tID,int cusID);

/*int main()
{
	pthread_t teller1,teller2,teller3,teller4,customerThread;
	tel t1={1,teller1};
	tel t2={2,teller2};
	tel t3={3,teller3};
	tel t4={4,teller4};
	
	pthread_create(&customerThread, NULL, customer, NULL);
	pthread_create(&teller1,NULL,teller,&t1);
	pthread_create(&teller2,NULL,teller,&t2);
	pthread_create(&teller3,NULL,teller,&t3);
	pthread_create(&teller4,NULL,teller,&t4);
	
	
	pthread_join(customerThread, NULL);
	pthread_join(teller1,NULL);
	pthread_join(teller2,NULL);
	pthread_join(teller3,NULL);
	pthread_join(teller4,NULL);
	
	//termination();
    
    	return 0;
}*/

int main(int argc,char *argv[])
{
	if(argc!=6)
	{
		printf("You should run like this!\n");
		printf("./ProgramName m tc tw td ti\n");
		printf("m :length of customer queue\n");
		printf("tc:Periodic time for the customer to arrive\n");
		printf("tw:Time duration to serve Withdrawal\n");
		printf("td:Time duration to serve Deposit\n");
		printf("ti:Time duration to serve Information\n");
		return 1;
	}
	
	m=atoi(argv[1]);
	tc=atoi(argv[2]);
	tw=atoi(argv[3]);
	td=atoi(argv[4]);
	ti=atoi(argv[5]);
	
	pthread_t teller1,teller2,teller3,teller4,customerThread;
	tel t1={1,teller1};
	tel t2={2,teller2};
	tel t3={3,teller3};
	tel t4={4,teller4};
	
	pthread_create(&customerThread, NULL, customer, NULL);
	pthread_create(&teller1,NULL,teller,&t1);
	pthread_create(&teller2,NULL,teller,&t2);
	pthread_create(&teller3,NULL,teller,&t3);
	pthread_create(&teller4,NULL,teller,&t4);
	
	
	pthread_join(customerThread, NULL);
	/*pthread_join(teller1,NULL);
	pthread_join(teller2,NULL);
	pthread_join(teller3,NULL);
	pthread_join(teller4,NULL);*/
	
	//termination();
    
    	return 0;
}


void enQueue(int cusID)
{
	//printf("\n%d",cusID);
	/*if(cq_rear==m-1)
	{
		printf("\nQueue is full");
	}
	else
	{
		//printf("\ncq_front :%d ",cq_front);
		
		if(cq_front==-1)
		{
			cq_front=0; 
		}
		cq_rear++;
		c_queue[cq_rear]=cusID;
		//printf("\nQueue check :%d",c_queue[cq_rear]);
		//pthread_cond_signal(&cq_cond);
	}*/
	
	//printf("\ncq_front :%d ",cq_front);
		
	if(cq_front==-1)
	{
		cq_front=0; 
	}
	cq_rear++;
	c_queue[cq_rear]=cusID;
	//printf("\nQueue check :%d",c_queue[cq_rear]);
	//pthread_cond_signal(&cq_cond);
	
}

void deQueue()
{
	/*if(cq_front==-1 || cq_front>cq_rear)
	{
		printf("\nQueue is empty");
		exit(0);
	}
	else
	{
		cq_front++;
	}*/
	cq_front++;
}


/*void *customer(void *args)
{
    FILE *c_file = fopen("c_file.txt", "r");
    if (c_file == NULL) {
        printf("Failed to open c_file.txt\n");
        exit(0);
    }
    
    pthread_mutex_lock(&cq_mutex); //cq mutex lock
    
    char line[100];
    while (fgets(line, sizeof(line), c_file) != NULL) {
        sscanf(line, "%d %c", &c.cusID, &c.ServiceType);
        printf("\nCustomer %d", c.cusID);
        //if(c.cusID==m){
        //    termination();
        //}
        enQueue(c.cusID);
        pthread_cond_signal(&cq_cond);
        sleep(1);
        
        FILE *r_log = fopen("r_log.txt", "a");
        if (r_log == NULL) {
            printf("Failed to open r_log.txt\n");
            exit(0);
        }
        
        time_t currentTime = time(NULL);
        sleep(2);
        char time_string[50];
        strftime(time_string, 50, "%H:%M:%S", localtime(&currentTime));
        strcpy(c.arr_time, time_string);
        
        fprintf(r_log, "Customer %d: %c\n", c.cusID, c.ServiceType);
        fprintf(r_log, "Arrival time: %s\n", c.arr_time); //added time string to output
        fprintf(r_log, "-----------------------------------\n");
        pthread_mutex_unlock(&cq_mutex); //cq mutex unlock
        fclose(r_log);
    }
    
    fclose(c_file);
    
}*/

void *customer(void *args)
{
    c_file=fopen("c_file.txt","r");
    if (c_file == NULL) 
    {
        printf("Failed to open c_file.txt\n");
        exit(0);
    }
    pthread_mutex_lock(&cq_mutex); //cq mutex lock
    while(fscanf(c_file,"%d %c",&c.cusID, &c.ServiceType)!=EOF)
    {
    	totalCustomers++;
    	//printf("\nTot_Customer %d",totalCustomers);
    	//printf("\nCustomer %d",c.cusID);
    	/*if(m<totalCustomers)
    	{
    		exit(0);
    	}*/
    	arrivalTime();
    	enQueue(c.cusID);
    	sleep(1);
    	pthread_cond_signal(&cq_cond);
    	pthread_mutex_unlock(&cq_mutex); //cq mutex unlock
        
       /* if(totalCustomers==m)
	{
		tellerDetails();
    		termination();
    	}*/
    }
    if(c.ServiceType=='W'){
        sleep(tw);
        //printf("widthdraw time\n");
	}else if(c.ServiceType=='D'){
        sleep(td);
        //printf("deposit time\n");
        }else if(c.ServiceType=='I'){
        sleep(ti);
        //printf("info time\n");
        }
    tellerDetails();
    termination();
    fclose(c_file);
}

void *teller(void *args)
{
	tel *t=(tel*)args;
	int tID=t->tellerID;
	//printf("\n%d",tID);
	
	int cusID=0;
	char ServiceType=' ';
	
	pthread_mutex_lock(&cq_mutex); //cq mutex lock
	while(1)
	{
		if(cq_front==-1 || cq_front>cq_rear)
		{
			
			pthread_cond_wait(&cq_cond, &cq_mutex);
			pthread_mutex_unlock(&cq_mutex); //cq mutex unlock
			continue;
		}else{
			pthread_mutex_unlock(&cq_mutex); //cq mutex unlock
		}
		
		//pthread_mutex_unlock(&cq_mutex); //cq mutex unlock	
		cusID = c_queue[cq_front];
		//printf("\nTeller customer id check :%d",c_queue[cq_front]);
		deQueue();
		//pthread_mutex_unlock(&cq_mutex);
		//sleep(1);
		//printf("\nCustomer %d",cusID);
		
		//printf("\nTeller , Queue check: %d",c_queue[cq_front]);
		
		// Increment the value in the tellerServedCount array
        	if (tID == 1) {
            		t1Served++;
            		//printf("\nt1Served %d",t1Served);
       	} else if (tID == 2) {
            		t2Served++;
            		//printf("\nt2Served %d",t1Served);
        	} else if (tID == 3) {
            		t3Served++;
            		//printf("\nt3Served %d",t1Served);
        	} else if (tID == 4) {
            		t4Served++;
            		//printf("\nt4Served %d",t1Served);
        	}
		customerResponse(tID,cusID);
	}
}

/*void ServedCustomers(int tID)
{
	switch(tID)
	{
		case 1:
			tellerServedCount[0]=t1Served++;
			//printf("\nt1Served %d",t1Served);
			break;
		case 2:
			tellerServedCount[1]=t2Served++;
			//printf("\nt2Served %d",t2Served);
			break;
		case 3:
			tellerServedCount[2]=t3Served++;
			//printf("\nt3Served%d",t3Served);
			break;
		case 4:
			tellerServedCount[3]=t4Served++;
			//printf("\nt4Served%d",t4Served);
			break;
	}
}*/

void termination()
{
	while (1) 
	{
		// Check if all the customers have been served
		if (t1Served + t2Served + t3Served + t4Served == totalCustomers)
		{
			// Terminate the program
			//printf("\nAll customers served. Terminating tellers.\n");s
		
			pthread_cancel(t1.thread);
			pthread_cancel(t2.thread);
			pthread_cancel(t3.thread);
			pthread_cancel(t4.thread);
			//exit(0);
			break;
		}
		else
		{
			// Wait for a while before checking again
			sleep(1);
		}
	}
}

void arrivalTime()
{
	r_log=fopen("r_log.txt","a");
        if (r_log == NULL) 
        {
        	printf("Failed to open r_log.txt\n");
        	exit(0);
    	}
        currentTime=time(NULL);
        //sleep(1);
        strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
        strcpy(c.arr_time,time_string);
        fprintf(r_log,"Customer %d: %c\n",c.cusID,c.ServiceType);
        //sleep(tc);
        //printf("response time\n");
        fprintf(r_log,"Arrival time: %s\n",c.arr_time); //added time string to output
        fprintf(r_log,"-----------------------------------\n");
        fclose(r_log);
}

void customerResponse(int tID,int cusID)
{
	FILE *f;
	f=fopen("r_log.txt","a");
	if(r_log==NULL)
	{
		printf("Failed to open r_log.txt file");
		exit(0);
	}
	currentTime=time(NULL);
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(f,"Teller: %d\n",tID);
	fprintf(f,"Customer: %d\n",cusID);
	//printf("\nCustomer %d",cusID);
	char timeString[50];
	strcpy(timeString,c.arr_time);
	fprintf(f,"Arrival time: %s\n",timeString);
	fprintf(f,"Response time: %s\n",time_string);
	fprintf(f,"-----------------------------------\n");
	if(c.ServiceType=='W'){
        sleep(tw);
        //printf("widthdraw time\n");
	}else if(c.ServiceType=='D'){
        sleep(td);
        //printf("deposit time\n");
        }else if(c.ServiceType=='I'){
        sleep(ti);
        //printf("info time\n");
        }
	currentTime=time(NULL);
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(f,"Teller: %d\n",tID);
	fprintf(f,"Customer: %d\n",cusID);
	//printf("\nCustomer %d",cusID);
	fprintf(f,"Arrival time: %s\n",c.arr_time);
	fprintf(f,"Completion time: %s\n",time_string);
	fprintf(f,"-----------------------------------\n");
	fclose(f);
	//fclose(r_log);
	//customerCompletion(tID,cusID);
}
void customerCompletion(int tID,int cusID)
{
	r_log=fopen("r_log.txt","a");
	if(r_log==NULL)
	{
		printf("Failed to open r_log.txt file");
		exit(0);
	}
	currentTime=time(NULL);
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(r_log,"Teller: %d\n",tID);
	fprintf(r_log,"Customer: %d\n",cusID);
	//printf("\nCustomer %d",cusID);
	fprintf(r_log,"Arrival time: %s\n",c.arr_time);
	fprintf(r_log,"Completion time: %s\n",time_string);
	fprintf(r_log,"-----------------------------------\n");
	//fclose(r_log);
}

void tellerDetails()
{
	r_log=fopen("r_log.txt","a");
	if(r_log==NULL)
	{
		printf("Failed to open r_log.txt file");
		exit(0);
	}
	currentTime=time(NULL);
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(r_log,"Termination: teller-%d\n",t1.tellerID);
	fprintf(r_log,"#served customers: %d\n",t1Served);
	fprintf(r_log,"Start time: %s\n",time_string);
	fprintf(r_log,"Termination time: %s\n",time_string);
	fprintf(r_log,"-----------------------------------\n");
	
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(r_log,"Termination: teller-%d\n",t2.tellerID);
	fprintf(r_log,"#served customers: %d\n",t2Served);
	fprintf(r_log,"Start time: %s\n",time_string);
	fprintf(r_log,"Termination  time: %s\n",time_string);
	fprintf(r_log,"-----------------------------------\n");
	
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(r_log,"Termination: teller-%d\n",t3.tellerID);
	fprintf(r_log,"#served customers: %d\n",t3Served);
	fprintf(r_log,"Start time: %s\n",time_string);
	fprintf(r_log,"Termination  time: %s\n",time_string);
	fprintf(r_log,"-----------------------------------\n");
	
	strftime(time_string,50,"%H:%M:%S",localtime(&currentTime));
	fprintf(r_log,"Termination: teller-%d\n",t4.tellerID);
	fprintf(r_log,"#served customers: %d\n",t4Served);
	fprintf(r_log,"Start time: %s\n",time_string);
	fprintf(r_log,"Termination time: %s\n",time_string);
	fprintf(r_log,"-----------------------------------\n");
	
	fprintf(r_log,"Teller Statistic\n");
	fprintf(r_log,"Teller-%d serves %d customers\n",t1.tellerID,t1Served);
	fprintf(r_log,"Teller-%d serves %d customers\n",t2.tellerID,t2Served);
	fprintf(r_log,"Teller-%d serves %d customers\n",t3.tellerID,t3Served);
	fprintf(r_log,"Teller-%d serves %d customers\n",t4.tellerID,t4Served);
	
	fclose(r_log);
}
