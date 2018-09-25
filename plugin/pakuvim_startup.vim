
scriptencoding utf-8
if exists('g:loaded_pakuvim')
    finish
endif
let g:loaded_pakuvim = 1
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim

command! -nargs=0 PakuVim call pakuvim_startup#start()

let &cpo = s:save_cpo
unlet s:save_cpo
