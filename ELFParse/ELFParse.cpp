#include "elf_struct.h"

bool ShowELFHeader64(Elf64_Ehdr* pElfHeader)
{
	if (pElfHeader->e_ident[0] != ELFMAG0 ||
		pElfHeader->e_ident[1] != ELFMAG1 ||
		pElfHeader->e_ident[2] != ELFMAG2 ||
		pElfHeader->e_ident[3] != ELFMAG3)
	{
		return false;
	}

	if (pElfHeader->e_ident[4] != ELFFILECLS1 &&
		pElfHeader->e_ident[4] != ELFFILECLS2) 
	{
		return false;
	}

	if (pElfHeader->e_ident[5] != ELFBYTEORD1 &&
		pElfHeader->e_ident[5] != ELFBYTEORD2)
	{
		return false;
	}



	return true;
}