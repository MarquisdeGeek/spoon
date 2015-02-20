#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


char g_token0 = '0';
char g_token1 = '1';

void bannner(void);
void help(void);
void parse_args(int argc,char *argv[]);
void import_brain(FILE *fp);
void import_spoon_as_brain(FILE *f);
int generate_c_code(char *fn, char *braincode);


int  p, r, q;
char a[5000], f[25000], b, o, *s=f;


void interpret(char *c)
{
	char *d;

	r++;
	while( *c ) {
		switch(o=1,*c++) {
		
		case '\1': exit(0);        break;

		case '<': p--;        break;
		case '>': p++;        break;
		case '+': a[p]++;     break;
		case '-': a[p]--;     break;
		case '.': putchar(a[p]); fflush(stdout); break;
		case ',': a[p]=getchar();fflush(stdout); break;
		case '[':
			for( b=1,d=c; b && *c; c++ )
				b+=*c=='[', b-=*c==']';
			if(!b) {
				c[-1]=0;
				while( a[p] )
					interpret(d);
				c[-1]=']';
				break;
			}
		case ']':
			puts("UNBALANCED BRACKETS"), exit(0);
		case '#':
				printf("%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n%*s\n",
				       *a,a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],3*p+2,"^");
			break;
		default: o=0;
		}
		if( p<0 || p>100)
			puts("RANGE ERROR"), exit(0);
	}
	r--;
}

main(int argc,char *argv[])
{
	parse_args(argc, argv);
	return 0;
}


void bannner(void)
{
	printf("Spoon Interpreter - v1.00\nWritten by S.Goodwin 1998\n");
	printf("Code and Language based on BrainF***, by Urban Mueller\n\n");
	printf("Usage: spoon [/b /c /d /h /q /0 /1] <source filename>\n\n");
}

void help(void)
{
	printf("/b   Process Brain source, instead of Spoon\n");
	printf("/cx  Convert program into C source file, called 'x'\n");
	printf("/d   Don't interpret source file\n");
	printf("/h   Help message\n");
	printf("/?   Help message\n");
	printf("/q   Hide banner\n");
	printf("/0x  Specify the '0' token as 'x'\n");
	printf("/1x  Specify the '1' token as 'x'\n");
}

void parse_args(int argc,char *argv[])
{
int quiet=0;
int l=1;
int generate_c=0;
int process_as_brain = 0;
int process_file = 1;
char *c_file;
char *fn;
FILE *fp;


	while(l < argc)
		{
		if (argv[l][0] == '-' || argv[l][0] == '/')
			{
			switch(argv[l][1])
				{
				case	'c':
				case	'C':
							generate_c = 1;
							c_file = &argv[l][2];
							break;

				case	'd':
				case	'D':
							process_file = 0;
							break;

				case	'h':
				case	'H':
				case	'?':
							bannner();
							help();
							quiet = 1;
							break;

				case	'q':
				case	'Q':
							quiet = 1;
							break;

				case	'b':
				case	'B':
							process_as_brain = 1;
							break;

				case	'0':
							g_token0 = argv[l][2];
							break;

				case	'1':
							g_token1 = argv[l][2];
							break;

				}
			}
		else
			fn = argv[l];
		l++;
		}
	/**/
	if (!quiet)
		bannner();
	/**/
	if(fp=fopen(fn,"r"))
		{
		if (process_as_brain)
			import_brain(fp);
		else
			import_spoon_as_brain(fp);
		fclose(fp);

		/* Add dummy character to end of file (for generating C code)*/
		*s++ = ' ';
		*s = 0;

		if (generate_c)
			generate_c_code(c_file, f);
		/**/
		if (process_file)
			interpret(f);
		}

}


void import_brain(FILE *fp)
{
	while( (b=getc(fp))>0 )
		*s++=b;
	*s=0;
}

void import_spoon_as_brain(FILE *f)
{
int curr_val = 0;
int bits_count = 0;
char ch;

	while((b = getc(f))>0)
		{
		if (b == g_token0 || b == g_token1)
			{
			curr_val *= 2;
			curr_val += (b==g_token0)?0:1;
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
								ch = '\1';	  				/* Special code, because Brain doesn't support exit */
							break;
				}
			/**/
			if (ch)
				{
				*s++ = ch;
				bits_count = curr_val = 0;
				}
			}
		}
	*s = '\0';
}

int generate_c_code(char *fn, char *braincode)
{
char line[256];
FILE *fp;
char *p = braincode;
char last_code;
int num_of_last_code;
int tabs_indented;
int process;

	if(fp=fopen(fn,"w+"))
		{
		/* Write standard program header information */
		fputs("/*\n", fp);
		fputs("** Spoon -> Converter. Copyright 1998 VisionSoft. Written by S.Goodwin\n", fp);
		fputs("**\n", fp);
		fputs("**\n", fp);
		fputs("** This program has been converted, fairly literately from\n", fp);
		fputs("** from the original Spoon source. \n", fp);
		fputs("**\n", fp);
		fputs("** Version 1.00 Conversion\n", fp);
		fputs("** + Multiple '+', '-', '<' and '>' symbols have been joined\n", fp);
		fputs("**\n", fp);
		fputs("*/\n", fp);
		fputs("\n", fp);
		fputs("#include <stdio.h>\n", fp);
		fputs("\n", fp);
		fputs("int p;\n", fp);
		fputs("char a[5000];\n", fp);
		fputs("\n", fp);
		fputs("int main(int argc, char *argv[])\n", fp);
		fputs("{\n", fp);

		p = braincode;
		last_code = *p++;
 
		num_of_last_code = 1;
		tabs_indented = 1;

		do
			{
			process = 1;

			switch(*p)
				{
				case	'+':		 /* these symbols will use the 'num_of_last_code' */
				case	'-':
				case	'>':
				case	'<':
								if (last_code == *p)
									{
									num_of_last_code++;
									process = 0;
									break;
									}
				}

			if (process)
				switch(last_code)
					{
					case	'+':
					case	'-':
					case	'<':
					case	'>':
				
					case	'[':
					case	']':
					case	'.':
					case	',':
					case	'#':
					case	'\1':
									/* Create line */
									if (last_code == ']')
										if (tabs_indented-- <= 0)
											tabs_indented = 0;

									memset(line, '\t', tabs_indented);
									switch(last_code)
										{
										case	'+':
														if (num_of_last_code == 1)
															strcpy(&line[tabs_indented], "a[p]++;");
														else
															sprintf(&line[tabs_indented], "a[p] += %d;", num_of_last_code);
														break;
										case	'-':
														if (num_of_last_code == 1)
															strcpy(&line[tabs_indented], "a[p]--;");
														else
															sprintf(&line[tabs_indented], "a[p] -= %d;", num_of_last_code);
														break;
										case	'>':
														if (num_of_last_code == 1)
															strcpy(&line[tabs_indented], "p++;");
														else
															sprintf(&line[tabs_indented], "p += %d;", num_of_last_code);
														break;
										case	'<':
														if (num_of_last_code == 1)
															strcpy(&line[tabs_indented], "p--;");
														else
															sprintf(&line[tabs_indented], "p -= %d;", num_of_last_code);
														break;
										case	'.':
														strcpy(&line[tabs_indented], "putchar(a[p]);  fflush(stdout);");
														break;
										case	',':
														strcpy(&line[tabs_indented], "a[p] = getchar();  fflush(stdout);");
														break;
										case	'[':
														strcpy(&line[tabs_indented], "while(a[p]) {");
														tabs_indented++;
														break;
										case	']':
														strcpy(&line[tabs_indented], "}");
														break;
									
										case	'\1':
														*(p+1) = 0;	/* force termination, so exit is not needed */
														break;

										case	'#':
														strcpy(&line[tabs_indented],
																"printf(\"%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d%\\n%*s%\\n\", *a,a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],a[9],3*p+2,\"^\");");
														break;
										}

									strcat(line, "\n");
									fputs(line, fp);
									num_of_last_code = 1;
									break;
					}
			/**/
			last_code = *p;
			p++;
			}
		while(*p);

		fputs("return 0;\n\n", fp);
		fputs("}\n\n", fp);
		/**/
		fclose(fp);
		/**/
		return 1;
		}

	return 0;
}
