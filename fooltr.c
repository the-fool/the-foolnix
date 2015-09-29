#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_SET_SIZE 100
#define BUF_SIZE 1024

char *interp_set (const char *input_set);
int chpos (const char character, const char *char_set);

int
main (int argc, char *argv[])
{

  int opt, dflag, c, pos;

  opt = getopt (argc, argv, ":d");
  switch (opt)
    {
    case 'd':
      dflag = 1;
      break;
    case '?':
    default:
      dflag = 0;
    }

  // check for proper arguments 
  if (dflag && argc >= 4)
    {
      fprintf (stderr, "%s: extra operand after %s\n", argv[0], argv[2]);
      return EXIT_FAILURE;
    }
  else if (argc < 3)
    {
      if (dflag)
	{
	  fprintf (stderr, "%s: missing operand\n", argv[0]);
	}
      else if (argc == 2)
	{
	  fprintf (stderr, "%s: missing operand after %s\n", argv[0],
		   argv[1]);
	}
      else
	{
	  fprintf (stderr, "Usage: %s [-d] SET1 [SET2]", argv[0]);
	}
      return EXIT_FAILURE;
    }


  char *set;

      set = interp_set (argv[dflag + 1]);

  while ((c = getchar ()) != EOF)
    {
      if ( (pos = (chpos (c, set))) == -1)
	{
	  printf ("%c", c);
	}
      else if (!dflag)
	{
	  // this conditional is for the sake of a special functionality of tr
	  // if the post-trans string is shorter than the the pre-trans, then the final char
	  // in the post-trans string is used for all of the extra pre-trans chars
	  pos = (pos < strlen (argv[2]) ? pos : strlen (argv[2]) - 1);
	  printf ("%c", argv[2][pos]);
	}
    }

  return EXIT_SUCCESS;
}

int
chpos (const char character, const char *char_set)
{
  int i;
  for (i = 0; char_set[i] != '\0'; i++)
    {
      if (char_set[i] == character)
	return i;
    }
  return -1;
}

char *
interp_set (const char *input_set)
{
  static char trans_set[MAX_SET_SIZE];
  int i = 0;

  while ((trans_set[i] = *input_set) != '\0')
    {
      if (*input_set == '\\')
	{
	  input_set += 1;
	  switch (*input_set)
	    {
	    case 'n':
	      trans_set[i] = '\n';
	      break;
	    case 't':
	      trans_set[i] = '\t';
	      break;
	    case 'r':
	      trans_set[i] = '\n';
	      break;
	    case '\\':
	      trans_set[i] = '\\';
	      break;
	    default:
              break;
	    }
	}
      i++;
      input_set += 1;
    }
  return trans_set;
}
