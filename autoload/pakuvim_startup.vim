
scriptencoding utf-8
if exists('g:pakuvim#commands')
    finish
endif
let g:pakuvim#commands = 1

let s:save_cpo = &cpo
set cpo&vim

function! Pakuvim_startup()
    let g:pakuvim#gui_id = jobstart(split(&shell) + split(&shellcmdflag) + ['../build/paku 6666'])
endfunction

let &cpo = s:save_cpo
unlet s:save_cpo
