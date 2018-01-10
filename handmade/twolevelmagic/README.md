
The idea behind this test case is you have two levels of
parsing; that is a primary header and then N different
sub-headers read next based off of a type field in the
primary.


```
struct phdr {
	unsigned char	ph_magic;
	unsigned int	ph_len;
	unsigned int	ph_type;
};

#define	PH_MAGIC	0x0E
#define	PHT_UNO	1
#define	PHT_DOS	2
#define	PHT_TRES	3

struct p_uno {
};

struct p_dos {
};

struct p_tres {
};
```


So there is a read of the header, read of type, etc and
and then read the next, depending on type.

