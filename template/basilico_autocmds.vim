
scriptencoding utf-8
if exists('g:loaded_basilico_autocmds')
    finish
endif
let g:loaded_basilico_autocmds = 1
setlocal runtimepath+=.

let s:save_cpo = &cpo
set cpo&vim

{% for name in message['autocmd']%}
au {{name}} * call rpcnotify(0, 'NeoVim#autocmd', '{{name}}')
{% endfor %}

let &cpo = s:save_cpo
unlet s:save_cpo
