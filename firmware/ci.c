#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <generated/csr.h>
#include <generated/mem.h>
#include <time.h>
#include <console.h>
#include <hw/flags.h>

#include "ci.h"
#include "bist.h"

static void ci_help(void)
{
	puts("help        - this command");
	puts("reboot      - reboot CPU");
#ifdef CSR_GENERATOR_BASE
	puts("sdram_test  - sdram test");
#endif
}

static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token_generic(char **str, char delimiter)
{
	char *c, *d;

	c = (char *)strchr(*str, delimiter);
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static char *get_token(char **str)
{
	return get_token_generic(str, ' ');
}

static void reboot(void)
{
	asm("call r0");
}

void ci_prompt(void)
{
	printf("RUNTIME>");
}

void ci_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;

	token = get_token(&str);

    if(strcmp(token, "help") == 0) {
		puts("Available commands:");
		token = get_token(&str);
		ci_help();
		puts("");
	}
	else if(strcmp(token, "reboot") == 0) reboot();
#ifdef CSR_GENERATOR_BASE
	else if(strcmp(token, "sdram_test") == 0) bist_test();
#endif
	ci_prompt();
}
