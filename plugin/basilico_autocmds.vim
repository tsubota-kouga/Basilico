
scriptencoding utf-8
if exists('g:loaded_basilico_autocmds')
    finish
endif
let g:loaded_basilico_autocmds = 1
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim


let &cpo = s:save_cpo
unlet s:save_cpo
