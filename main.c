#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

const int MAX_LIST = 256;
FILE *cfd;

struct pstats_t {
	int t, s, f;
	double r;
};
struct mp3s {
	char **list;
	int size;
};
struct mp3s getmp3s(char *dir)
{
	char cmd[256];
	char *buf = malloc(1024);
	FILE *fd;
	struct mp3s out;

	out.size = 0;
	out.list = NULL;

	sprintf(cmd, "ls *.mp3 *.MP3 2> /dev/null", dir);
	fd = popen(cmd, "r");
	if (fd == NULL) {
		perror("getmp3s popen");
		exit(-1);
	}

	if (0 == fread(buf, 1, 1024, fd)) {
		perror("getmp3s read");
		exit(-1);
	}

	//alloc
	out.list = malloc(MAX_LIST * sizeof(char *));

	//separate by newline
	out.list[0] = buf;
	out.size++;
	strtok(buf, "\n");
	for (int i = 1; ; i++) {
		out.list[i] = strtok(NULL, "\n");
		if (out.list[i] == NULL) {
			break;
		}
		out.size++;
	}

	return out;
}
void pstats(struct pstats_t in)
{
	printf("S: %d, F: %d, T:%d, R:%f\n", in.s, in.f, in.t, in.r);
}
int max(int a, int b)
{
	if (a > b) {
		return a;
	}
	return b;
}
int main(int argc, char **argv) 
{
	char DIRE[256] = "./";
	struct mp3s fls;
	struct pstats_t stats;

	stats.f = 0;
	stats.s = 0;
	stats.t = 0;
	stats.f = 0;

	if (argc > 2) {
		puts("Too many args. Usage: asst [<dir>]");
		exit(-1);
	}
	else if (argc == 2) {
		strcpy(DIRE, argv[1]);
	}

	fls = getmp3s(DIRE);

	for (int i = 0; i < fls.size; i++) {
		printf("%d: %s\n", i, fls.list[i]);
	}

	srand(clock());

	//loop steps:
	//1. Select track and start time
	//2. Fork into process which plays and which waits for 
	//user input
	//3. Print stats
	//4. Give option to continue, replay or

	int tno; //track number
	char stime[256]; //start time
	int tlen; //track length
	int cpid; //child pid
	FILE *fd; //for cmd pipe
	char cmd[512];
	char *cbuf = malloc(128); //command output buffer
	char *abuf = malloc(256);
	char c; //read loop char

	printf("Commands:\nh - print help\nr - replay\ns - skip\na - answer\n");
	while (1) {
iter:
		tno = rand() % fls.size;
		//file length
		sprintf(cmd, "mplayer -ao null -identify -frames 0 %s 2>&1 | grep LENGTH", fls.list[tno]);

		fd = popen(cmd, "r");
		fread(cbuf, 1, 1024, fd); 
		if (fd == NULL) {
			perror("loop fread");
			exit(-1);
		}
		fflush(stdout);
		tlen = atoi(strchr(cbuf, '=') + 1);
		//start time!
		sprintf(stime, "%d", max(floor((double) tlen * 
			((double) rand() / RAND_MAX)) - 30, 0)); //guarantees 30 secs

refork:
		cpid = fork();

		if (!cpid) { //child
			//printf("playing file %s at second %s\n", fls.list[tno], stime);
			close(STDIN_FILENO);
			close(STDERR_FILENO);
			sprintf(cmd, "cvlc %s --start-time %d", fls.list[tno], stime);
			execl("/usr/bin/cvlc", "cvlc", fls.list[tno], 
					"--start-time", stime, (char *) NULL);
			_exit(0);
		}
		while (1) { //read loop
			puts("input command:");
shit:
			scanf("%c", &c);
			switch (c) {
				case '\n':
					goto shit;
					break;
				case 'h':
					printf("Commands:\nh - print help\nr - replay\ns - skip\na - answer\n");
					break;
				case 'r':
					kill(cpid, SIGINT);
					goto refork;
					break;
				case 's':
					kill(cpid, SIGINT);
					pstats(stats);
					goto iter;
					break;
				case 'a':
					puts("input your answer:");
					scanf("%s", abuf);
					if (!strcasestr(fls.list[tno], abuf)) {
						puts("you FAILED!");
						printf("correct track: %s\n number:%d\n", fls.list[tno], tno);
						stats.f++;
						stats.t++;
						stats.r = (double) stats.s / stats.t;
						pstats(stats);

						puts("r - replay\nanything else - continue");
						kill(cpid, SIGINT);
						scanf("\n%c", &c);
						if (c == 'r') {
							goto refork;
						}
					}
					else {
						puts("SUCCESS!");
						printf("correct track: %s\n number: %d\n", fls.list[tno], tno);
						stats.s++;
						stats.t++;
						stats.r = (double) stats.s / stats.t;
						pstats(stats);
					}
					kill(cpid, SIGINT);
					goto iter;
					break;
			}
		}
	}

	exit(0);
	return 0;
}
