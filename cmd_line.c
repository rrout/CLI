#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "parser.h"

extern char *xmalloc PARAMS((size_t));
char *stripwhite ();

char **cli_completion (const char *text, int start, int end);
char *command_generator (const char *text, int state);

/* When non-zero, this global means the user is done using this program. */
int done;

void cmdLineInit(char *hostname)
{
	/* Allow conditional parsing of the ~/.inputrc file. */
	rl_readline_name = hostname;

    rl_bind_key('\t', rl_complete);

	/* Tell the completer that we want a crack first. */
	rl_attempted_completion_function = cli_completion;
}

char *
dupstr (char *s)
{
	char *r;

	r = xmalloc (strlen (s) + 1);
	strcpy (r, s);
	return (r);
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
char *
stripwhite (char *string)
{
	register char *s, *t;

	for (s = string; whitespace (*s); s++)
		;

	if (*s == 0)
		return (s);

	t = s + strlen (s) - 1;
	while (t > s && whitespace (*t))
		t--;
	*++t = '\0';

	return s;
}

/*
   Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any.
*/
char **
cli_completion (const char *text, int start, int end)
{
	char **matches;

	matches = (char **)NULL;
	rl_attempted_completion_over = 1;

	/* If this word is at the start of the line, then it is a command
	   to complete.  Otherwise it is the name of a file in the current
	   directory. */
	/*if (start == 0)*/
	matches = rl_completion_matches (text, command_generator);

	return (matches);
}

/*
   Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list.
*/
char *
command_generator (const char *text, int state)
{
	static int list_index, len;
	char *name;
	cdb_node_t *cmdNode = NULL;
    int num_index = 0;
	/* If this is a new word to complete, initialize now.  This includes
	   saving the length of TEXT for efficiency, and initializing the index
	   variable to 0. */
	if (!state)
	{
		list_index = 0;
		len = strlen (text);
	}
	cmdNode = getCdbNextNode(rl_line_buffer);
	if (cmdNode == NULL)
	{
		return ((char *)NULL);
	}
	else
	{
        //int num_index = 0;
        while(!(cmdNode[num_index++].flags & CMD_FLAG_LAST));
		if(list_index >= num_index)
			return ((char *)NULL);

	}
	/* Return the next name which partially matches from the command list. */
	while ((name = cmdNode[list_index].cmd))
	{
		list_index++;

		if (strncmp (name, text, len) == 0)
            return (dupstr(name));

		//if(cmdNode[list_index].flags & CMD_FLAG_LAST)
        if (list_index > num_index)
		return ((char *)NULL);
	}

	/* If no names matched, then return NULL. */
	return ((char *)NULL);
}

/* Execute a command line. */
int
cmdLineExecute(char *line)
{
    getCdbExecCli(line);
    return 0;
}

void cmdLineStart(char *cliHost, cdb_t *sptr_cdb)
{
	char *line, *s;
	char prompt[100] = {'\0'};

	cmdLineInit(cliHost);
	parserInit(cliHost, sptr_cdb);
	snprintf(prompt, sizeof(prompt), "%s$ ", cliHost);
	/*rl_bind_key('\t', rl_complete);*/
	/* Loop reading and executing lines until the user quits. */
	for ( ; done == 0; )
    {
		snprintf(prompt, sizeof(prompt), "%s(%s)%s%s ",
            sptr_cdb->cmd_hostname, sptr_cdb->curr_mode_str,
            sptr_cdb->curr_mode_usr_str, sptr_cdb->cmd_prompt);
		line = readline (prompt);

		if (!line)
			break;

		/* Remove leading and trailing whitespace from the line.
		   Then, if there is anything left, add it to the history list
		   and execute it. */
		s = stripwhite (line);
		/*s = line;*/
		if (*s)
		{
			add_history (s);
         	cmdLineExecute(s);
		}

		free (line);
	}
	return;
}

void cmdCLIStart(char *cliHost)
{
    memset(&g_sptr_cdb, 0, sizeof(g_sptr_cdb));
    cmdLineStart(cliHost, &g_sptr_cdb);
}

