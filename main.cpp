#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t br;

void *ffs(void *val);
void *fst(void *val);
// #define RELEASE 0

int main(int c, char **v)
{
#ifdef RELEASE
	FILE *fs=fopen("/home/box/main.pid","w");
	fprintf(fs,"%d",getpid());
	fclose(fs);
#endif
	pthread_t ft, st;
	printf("base:first created\n");
	if(pthread_create(&ft,NULL, ffs, NULL) != 0) perror("create_first");
	printf("base:mutex_lock_first\n");
	pthread_mutex_lock(&mtx);
	printf("base:sleep1\n");
	sleep(2);
	printf("base:mutex_unlock\n");
	pthread_mutex_unlock(&mtx);

	printf("base:cond_signal_1\n");
	pthread_cond_signal(&cond);
	printf("base:sleep2\n");
	sleep(2);
	printf("base:mutex_lock\n");
	pthread_mutex_lock(&mtx);
	printf("base:sleep3\n");
	sleep(3);
	printf("base:mutex_unlock\n");
	pthread_mutex_unlock(&mtx);

	printf("base:signal_2\n");
	pthread_cond_signal(&cond);
	sleep(5);
	printf("base:signal_3\n");
	pthread_cond_signal(&cond);

//	pthread_barrier_init(&br,NULL,1);
//	if(pthread_create(&st,NULL, fst,NULL) != 0) perror("create_second");


	printf("base:cancel_ft\n");
	pthread_cancel(ft);
//	pthread_cancel(st);
	printf("base:join_ft\n");
	pthread_join(ft,NULL);
	printf("base:cond_destroy\n");
	pthread_cond_destroy(&cond);
	printf("base:mutex_destroy\n");
	pthread_mutex_destroy(&mtx);
	return 0;
}

void *ffs(void *val)
{
	printf ("ffs\n");
	printf("ffs:mutex_Lock_1\n");
	if(pthread_mutex_lock(&mtx)!=0) perror("fss:mutex_lock_1");
	printf("ffs:wait\n");
	pthread_cond_wait(&cond,&mtx);
	printf("ffs:mutex_UnLock_1\n");
	if (pthread_mutex_unlock(&mtx)!=0) perror("fss:mutex_Unlock_1");

	printf("ffs:mutex_Lock_2\n");
	pthread_mutex_lock(&mtx);
	printf("ffs:wait_2\n");
	pthread_cond_wait(&cond,&mtx);
	printf("ffs:for\n");	
	for(int i=1;i<10;++i) { 
		printf("waiting condition, %d\n",i); 
//		sleep(1);
		}
	printf("ffs:mutex_UnLock_2\n");

	pthread_mutex_unlock(&mtx);

	printf("ffs:exit\n");
	pthread_exit((void*)0);
}

void *fst(void *val)
{
	printf("fst\n");
	return 0;
}
