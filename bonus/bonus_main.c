/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/02 18:03:37 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/bonus.h"

void	clean_fds(t_bonus *bonus)
{
	int	i;
	
	i = 0;
	while (bonus->pipe_fd[i])
		free (bonus->pipe_fd[i++]);
}

int	main(int ac, char **av, char **envp)
{
	t_bonus	bonus;
	int		i;
	int		wstatus;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, bonus.here_doc);
	create_pipes(&bonus);
	fork_children(&bonus, av, envp, ac);
	//execute_parent();
	i = 0;
	while (i < bonus.cmd_count)
	{
		if (waitpid(bonus.pids[i], &wstatus, 0) == -1)
			error_handling(EC_WAIT);
		i++;
	}
	clean_fds(&bonus);
	free (bonus.pids);
	return (0);
}


/*
skanna@k1r2p5:Documents/Pipex ‹pipex_v2*›$ valgrind --track-fds=yes ./pipex_bonus infile.txt cat  wc outfile
==792701== Memcheck, a memory error detector
==792701== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==792701== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==792701== Command: ./pipex_bonus infile.txt cat wc outfile
==792701== 
==792702== Syscall param close(fd) contains uninitialised byte(s)
==792702==    at 0x4985F67: close (close.c:27)
==792702==    by 0x10934E: close_fds (bonus.c:25)
==792702==    by 0x1093CF: exec_cmd (bonus.c:39)
==792702==    by 0x109521: dup_fds (bonus.c:66)
==792702==    by 0x109617: fork_children (bonus.c:84)
==792702==    by 0x10A583: main (bonus_main.c:35)
==792702== 
cat: -: Bad file descriptor
cat: closing standard input: Bad file descriptor
==792701== Syscall param wait4(pid) contains uninitialised byte(s)
==792701==    at 0x495B3EA: wait4 (wait4.c:30)
==792701==    by 0x10A5B1: main (bonus_main.c:40)
==792701== 
Waitpid returned error: No child processes
==792701== 
==792701== FILE DESCRIPTORS: 6 open (3 std) at exit.
==792701== Open file descriptor 4: outfile
==792701==    at 0x498553B: open (open64.c:41)
==792701==    by 0x10991C: set_outfile (bonus_utils.c:48)
==792701==    by 0x10959F: fork_children (bonus.c:77)
==792701==    by 0x10A583: main (bonus_main.c:35)
==792701== 
==792701== Open file descriptor 5: infile.txt
==792701==    at 0x498553B: open (open64.c:41)
==792701==    by 0x1099B1: set_infile (bonus_utils.c:63)
==792701==    by 0x109589: fork_children (bonus.c:76)
==792701==    by 0x10A583: main (bonus_main.c:35)
==792701== 
==792701== Open file descriptor 3:
==792701==    at 0x498607D: pipe (pipe.c:29)
==792701==    by 0x109688: create_pipes (bonus.c:98)
==792701==    by 0x10A56C: main (bonus_main.c:34)
==792701== 
==792701== 
==792701== HEAP SUMMARY:
==792701==     in use at exit: 40 bytes in 4 blocks
==792701==   total heap usage: 6 allocs, 2 frees, 1,536 bytes allocated
==792701== 
==792701== LEAK SUMMARY:
==792701==    definitely lost: 0 bytes in 0 blocks
==792701==    indirectly lost: 0 bytes in 0 blocks
==792701==      possibly lost: 0 bytes in 0 blocks
==792701==    still reachable: 40 bytes in 4 blocks
==792701==         suppressed: 0 bytes in 0 blocks
==792701== Rerun with --leak-check=full to see details of leaked memory
==792701== 
==792701== Use --track-origins=yes to see where uninitialised values come from
==792701== For lists of detected and suppressed errors, rerun with: -s
==792701== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

*/