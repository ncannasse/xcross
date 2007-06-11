/* ************************************************************************ */
/*																			*/
/*  XCross - Neko Standalone Runtime										*/
/*  Copyright (c)2006 Motion-Twin											*/
/*																			*/
/* This library is free software; you can redistribute it and/or			*/
/* modify it under the terms of the GNU Lesser General Public				*/
/* License as published by the Free Software Foundation; either				*/
/* version 2.1 of the License, or (at your option) any later version.		*/
/*																			*/
/* This library is distributed in the hope that it will be useful,			*/
/* but WITHOUT ANY WARRANTY; without even the implied warranty of			*/
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU		*/
/* Lesser General Public License or the LICENSE file for more details.		*/
/*																			*/
/* ************************************************************************ */

PRIM(std,buffer_new,0);
PRIM(std,buffer_add,2);
PRIM(std,buffer_add_char,2);
PRIM(std,buffer_add_sub,4);
PRIM(std,buffer_string,1);
PRIM(std,buffer_reset,1);

PRIM(std,date_now,0);
PRIM(std,date_new,1);
PRIM(std,date_format,2);
PRIM(std,date_set_hour,4);
PRIM(std,date_set_day,4);
PRIM(std,date_get_hour,1);
PRIM(std,date_get_day,1);

PRIM(std,file_open,2);
PRIM(std,file_close,1);
PRIM(std,file_name,1);
PRIM(std,file_write,4);
PRIM(std,file_read,4);
PRIM(std,file_write_char,2);
PRIM(std,file_read_char,1);
PRIM(std,file_seek,3);
PRIM(std,file_tell,1);
PRIM(std,file_eof,1);
PRIM(std,file_flush,1);
PRIM(std,file_contents,1);
PRIM(std,file_stdin,0);
PRIM(std,file_stdout,0);
PRIM(std,file_stderr,0);

PRIM(std,int32_new,1);
PRIM(std,int32_to_int,1);
PRIM(std,int32_to_float,1);
PRIM(std,int32_compare,2);
PRIM(std,int32_ushr,2);
PRIM(std,int32_add,2);
PRIM(std,int32_sub,2);
PRIM(std,int32_mul,2);
PRIM(std,int32_div,2);
PRIM(std,int32_shl,2);
PRIM(std,int32_shr,2);
PRIM(std,int32_mod,2);
PRIM(std,int32_neg,1);
PRIM(std,int32_complement,1);
PRIM(std,int32_or,2);
PRIM(std,int32_and,2);
PRIM(std,int32_xor,2);
PRIM(std,int32_address,1);

PRIM(std,math_atan2,2);
PRIM(std,math_pow,2);
PRIM(std,math_abs,1);
PRIM(std,math_ceil,1);
PRIM(std,math_floor,1);
PRIM(std,math_round,1);
PRIM(std,math_pi,0);
PRIM(std,math_sqrt,1);
PRIM(std,math_atan,1);
PRIM(std,math_cos,1);
PRIM(std,math_sin,1);
PRIM(std,math_tan,1);
PRIM(std,math_log,1);
PRIM(std,math_exp,1);
PRIM(std,math_acos,1);
PRIM(std,math_asin,1);

PRIM(std,make_md5,1);

//PRIM(std,mem_size,1);

PRIM(std,module_read,2);
PRIM(std,module_read_path,3);
PRIM(std,module_exec,1);
PRIM(std,module_name,1);
PRIM(std,module_exports,1);
PRIM(std,module_loader,1);
PRIM(std,module_nglobals,1);
PRIM(std,module_global_get,2);
PRIM(std,module_global_set,3);
PRIM(std,module_code_size,1);

PRIM(std,random_new,0);
PRIM(std,random_set_seed,2);
PRIM(std,random_int,2);
PRIM(std,random_float,1);

PRIM(std,serialize,1);
PRIM(std,unserialize,2);

PRIM(std,socket_init,0);
PRIM(std,socket_new,1);
PRIM(std,socket_close,1);
PRIM(std,socket_send_char,2);
PRIM(std,socket_send,4);
PRIM(std,socket_recv,4);
PRIM(std,socket_recv_char,1);
PRIM(std,socket_write,2);
PRIM(std,socket_read,1);
PRIM(std,host_resolve,1);
PRIM(std,host_to_string,1);
PRIM(std,host_reverse,1);
PRIM(std,host_local,0);
PRIM(std,socket_connect,3);
PRIM(std,socket_listen,2);
PRIM(std,socket_select,4);
PRIM(std,socket_bind,3);
PRIM(std,socket_accept,1);
PRIM(std,socket_peer,1);
PRIM(std,socket_host,1);
PRIM(std,socket_set_timeout,2);
PRIM(std,socket_shutdown,3);
PRIM(std,socket_set_blocking,2);
PRIM(std,socket_poll_alloc,1);
PRIM(std,socket_poll,3);

PRIM(std,string_split,2);
PRIM(std,sprintf,2);
PRIM(std,url_decode,1);
PRIM(std,url_encode,1);
PRIM(std,base_encode,2);
PRIM(std,base_decode,2);

PRIM(std,get_env,1);
PRIM(std,put_env,2);
PRIM(std,sys_sleep,1);
PRIM(std,set_time_locale,1);
PRIM(std,get_cwd,0);
PRIM(std,set_cwd,1);
PRIM(std,sys_string,0);
PRIM(std,sys_exit,1);
PRIM(std,sys_command,1);
PRIM(std,sys_exists,1);
PRIM(std,file_delete,1);
PRIM(std,sys_rename,2);
PRIM(std,sys_stat,1);
PRIM(std,sys_file_type,1);
PRIM(std,sys_create_dir,2);
PRIM(std,sys_remove_dir,1);
PRIM(std,sys_time,0);
PRIM(std,sys_cpu_time,0);
PRIM(std,sys_read_dir,1);
PRIM(std,file_full_path,1);
PRIM(std,sys_exe_path,0);
PRIM(std,sys_env,0);
PRIM(std,sys_getch,1);
PRIM(std,sys_get_pid,0);

PRIM(std,utf8_buf_alloc,1);
PRIM(std,utf8_buf_add,2);
PRIM(std,utf8_buf_content,1);
PRIM(std,utf8_buf_length,1);
PRIM(std,utf8_buf_size,1);
PRIM(std,utf8_validate,1);
PRIM(std,utf8_length,1);
PRIM(std,utf8_sub,3);
PRIM(std,utf8_get,2);
PRIM(std,utf8_iter,2);
PRIM(std,utf8_compare,2);

PRIM(std,parse_xml,2);

PRIM(std,thread_create,2);
PRIM(std,thread_current,0);
PRIM(std,thread_send,2);
PRIM(std,thread_read_message,1);
PRIM(std,lock_create,0);
PRIM(std,lock_release,1);
PRIM(std,lock_wait,2);

PRIM(std,float_bytes,2);
PRIM(std,double_bytes,2);
PRIM(std,float_of_bytes,2);
PRIM(std,double_of_bytes,2);
PRIM(std,run_gc,1);
PRIM(std,gc_stats,0);
PRIM(std,enable_jit,1);
PRIM(std,test,0);
PRIM(std,print_redirect,1);

PRIM(regexp,regexp_new_options,2);
PRIM(regexp,regexp_new,1);
PRIM(regexp,regexp_match,4);
PRIM(regexp,regexp_replace,3);
PRIM(regexp,regexp_replace_all,3);
PRIM(regexp,regexp_replace_fun,3);
PRIM(regexp,regexp_matched,2);
PRIM(regexp,regexp_matched_pos,2);

PRIM(zlib,deflate_init,1);
PRIM(zlib,deflate_buffer,5);
PRIM(zlib,deflate_end,1);
PRIM(zlib,inflate_init,1);
PRIM(zlib,inflate_buffer,5);
PRIM(zlib,inflate_end,1);
PRIM(zlib,set_flush_mode,2);
PRIM(zlib,get_adler32,1);
PRIM(zlib,update_adler32,4);
PRIM(zlib,update_crc32,4);
PRIM(zlib,deflate_bound,2);

PRIM(os,os_is_main,0);
PRIM(os,os_loop,0);
PRIM(os,os_stop_loop,0);
PRIM(os,os_sync,1);

PRIM(xcross,os_dialog,4);
PRIM(xcross,os_winlog_new,2);
PRIM(xcross,os_winlog_set,2);
PRIM(xcross,os_winlog_set_button,3);
PRIM(xcross,os_winlog_destroy,1);
PRIM(xcross,os_authorize,0);

/* ************************************************************************ */
