#include "find_pipe.h"
#include "tfp0_krw.h"
#include "proc.h"
#include "offsets.h"
#include <stdio.h>

uint64_t obtain_pipe_kaddr(int rfd) {
	
	uint64_t p_fd = (proc_of_pid(getpid()) + off_p_pfd); 
	uint64_t fileproc = tfp0_kread64(p_fd + off_fd_ofiles);
	uint64_t rpipe_fp = tfp0_kread64(fileproc + rfd * 8);
	uint64_t r_fp_glob = tfp0_kread64(rpipe_fp + off_fp_fglob);
	uint64_t rpipe = tfp0_kread64(r_fp_glob + off_fg_data); 
	uint64_t pipe_base = tfp0_kread64(rpipe + off_pb_buffer); 
	
    return pipe_base;
}