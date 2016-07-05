#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mbr = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t br;
int check=0;

void test_cond();
void test_barrier();
void *fss(void *val);
void *fbr(void *val);
// #define RELEASE 0

int main(int c, char **v)
{
#ifdef RELEASE
	FILE *fs=fopen("/home/box/main.pid","w");
	fprintf(fs,"%d",getpid());
	fclose(fs);
#endif
	
	test_cond();
	test_barrier();

	return 0;
}
void test_cond()
{
	printf("***Test Condition***\n");
	pthread_t ft;
	printf("*:Created\n");
	if(pthread_create(&ft,NULL, fss, NULL) != 0) perror("*:ThreadCond");
	printf("*:Lock_1\n");
	pthread_mutex_lock(&mtx);
	printf("*:Sleep1\n");
	sleep(2);
    check = 1;
	printf("*:Unlock\n");
	pthread_mutex_unlock(&mtx);

while(check == 1){
	printf("*:Signal\n");
	pthread_cond_signal(&cond); 
	sleep(1);	
	}
//	pthread_cancel(st);
	printf("*:join_ft\n");
	if ((pthread_join(ft,NULL)!=0))perror("*:JoinFailed");
	printf("*:cond_destroy\n");
	pthread_cond_destroy(&cond);
	printf("*:mutex_destroy\n");
	pthread_mutex_destroy(&mtx);
}

void *fss(void *val)
{
	printf("-:Lock_1\n");
	if(pthread_mutex_lock(&mtx)!=0) perror("-:Err_Lock");
	printf("-:Wait:\n");
	pthread_cond_wait(&cond,&mtx);
	check = 0;
	printf("-:UnLock_1\n");
	if (pthread_mutex_unlock(&mtx)!=0) perror("fss:mutex_Unlock_1");

	printf("-:Exit\n");
	pthread_exit((void*)0);

}
void test_barrier()
{
	printf("***Test Barrier***\n");
	pthread_t bp;
	if(pthread_create(&bp,NULL, fbr,NULL) != 0) perror("+:ThreadCreateFailed");
	printf ("+:InitBarrier\n");
	pthread_barrier_init(&br,NULL, 1);
//	printf("+:Sleep\n");
//	sleep(2);
	printf("+:Join\n");
	if((pthread_join(bp,NULL)!=0)) perror("+:JoinFailed");
	printf("+:DestroyMutex\n");
	pthread_mutex_destroy(&mbr);
	printf("+:DestroyBarrie\n");
	pthread_barrier_destroy(&br);
	printf("+:Exit\n");
	pthread_exit(0);
		
}
void *fbr(void *val)
{
	printf("-:Sleep\n");
	sleep(1);
	printf("-:Wait\n");
	pthread_barrier_wait(&br);
	printf("-:WokenUP\n");
	
	printf("-:Lock\n");
	pthread_mutex_lock(&mbr);
	printf("-:SleepInsideLock\n");
	sleep(5);
	printf("-:UnLock\n");
	pthread_mutex_unlock(&mbr);
	printf("-:Exit\n");
	pthread_exit((void*)0);
}
