
scriptencoding utf-8
if exists('g:loaded_basilico')
    finish
endif
let g:loaded_basilico = 1
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim

command! -nargs=0 Basilico call basilico_startup#start()

" au TabEnter * call rpcnotify(0, 'NeoVim#autocmd', 'TabEnter')

let &cpo = s:save_cpo
   unlet s:save_cpo
