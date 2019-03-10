
scriptencoding utf-8
if exists('g:loaded_basilico_gen')
    finish
endif
let g:loaded_basilico_gen = 1
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim

command -nargs=? BasilGenarate call basilico_gen#generator(<f-args>)
command -nargs=0 BasilBuild call basilico_gen#build()

let &cpo = s:save_cpo
unlet s:save_cpo
