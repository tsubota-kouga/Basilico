
scriptencoding utf-8
if !exists('g:loaded_basilico_gen')
    finish
endif

let s:save_cpo = &cpo
set cpo&vim

let s:this_dir = expand('<sfile>:p:h')

function! basilico_gen#generator(...) abort
    tabnew
    if a:0 > 0
        if a:1 == 'np'
            execute 'terminal python3 ' . s:this_dir . '/../gen.py ' . $NVIM_LISTEN_ADDRESS . ' np'
            return
        endif
    endif
    execute 'terminal python3 ' . s:this_dir . '/../gen.py ' . $NVIM_LISTEN_ADDRESS
endfunction

function! basilico_gen#build() abort
    tabnew
    execute 'cd' . s:this_dir . '/../'
    execute 'terminal bash ' . s:this_dir . '/../build.sh'
endfunction

let &cpo = s:save_cpo
unlet s:save_cpo
