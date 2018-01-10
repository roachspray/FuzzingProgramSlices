
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "tlm.h"

int verbose = 0;

int
phdr_len_ok(int fd)
{
	struct stat s;
	memset(&s, 0, sizeof(struct stat));
	if (fstat(fd, &s) == -1) {
		return 0;
	}
	if (s.st_size < sizeof(struct phdr)) {
		return 0;
	}
	return 1;
}

int
uno_len_ok(int fd, unsigned int lcomp)
{
	struct stat s;
	memset(&s, 0, sizeof(struct stat));
	if (fstat(fd, &s) == -1) {
		return 0;
	}
	if ((s.st_size - sizeof(struct phdr)) != lcomp) {
		return 0;
	}
	return 1;
}
int
dos_len_ok(int fd, unsigned int lcomp)
{
	struct stat s;
	memset(&s, 0, sizeof(struct stat));
	if (fstat(fd, &s) == -1) {
		return 0;
	}
	if ((s.st_size - sizeof(struct phdr)) != lcomp) {
		return 0;
	}
	return 1;
}
void
parse_uno(int fd, struct phdr *ph)
{
	struct ph_uno *uno;
	char	buf[512];	

	unsigned int l = ph->ph_len;
	if (uno_len_ok(fd, l) == 0) { // file len vs header told len
		return;
	}
	uno = (struct ph_uno *)malloc(l);
	if (uno == NULL) {
		return;
	}
	memset(uno, 0, sizeof(*uno));
	if (read(fd, uno, l) != l) {
		free(uno);
		return;
	}
	l -= sizeof(unsigned int);
	if (l >= 512) {
		free(uno);
		return;
	}
	memset(&buf, 0, 512);
	strcpy(buf, uno->u_str); 
	if (verbose) {
		printf("buf: %s\n", buf);
	}
	free(uno);
	return;
}

void
parse_dos(int fd, struct phdr *ph)
{
	struct ph_dos *dos;
	char	buf[512];	

	unsigned int l = ph->ph_len;
	if (dos_len_ok(fd, l) == 0) { // file len vs header told len
		return;
	}
	dos = (struct ph_dos *)malloc(l);
	if (dos == NULL) {
		return;
	}
	memset(dos, 0, sizeof(*dos));
	if (read(fd, dos, l) != l) {
		free(dos);
		return;
	}
	l -= (3 *sizeof(unsigned int));
	// Not a basic length check done on the string vs file size.
	// There are other issues and others we coulda lso introduce, but let's go with this for now.
	memset(&buf, 0, 512);
	strcpy(buf, dos->b_str); 
	if (verbose) {
		printf("buf: %s\n", buf);
	}
	free(dos);
	return;
}

void
parse_file(char *fpath)
{
	struct phdr	ph;
	int fd;

	fd = open(fpath, O_RDONLY);
	if (fd == -1) {
		perror("open");
		return;
	}
	
	if (phdr_len_ok(fd) == 0) {
		printf("Bad header length\n");
		(void)close(fd);
		return;
	}
	memset(&ph, 0, sizeof(ph));
	if (read(fd, &ph, sizeof(ph)) != sizeof(ph)) {
		printf("Unable to read phdr\n");
		(void)close(fd);
		return;
	}
	if (ph.ph_magic != PH_MAGIC) {
		printf("Invalid magic signature\n");
		(void)close(fd);
		return;
	}
	if (ph.ph_type == PHT_UNO) {
		parse_uno(fd, &ph);
	} else if (ph.ph_type == PHT_DOS) {
		parse_dos(fd, &ph);
	} 

	(void)close(fd);	
}


int
main(int argc, char **argv)
{
	unsigned idx = 1;	
	if (argc != 2 && argc != 3) {
		printf("usage: ...\n");
		exit(1);
	}
	if (argc == 3) {
		verbose = 1;
		idx++;
	}

	parse_file(argv[idx]);		
		
	return 0;
}
