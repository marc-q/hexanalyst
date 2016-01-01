/* Copyright 2015 - 2016 Marc Volker Dickmann */
/* Project: Hexanalyst */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "lib/dutils.h"

#define P_CMD "hexanalyst"
#define P_VERSION "0.0.1"

#define PRINTIF(a, b) ((a) >= 21 && (a) <= (126) ? (a) : (b))

/*
	Function: utils_streq (char* one, char* two);
	Description: Check's if two strings are equal and have the same length.
	InitVersion: 0.0.1
*/
int utils_streq (char* one, char* two)
{
	if (strlen (one) != strlen (two))
	{
		return -1;
	}
	
	return strncmp (one, two, strlen (one));
}

/*
	Function: ha_view_bits (FILE* bfile, int seekpos, int end);
	Description: Prints the bits and bytes.
	InitVersion: 0.0.1
*/
static void ha_view_bits (FILE* bfile, int seekpos, int end)
{
	int i, j, bit;
	char byte;
	
	i = j = bit = 0;
	
	byte = '\0';
	
	fseek (bfile, seekpos, SEEK_SET);
	
	for (i = 0; i < end && !feof (bfile); i++)
	{
		fread (&byte, sizeof (char), 1, bfile);

		printf ("| Offset: %i - %x\n", seekpos+i, seekpos+i);
		printf ("| %c - %i - %x\t", PRINTIF (byte, ' '), byte, byte);

		for (j = 0; j < 8; j++)
		{
			bit = (byte >> j) & 1;
			
			if (j == 1)
			{
				printf ("----------------");
			}
			else if (j != 0)
			{
				printf ("\t\t");
			}
			
			printf ("|%i - %i\n", j, bit);
		}
		printf ("\n");
	}
}

/*
	Function: ha_view_ascii (FILE* bfile, int seekpos, int end);
	Description: Prints the readable ascii.
	InitVersion: 0.0.1
*/
static void ha_view_ascii (FILE* bfile, int seekpos, int end)
{
	int i, j;
	char byte;
	
	i = j = 0;
	
	byte = '\0';
	
	fseek (bfile, seekpos, SEEK_SET);
	
	for (i = 0; i < end && !feof (bfile); i++, j++)
	{
		fread (&byte, sizeof (char), 1, bfile);

		printf ("%c", PRINTIF (byte, ' '));
		
		if (j >= 80)
		{
			printf ("\n");
			j = 0;
		}
	}
	
	printf ("\n");
}

/*
	Function: ha_read_file_binary (char* filename, int view, int seekpos, int end);
	Description: Reads a file and display its bytes and the bits of the bytes.
	InitVersion: 0.0.1
*/
static int ha_read_file_binary (char* filename, int view, int seekpos, int end)
{
	FILE *bfile;

	bfile = fopen (filename, "rb");

	if (bfile == NULL)
	{
		return -1;
	}

	switch (view)
	{
		case 0:
			ha_view_bits_two (bfile, seekpos, end);
			break;
		case 1:
			ha_view_ascii (bfile, seekpos, end);
			break;
		default:
			break;
	}
	
	fclose (bfile);
	
	return 0;
}

/*
	Function: print_about (void);
	Description: Prints the about message.
	InitVersion: 0.0.1
*/
static void print_about (void)
{
	printf ("LICENCE: GPL v.3\n");
}

/*
	Function: print_help (void);
	Description: Prints the help message.
	InitVersion: 0.0.1
*/
static void print_help (void)
{
	printf ("%s\n", P_CMD);
	printf ("\tabout\t\t\t\tAbout this programm.\n");
	printf ("\thelp\t\t\t\tThis help message.\n");
	printf ("\tdo [FILENAME] [bytes] ARGS\tShow [bytes] from [FILENAME] with optional ARGS.\n");
	
	printf ("\nARGS:\n");
	printf ("-a\t\tUses the ascii view instad of the bit view.\n");
	printf ("-h\t\tThis help.\n");
	printf ("-s [seekset]\tSeek [seekset] bytes from the beginning.\n");
	
	printf ("\nLEGEND: <optional> [necessary] [integer] [STRING]\n");
}


/*
	Function: usage (void);
	Description: Prints the usage message.
	InitVersion: 0.0.1
*/
static void usage (void)
{
	printf ("Use: $ %s -h\n", P_CMD);
}

int main (int argc, char* argv[])
{
	int optc, useascii, seekpos;
	
	useascii = seekpos = 0;
	
	printf ("Hexanalyst v. %s (c) 2015 Marc Volker Dickmann\n\n", P_VERSION);

	if (argc == 2)
	{
		if (utils_streq (argv[1], "help") == 0)
		{
			print_help ();
			exit (EXIT_SUCCESS);
		}
		else if (utils_streq (argv[1], "about") == 0)
		{
			print_about ();
			exit (EXIT_SUCCESS);
		}
	}

	while ((optc = getopt (argc, argv, "has:")) != -1)
	{
		switch (optc)
		{
			case 'h':
				print_help ();
				exit (EXIT_SUCCESS);
				break;
			case 'a':
				useascii = 1;
				break;
			case 's':
				if (isdigit (optarg[0]) != 0)
				{
					seekpos = atoi (optarg);
				}
				break;
			default:
				break;
		}
	}

	if (argc >= 4)
	{
		if (utils_streq (argv[optind], "do") == 0)
		{
			if (isdigit (argv[optind+2][0]) == 0)
			{
				usage ();
			}
			else
			{
				ha_read_file_binary (argv[optind+1], useascii, seekpos, atoi (argv[optind+2]));
			}
		}
		else
		{
			usage ();
		}
	}
	else
	{
		usage ();
	}

	return 0;
}
