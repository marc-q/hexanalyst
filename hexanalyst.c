/* Copyright 2015 Marc Volker Dickmann */
/* Project: Hexanalyst */
#include <stdio.h>
#include <string.h>

#define P_CMD "hexanalyst"
#define P_VERSION "0.0.1"

/*
	Function: ha_read_file_hex (char* filename, int seekpos, int start, int end);
	Description: Reads a file and display its bytes and the bits of the bytes.
	InitVersion: 0.0.1
*/
static int ha_read_file_hex (char* filename, int seekpos, int start, int end)
{
	int i, j, bit;
	char byte;
	FILE *afile;

	i = j = bit = 0;

	afile = fopen (filename, "rb");

	if (afile == NULL)
	{
		return -1;
	}

	fseek (afile, seekpos, SEEK_SET);

	for (i = start; i < end; i++)
	{
		fread (&byte, sizeof (char), 1, afile);

		printf ("%x", byte);

		for (j = 0; j < 8; j++)
		{
			bit = (byte >> j) & 1;
			printf ("\t%i - %i\n", j, bit);
		}
		printf ("\n");
	}

	fclose (afile);
	
	return 0;
}

static void usage (void)
{
	printf ("Use: $ %s <filename> <seekfromset> <counterstart> <counterend>\n", P_CMD);
}

int main (int argc, char* argv[])
{
	printf ("Hexanalyst v. %s (c) 2015 Marc Volker Dickmann\n\n", P_VERSION);

	if (argc == 5)
	{
		if (isdigit (argv[2][0]) == 0 ||
		    isdigit (argv[3][0]) == 0 ||
		    isdigit (argv[4][0]) == 0)
		{
			usage ();
		}
		else
		{
			ha_read_file_hex (argv[1], atoi (argv[2]), atoi (argv[3]), atoi (argv[4]));
		}
	}
	else
	{
		usage ();
	}

	return 0;
}

