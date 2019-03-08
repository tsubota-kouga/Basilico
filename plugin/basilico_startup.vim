
scriptencoding utf-8
if exists('g:loaded_basilico')
    finish
endif
let g:loaded_basilico = 1
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim

if !exists('g:basilico#tabline_style_sheet')
    let g:basilico#tabline_style_sheet=''
endif
if !exists('g:basilico#ext_tabline')
    let g:basilico#ext_tabline=v:false
endif
if !exists('g:basilico#ext_popupmenu')
    let g:basilico#ext_popupmenu=v:false
endif

command! -nargs=0 Basilico call basilico_startup#start()

let &cpo = s:save_cpo
   unlet s:save_cpo
