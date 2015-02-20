/*
**
**  Spoon <-> Brain Converter - Copyright 1998 by VisionSoft - Written by S.Goodwin
**
**                           All Rights Reserved
**
**    Permission is granted to copy and distribute this source in any form, so
**    long as no money is charged for it, and any changes are documented.
**
*/

/*

Spoon     Brain    Operation                                 C
       1    +      Increment the number at the ptr           a[p]++
     000    -      Increment the number at the ptr           a[p]--
     010    >      Move the address pointer on 1 space       p++
     011    <      Move the address pointer back 1 space     p--
    0011    ]      End loop - terminates when address!=0     }
   00100    [      Start loop (pre-check)					 while(a[p]){
  001010    .      Prints character at address space         putchar(a[p]);
 0010110    ,      Get char from keyboard, and write to mem  a[p]=getchar()
00101110    #      Outputs stack information
00101111   <eof>   Terminates the program					 exit(0);
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


typedef int BOOL;


#define TRUE	1
#define FALSE	0


int g_token0 = '0';
int g_token1 = '1';

void help(void);
void bannner(void);

int main(int argc, char *argv[]);
void parse_args(int argc, char *argv[]);

BOOL b2s(char *dest, char *src, int line_width, BOOL pad, BOOL ignore_comments);
BOOL s2b(char *dest, char *src, BOOL pad, BOOL ignore_comments);

void bannner(void)
{
	printf("Spoon <-> Brain Converter\nWritten by S.Goodwin 1998\n\n");
	printf("Usage: spoon [/h /s /b /p /i /0 /1 /l] <source filename> [destination filename]\n\n");
}

void help(void)
{
	printf("/h    Help message\n");
	printf("/s    Convert to 'Spoon' (default)\n");
	printf("/b    Convert to 'Brain'\n");
	printf("/p    Pad destination\n");
	printf("/i    Ignore commented code\n");
	printf("/lx   Enfore constant line width of 'x' characters (Spoon files only)\n");
	printf("/0x   Specify the '0' token as 'x'\n");
	printf("/1x   Specify the '1' token as 'x'\n");
	printf("\n");
	printf("This program performs conversion between the two Esoteric languages of\n");
	printf("BrainF*** & Spoon.\n");
	printf("Both languages have identical syntax, only the tokens differs. Spoon uses\n");
	printf("Huffman encoded tokens in order to reduce the total number of different\n");
	printf("symbols to two: '0' and '1'.\n\n");
	printf("The mapping is:\n");
	printf("\t+ 1\n");
	printf("\t- 000\n");
	printf("\t> 010\n");
	printf("\t< 011\n");
	printf("\t] 0011\n");
	printf("\t[ 00100\n");
	printf("\t. 001010\n");
	printf("\t, 0010110\n");
	printf("\t# 00101110\n");
	printf("\tX 00101111 (exit program - not available in Brain)\n\n");
}

int main(int argc, char *argv[])
{
	parse_args(argc, argv);
	return 0;
}

void parse_args(int argc, char *argv[])
{
char *src_ptr=NULL, *dest_ptr=NULL;
char dest_file[200], *cp;
BOOL convert_to_spoon = TRUE;
BOOL pad_source = FALSE;
BOOL ignore_comments = FALSE;
BOOL success;
int line_width = 0;
int l = 1;

	bannner();

	while(l < argc)
		{
		if (argv[l][0] == '-' || argv[l][0] == '/')
			{
			switch(argv[l][1])
				{
				case	'h':
				case	'H':
				case	'?':
							help();
							break;

				case	's':
				case	'S':
							convert_to_spoon = TRUE;
							break;

				case	'b':
				case	'B':
							convert_to_spoon = FALSE;
							break;

				case	'p':
				case	'P':
							pad_source = TRUE;
							break;

				case	'i':
				case	'I':
							ignore_comments = TRUE;
							break;

				case	'0':
							g_token0 = argv[l][2];
							break;

				case	'1':
							g_token1 = argv[l][2];
							break;

				case	'l':
				case	'L':
							line_width = atoi(&argv[l][2]);
							if (line_width < 0 || line_width >= BUFSIZ)
								line_width = 80;
							break;

				default:
							printf("Unknown argument: %s\n", argv[l]);
				}
			}
		else
			{
			if (src_ptr == NULL)
				src_ptr = argv[l];
			else if (dest_ptr == NULL)
				dest_ptr = argv[l];
			else
				printf("Too many file names (%s ignored)!\n", argv[l]);
			}

		l++;
		}
	/**/
	if (src_ptr)
		{
		if (!dest_ptr)
			{
			strcpy(dest_file, src_ptr);
			if (cp = strrchr(dest_file, '.'))
				*cp = '\0';
		
			if (convert_to_spoon)
				strcat(dest_file, ".sp");
			else
				strcat(dest_file, ".b");
			/**/
			dest_ptr = &dest_file[0];
			}
		/**/
		printf("Converting '%s' to '%s'...", src_ptr, dest_ptr);
		/**/
		if (convert_to_spoon)
			success = b2s(dest_ptr, src_ptr, line_width, pad_source, ignore_comments);
		else
			success = s2b(dest_ptr, src_ptr, pad_source, ignore_comments);
		/**/
		if (success)
			printf("Success!\n");
		else
			printf("Failed!\n");
		}
	else
		printf("No source file specified!\n");

}

BOOL b2s(char *dest, char *src, int line_width, BOOL pad, BOOL ignore_comments)
{
BOOL success = FALSE;
FILE *fp_src, *fp_dest;
char str[BUFSIZ];
char target[BUFSIZ*6], *tp;
int l, mx, sz;

	if (fp_src = fopen(src, "r"))
		{
		if (fp_dest = fopen(dest, "w"))
			{
			tp = &target[0];
			while(fgets(str, sizeof(str), fp_src))
				{
				mx = strlen(str);
				for(l=0;l<mx;l++)
					{
					switch(str[l])
						{
						case	'+':
									*tp++ = g_token1;
									break;

						case	'-':
									*tp++ = g_token0;
									*tp++ = g_token0;
									*tp++ = g_token0;
									break;

						case	'>':
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token0;
									break;

						case	'<':
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token1;
									break;

						case	']':
									*tp++ = g_token0;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token1;
									break;

						case	'[':
									*tp++ = g_token0;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token0;
									*tp++ = g_token0;
									break;

						case	'.':
									*tp++ = g_token0;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token0;
									break;

						case	',':
									*tp++ = g_token0;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token1;
									*tp++ = g_token0;
									break;

						case	'#':
									*tp++ = g_token0;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token0;
									*tp++ = g_token1;
									*tp++ = g_token1;
									*tp++ = g_token1;
									*tp++ = g_token0;
									break;
									
						default:
									if (ignore_comments == FALSE)
										*tp++ = str[l];
						}
					/**/
					switch(str[l])
						{
						case	'+':
						case	'-':
						case	'[':
						case	']':
						case	'<':
						case	'>':
						case	'.':
						case	',':
						case	'#':
									if (pad)
										*tp++ = ' ';
									break;
						}
					/**/
					}
				/**/
				*tp++ = '\0';
				/**/
				if (line_width == 0)
					{
					fputs(target, fp_dest);
					tp = &target[0];
					}
				else
					{
					while(strlen(target) >= line_width)
						{
						sz = min(line_width, strlen(target));
						sz = fwrite(target, 1, sz, fp_dest);
						memmove(&target[0], &target[sz], (strlen(target)-sz)+1);
						fputs("\n", fp_dest);			/* forces out a newline character */
						}
					tp = strchr(target, '\0');
					}
				}
			/**/
			if (line_width)
				{
				fputs(target, fp_dest);
				fputs("\n", fp_dest);
				}
			/**/
			fclose(fp_src);
			fclose(fp_dest);
			/**/
			success = TRUE;
			}
		else
			fclose(fp_src);
		}

	return success;
}

BOOL s2b(char *dest, char *src, BOOL pad, BOOL ignore_comments)
{
BOOL success = FALSE;
FILE *fp_src, *fp_dest;
char str[200];
char target[200], *tp;
char ch;
int l, mx;
int curr_val, bits_count;

	if (fp_src = fopen(src, "r"))
		{
		if (fp_dest = fopen(dest, "w"))
			{
			curr_val = bits_count = 0;
			/**/
			while(fgets(str, sizeof(str), fp_src))
				{
				mx = strlen(str);
				tp = &target[0];
				for(l=0;l<mx;l++)
					{
					if (str[l] == g_token0 || str[l] == g_token1)
						{
						curr_val *= 2;
						curr_val += (str[l]==g_token0)?0:1;
						/**/
						bits_count++;
						/**/
						ch = '\0';
						/**/
						switch(bits_count)
							{
							case	1:
										if (curr_val == 1)
											ch = '+';
										break;

							case	3:
										if (curr_val == 0)
											ch = '-';
										else if (curr_val == 2)
											ch = '>';
										else if (curr_val == 3)
											ch = '<';
										break;

							case	4:
										if (curr_val == 3)
											ch = ']';
										break;

							case	5:
										if (curr_val == 4)
											ch = '[';
										break;

							case	6:
										if (curr_val == 10)
											ch = '.';
										break;

							case	7:
										if (curr_val == 22)
											ch = ',';
										break;

							case	8:
										if (curr_val == 46)
											ch = '#';
										else if (curr_val == 47)
											{
											strcpy(tp, "(exit program not supported in Brain language)");
											tp = strchr(tp, '\0');
											}
										break;
							}
						/**/
						if (ch)
							{
							*tp++ = ch;
							bits_count = curr_val = 0;
							/**/
							if (pad && (ch == '.' || ch == ','))
								*tp++ = ' ';
							}
						}
					else if (ignore_comments == FALSE)
						*tp++ = str[l];

					}
				/**/
				*tp++ = '\0';
				fputs(target, fp_dest);
				/**/
				}
			/**/
			fclose(fp_src);
			fclose(fp_dest);
			/**/
			success = TRUE;
			}
		else
			fclose(fp_src);
		}

	return success;
}
