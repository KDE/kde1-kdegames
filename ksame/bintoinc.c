#include <stdio.h>
#include <stdlib.h>

int main (int argc,char **argv) {
  FILE *in;
  FILE *out;

#define BUFSIZE 1000
unsigned char c[BUFSIZE];
  int i,n;
  long t=1;

  //char dd[]={12,12,12,12,};

  if (argc!=4) {
    printf("usage: %s <infile binary> <outfile c-include> <name>\n",argv[0]);
    return(0);
  }

  in=fopen(argv[1],"r");
  out=fopen(argv[2],"w+");
  
 fprintf(out,"char %s[]={\n",argv[3]);
  while (!feof(in)) {
   
    n=fread(c,sizeof(char),BUFSIZE,in);
    for (i=0;i<n;i++) {
      fprintf(out,"%3i,",c[i]);
      if (!((t++)%25)) fprintf(out,"\n"); 
    }
   
  }
  fprintf(out,"};\nint %s_len=%li;\n",argv[3],t-1);
  fclose(in);
  fclose(out);
  
  return(0);
  
}

/*
 *   ksame 0.2 - simple Game
 *   Copyright (C) 1997  Marcus Kreutzberger
 * 
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *    
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
