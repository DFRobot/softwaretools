/*
    Copyright (C) <2013>  www.dfrobot.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//create by lisper <leyapin@gmail.com> from DFRobot
//2013-11-20

#include <stdio.h>
#include <stdlib.h>

//
long get_file_size (FILE *fp) {
	rewind (fp);
	fseek (fp, 0L, SEEK_END);
	long length = ftell (fp);
	rewind (fp);
	return length;
}

//
int main (int argc, char **argv) {
	if (argc != 4 ) {
		printf ("this program use for connect two hexfile\ncreated by lisper<leyapin@gmail.com> from DFRobot\nIt will delete the last row of infile1\nplease input:\ncathex infile1 infile2 outfile\n");
		exit (1);
	}

	FILE *infp1;
	FILE *infp2;
	FILE *outfp;

	infp1 = fopen (argv[1], "rb");
	if (infp1 == NULL) {
		printf ("Error! can't open file %s\n", argv[1]);
		exit (2);
	}

	infp2 = fopen (argv[2], "rb");
	if (infp2 == NULL) {
		printf ("Error! can't open file %s\n", argv[2]);
		exit (3);
	}

	outfp = fopen (argv[3], "rb");
	if (outfp) {
		char c = 0;
		printf ("Warning! file %s already exists!\n", argv[3]);
		printf ("overwrite %s ? y/n\n", argv[3]);
		for (;;) {
			fread (&c, 1, 1, stdin);
			if (c == 'n' || c == 'N') 
				exit (10);
			else if (c == 'y' || c == 'Y') {
				//printf ("\n");
				break;
			} else {
				printf ("please input 'y' or 'n'\n");
				continue ;
			}
		}
	}

	fclose (outfp);
	outfp = fopen (argv[3], "wb");

	long in_length1 = get_file_size (infp1);
	long in_length2 = get_file_size (infp2);

	unsigned char *buffer1 = malloc (in_length1);
	fread (buffer1, 1, in_length1, infp1);

	unsigned char *buffer2 = malloc (in_length2);
	fread (buffer2, 1, in_length2, infp2);

	int sub;
	for (sub = in_length1-3 ; sub > 0; sub--) {
		if (buffer1[sub] == 0x0a) {
			sub++;
			break;
		}
	}


	printf ("> %8ld %#9x  size of %s\n", in_length1, in_length1, argv[1]);
	printf ("> %8ld %#9x  size of %s\n", in_length2, in_length2, argv[2]);
	printf ("> %8ld %#9x  size of %s\n", in_length2+sub, in_length2+sub, argv[3]);

	fwrite (buffer1, 1, sub, outfp);
	fwrite (buffer2, 1, in_length2, outfp);

	printf ("delete %d byte of %s\n", in_length1-sub, argv[1]);
	printf ("Successful output file: %s", argv[3]);

	fclose (infp1);
	fclose (infp2);
	fclose (outfp);

	return 0;
}


