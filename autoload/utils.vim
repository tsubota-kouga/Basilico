
scriptencoding utf-8
if !exists('g:loaded_pakuvim')
    finish
endif

let s:save_cpo = &cpo
set cpo&vim

" function! utils#get_buffer()
"     let buf_id = nvim_get_current_buf()
"     return 
" endfunction

let &cpo = s:save_cpo
unlet s:save_cpo
