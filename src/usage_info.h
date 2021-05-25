void usage_info(void)
{
	printf("------------------------------------------------------------------------\n");
	printf("tlg2u: TLG/PHI beta code to Unicode (utf-8) converter.                  \n");
	printf("tlg2u:           Version:  ");
	printf( VERSION); printf("\n");
	printf("tlg2u: Convert beta encoded text to utf-8 and print to standard output.\n");
	printf("tlg2u: Copyright (C) 2016 proteuss@sdf.lonestar.org\n\n");
	printf("------------------------------------------------------------------------\n");
	printf("tlg2u: Kudos to Dimitri Marinakis (http://tlgu.carmen.gr/)\n");
	printf("tlg2u:           for the utility tlgu\n");
	printf("------------------------------------------------------------------------\n\n");
	printf("tlg2u: Syntax: tlg2u [OPTION]... [BETA_CODE_FILE] \n\n");
	printf("tlg2u: -a -- Author id-code (nnnn)\n");
	printf("tlg2u: -b -- Book id-code (nnn)\n");
	printf("tlg2u: -B -- Number of first 8k block of Book. \n");
	printf("tlg2u: -c -- Corpus (tlg, lat, civ, ins, ddp or chr)\n");
	printf("tlg2u: -d -- Corpus directory (e.g. /cdroms/tlg/ )\n");
	printf("tlg2u: -C -- Print all citations in left margin. (nnn.nnn.nnn...)\n");
	printf("tlg2u: -P -- Print paragraphs in left margin.\n");
	printf("tlg2u: -l -- Print line numbers in right margin.\n");
	printf("tlg2u: -m -- Print marginalia in right margin.\n");
	printf("tlg2u: -T -- Main text width.\n");
	printf("tlg2u: -M -- Marginal text width.\n");
	printf("tlg2u: -L -- Latin Text.\n");
	printf("tlg2u: -G -- Greek Text.  (default)\n");
	printf("tlg2u: -u -- utf-8 output (default)\n");
	printf("tlg2u: -p -- pdf output. (Include LaTeX markups to be used by XeLaTeX -> pdf)\n");
	printf("tlg2u: -n -- Print only maintext_width, marginalia_width, lines_count. \n");
  printf("tlg2u:               (used for building canons).\n\n");
	printf("tlg2u: -h -- Print usage information\n");
	printf("tlg2u: Note: If BETA_CODE_FILE is given then -a, -b, -B and -d\n");
	printf("tlg2u:       are ignored and the whole file is converted.\n\n");
}

 
