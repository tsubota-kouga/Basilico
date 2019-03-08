
function! basilico_tabplugin#make_tab_for_plugin(name) abort
    execute 'tabnew ' . a:name
    execute 'setlocal filetype=' . a:name
    setlocal noswapfile
    setlocal nobuflisted
endfunction

