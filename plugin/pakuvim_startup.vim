
scriptencoding utf-8
if exists('g:pakuvim#commands')
    finish
endif
let g:loaded_pakuvim = 1

let s:save_cpo = &cpo
set cpo&vim

command! -nargs=0 PakuVim call Pakuvim_startup()

let &cpo = s:save_cpo
unlet s:save_cpo
