#pragma once
#include <cstdint>

/* defined in unix /usr/include/elf.h */

/* Type for a 16-bit quantity.  */
typedef uint16_t Elf32_Half;
typedef uint16_t Elf64_Half;

/* Types for signed and unsigned 32-bit quantities.  */
typedef uint32_t Elf32_Word;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf64_Word;
typedef int32_t  Elf64_Sword;

/* Types for signed and unsigned 64-bit quantities.  */
typedef uint64_t Elf32_Xword;
typedef int64_t  Elf32_Sxword;
typedef uint64_t Elf64_Xword;
typedef int64_t  Elf64_Sxword;

/* Type of addresses.  */
typedef uint32_t Elf32_Addr;
typedef uint64_t Elf64_Addr;

/* Type of file offsets.  */
typedef uint32_t Elf32_Off;
typedef uint64_t Elf64_Off;

/* Type for section indices, which are 16-bit quantities.  */
typedef uint16_t Elf32_Section;
typedef uint16_t Elf64_Section;

/* Type for version symbol information.  */
typedef Elf32_Half Elf32_Versym;
typedef Elf64_Half Elf64_Versym;

/* The ELF file header.  This appears at the start of every ELF file.  */

#define EI_NIDENT (16)

typedef struct
{
	unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
	Elf32_Half	e_type;         /* Object file type */
	Elf32_Half	e_machine;      /* Architecture */
	Elf32_Word	e_version;      /* Object file version */
	Elf32_Addr	e_entry;        /* Entry point virtual address */
	Elf32_Off	e_phoff;        /* Program header table file offset */
	Elf32_Off	e_shoff;        /* Section header table file offset */
	Elf32_Word	e_flags;        /* Processor-specific flags */
	Elf32_Half	e_ehsize;       /* ELF header size in bytes */
	Elf32_Half	e_phentsize;        /* Program header table entry size */
	Elf32_Half	e_phnum;        /* Program header table entry count */
	Elf32_Half	e_shentsize;        /* Section header table entry size */
	Elf32_Half	e_shnum;        /* Section header table entry count */
	Elf32_Half	e_shstrndx;     /* Section header string table index */
} Elf32_Ehdr;

typedef struct
{
	unsigned char e_ident[EI_NIDENT];	/* Magic number and other info */
										/* First Four Bytes Must Be 0x7f 'E' 'L' 'L', identify elf file*/
										/* 
											The Firth Byte identify elf file type
											0 invalid file
											1 elf32
											2 elf64
										*/
										/*
											The Sixth Byte Identify byte-order
											0 invalid format
											1 small-endian
											2 big-endian
										*/
	Elf64_Half	e_type;         /* Object file type */
								/*
									1 ET_REL 可重定位文件 relcated object file
									2 ET_EXEC 可执行文件 executable file
									3 ET_DYN 共享目标文件 shared lib
								*/
	Elf64_Half	e_machine;      /* Cpu Architecture */
								/*
									1 EM_M32	AT&T WE 32100
									2 EM_SPARC	SPARC
									3 EM_386	Intel x86
									4 EM_68k	Motorola 68000
									5 EM_88k	Motorola 88000
									6 EM_860	Intel 80860
								*/
	Elf64_Word	e_version;      /* Object file version, usually 0x1*/
	Elf64_Addr	e_entry;        /* Entry point virtual address*/
	Elf64_Off	e_phoff;        /* Program header table file offset */
	Elf64_Off	e_shoff;        /* Section header table file offset */
	Elf64_Word	e_flags;        /* Processor-specific flags */
	Elf64_Half	e_ehsize;       /* ELF header size in bytes */
	Elf64_Half	e_phentsize;    /* Program header table entry size */
	Elf64_Half	e_phnum;        /* Program header table entry count */
	Elf64_Half	e_shentsize;	/* Section header table entry size */
	Elf64_Half	e_shnum;        /* Section header table entry count */
	Elf64_Half	e_shstrndx;     /* Section header string table index */
} Elf64_Ehdr;

#define ELFMAG0     0x7f    /* Magic number byte 0 */
#define ELFMAG1     0x45    /* Magic number byte 1 */
#define ELFMAG2     0x4c    /* Magic number byte 2 */
#define ELFMAG3     0x46	/* Magic number byte 3 */

#define ELFFILECLS1	0x01	/*ELF32*/
#define ELFFILECLS2	0x02	/*ELF64*/

#define ELFBYTEORD1	0x01	/*Small-endian*/
#define ELFBYTEORD2	0x02	/*Big-endian*/