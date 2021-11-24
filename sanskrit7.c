/*-
 * Copyright (c) 2021 Michael Roe
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <wctype.h>
#include <getopt.h>

int main(int argc, char **argv)
{
wchar_t buff[128];
wchar_t *cp;
int translit;
int output_notranslit = 0;
int output_uppercase = 0;
int output_tags = 0;
int opt;

  setlocale(LC_ALL, getenv("LANG"));
  
  while ((opt = getopt(argc, argv, "atu")) != -1)
  {
    switch (opt)
    {
      case 'a':
        output_notranslit = 1;
	break;
      case 't':
        output_tags = 1;
        break;
      case 'u':
	output_uppercase = 1;
	break;
      default:
        break;
    }
  }    

  translit = 0;
  while (fgetws(buff, sizeof(buff), stdin))
  {
    cp = buff;
    while (*cp)
    {
      if (translit)
      {
        if (*cp == ']')
        {
          if (output_tags)
	  {
	    wprintf(L"]");
	  }
          translit = 0;
	  cp++;
	}
	else if (wcsncmp(cp, L"aa", 2) == 0)
        {
          wprintf(L"\u0101");
	  cp += 2;
	}
	else if (wcsncmp(cp, L"ii", 2) == 0)
        {
          wprintf(L"\u012b");
	  cp += 2;
	}
	else if (wcsncmp(cp, L"uu", 2) == 0)
	{
          wprintf(L"\u016b");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".r.r", 4) == 0)
        {
          wprintf(L"\u1e5d");
	  cp += 4;
	}
	else if (wcsncmp(cp, L".r", 2) == 0)
	{
	  wprintf(L"\u1e5b");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".l.l", 4) == 0)
        {
          wprintf(L"\u1e39");
	  cp += 4;
	}
	else if (wcsncmp(cp, L".l", 2) == 0)
	{
	  wprintf(L"\u1e37");
	  cp += 2;
	}
	else if (wcsncmp(cp, L"\"n", 2) == 0)
        {
          wprintf(L"\u1e45");
	  cp += 2;
	}
	else if (wcsncmp(cp, L"\"s", 2) == 0)
	{
	  wprintf(L"\u015b");
	  cp += 2;
	}
	else if (wcsncmp(cp, L"~n", 2) == 0)
	{
	  wprintf(L"\u00f1");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".d", 2) == 0)
        {
	  wprintf(L"\u1e0d");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".h", 2) == 0)
        {
	  wprintf(L"\u1e25");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".m", 2) == 0)
        {
	  wprintf(L"\u1e43");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".n", 2) == 0)
	{
	  wprintf(L"\u1e47");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".s", 2) == 0)
	{
	  wprintf(L"\u1e63");
	  cp += 2;
	}
	else if (wcsncmp(cp, L".t", 2) == 0)
	{
	  wprintf(L"\u1e6d");
	  cp += 2;
        }
	else if (*cp == '&')
	{
          /* explicit virama */
	  cp++;
	}
	else if (*cp == '@')
	{
	  wprintf(L"\u0970");
	  cp++;
	}
	else
        {
          wprintf(L"%lc", *cp);
          cp++;
	}
      }
      else
      {
	if (wcsncmp(cp, L"[DV:", 4) == 0)
        {
          if (output_tags)
	  {
            wprintf(L"[DV:");
	  }
	  cp += 4;
	  translit = 1;
	}
	else if (wcsncmp(cp, L"[SA:", 4) == 0)
	{
          if (output_tags)
	  {
            wprintf(L"[SA:");
	  }
	  cp += 4;
	  translit = 1;
	}
	else if (wcsncmp(cp, L"[HI:", 4) == 0)
	{
	  if (output_tags)
          {
            wprintf(L"[HI:");
	  }
	  cp += 4;
	  translit = 1;
	}
	else
        {
          if (output_notranslit || (*cp == '\n'))
          {
	    wprintf(L"%lc", *cp);
	  }
	  else if (output_uppercase)
	  {
            wprintf(L"%lc", towupper(*cp));
	  }
          cp++;
	}
      }
    }
  }
  return 0;
}
