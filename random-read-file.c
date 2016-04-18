#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "sys/time.h"
#include "string.h"

int main(int argc, char * argv[])
{

	char file_name[100];
	char *buf;
	FILE *fp;
	int loop_size = 1;
	int i = 0, j = 0;
	long req_size = 1 * 1024;
	long file_size = 0;
	struct timeval tv0, tv1;
	struct timezone tz0, tz1;
	double time;

	int ret;

	if(argc != 3){
		printf("Command format: %s file_name req_size(KB)\n", argv[0]);
		exit(0);
	}

	strcpy(file_name, argv[1]);
	fp = fopen(file_name, "r");
	if(fp == NULL){
		printf("Error: cannot open %s\n", file_name);
		exit(1);
	}

	req_size = atoi(argv[2]) * 1024;
	buf = (char *) malloc(sizeof(char) * req_size);
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	loop_size = ( file_size + req_size ) / req_size - 1;
	
	ret = gettimeofday(&tv0, &tz0);
	if(ret)
		printf("Fail to get time!\n");
	
	srand(20);

	for(i = 0; i < loop_size; i++){
		j = (int) rand() % loop_size;		
//		printf("random j is %d\n", j);
		fseek(fp, req_size*j, SEEK_SET);
		fread(buf, req_size, 1, fp);
/*
		if(i%2 == 0){
			j = i/2;
			fseek(fp, req_size*j, SEEK_SET);
			fread(buf, req_size, 1, fp);
		}
		else{
			j = i/2 + 1;
			fseek(fp, req_size*j, SEEK_END);
			fread(buf, req_size, 1, fp);
		}
*/		
	}

	ret = gettimeofday(&tv1, &tz1);
        if(ret)
                printf("Fail to get time!\n");
	
	time = ((tv1.tv_sec - tv0.tv_sec) * 1000000 + tv1.tv_usec - tv0.tv_usec) * 1.0 / 1000000;
	printf("random:Running time : %f s, I/O throughput : %f MB/s\n", time, file_size * 1.0 / (time * 1024 * 1024));
	free(buf);
	fclose(fp);

	return 0;
}
