
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "tlm.h"

void
gen_uno(char *fpath, unsigned int uno_len)
{
	struct phdr p;
	struct ph_uno *uno;
	int fd;
	unsigned int ul = uno_len;

	memset(&p, 0, sizeof(p));
	p.ph_magic = PH_MAGIC;
	p.ph_len = uno_len;
	uno = (struct ph_uno *)malloc(uno_len);
	uno_len -= sizeof(unsigned int);
	uno_len -= 1;
	uno->a = 0xdeadbeef;
	while (uno_len > 0) {
		uno_len--;	
	// will be over
		uno->u_str[uno_len] = ((uno_len % 0x1A) + 0x41);
	}
	p.ph_type = PHT_UNO;
	fd = open(fpath, O_WRONLY|O_CREAT);
	write(fd, &p, sizeof(struct phdr));
	write(fd, uno, ul);
	close(fd);
}

void
gen_dos(char *fpath, unsigned int dos_len)
{
	struct phdr p;
	struct ph_dos *dos;
	int fd;
	unsigned int ul = dos_len;

	memset(&p, 0, sizeof(p));
	p.ph_magic = PH_MAGIC;
	p.ph_len = dos_len;
	dos = (struct ph_dos *)malloc(dos_len);
	dos_len -= (3 * sizeof(unsigned int));
	dos_len -= 1;
	dos->a = 0xdeadbeef;
	while (dos_len > 0) {
		dos_len--;	
	// will be over
		dos->b_str[dos_len] = ((dos_len % 0x1A) + 0x41);
	}
	p.ph_type = PHT_DOS;
	fd = open(fpath, O_WRONLY|O_CREAT);
	write(fd, &p, sizeof(struct phdr));
	write(fd, dos, ul);
	close(fd);
}
int
main(int argc, char **argv)
{
	gen_uno("type_uno.tlm", 256);
	gen_dos("type_dos.tlm", 256);
	return 0;
}
