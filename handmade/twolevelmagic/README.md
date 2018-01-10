
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



## steps

```
 python ~/acsac17wip/naive/python/pskin_help.py -t dos.json
 cd work_dos/
 ./trace ../type_dos.tlm
 cat trace.log
 opt-4.0 -load ../../exitblocks/build/lib/ExitBlock.so -pskin-exit-blocks -pskin-depth 2 -pskin-log-file work_dos/trace.log -o work_dos/eb_d2.bc work_dos/prep.bc
 cd work_dos/
 AFL_CC=clang-4.0 afl-clang -o eb_d2 eb_d2.bc
```
