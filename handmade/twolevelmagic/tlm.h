#ifndef	_TLM_H
#define	_TLM_H

struct phdr {
    unsigned int	ph_magic;
    unsigned int    ph_len;
    unsigned int    ph_type;
};

#define PH_MAGIC    0xaabbccdd
#define PHT_UNO 1
#define PHT_DOS 2
#define PHT_TRES    3

struct ph_uno {
	unsigned int	a;
	char	u_str[];
};

struct ph_dos {
	unsigned int	a;
	unsigned int	b;
	unsigned int	c;
	char b_str[];
};

#endif /* !_TLM_H */
