
scriptencoding utf-8
if !exists('g:loaded_basilico')
    finish
endif

let s:save_cpo = &cpo
set cpo&vim

let s:this_dir = expand('<sfile>:p:h')
let s:this_port = matchstr(expand('$NVIM_LISTEN_ADDRESS'), '\w*$')

let g:basilico_plugin_default_dir = ''
let g:basilico_plugin_added = []

function! basilico_startup#start()
    let g:basilico#gui_id = jobstart(split(&shell) + split(&shellcmdflag)
                \ + [s:this_dir . '/../build/basil ' . s:this_port])
endfunction

let &cpo = s:save_cpo
unlet s:save_cpo
