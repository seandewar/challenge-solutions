" Add comment markers and trims trailing whitespace for a list of lines
function! s:CommentifyLines(lines) abort
    if !empty(&commentstring)
        let cms = &commentstring->substitute('\s\+%s', '%s', '')
                              \ ->substitute('%s\s\+', '%s', '')
        call map(a:lines, {_, v -> printf(cms, ' ' . v . ' ')})
    endif
    call map(a:lines, {_, v -> substitute(v, '^\(.\{-}\)\s*$', '\1', '')})
    return a:lines
endfunction

" Inserts main solution header at beginning of file
function! s:AddLcMainHeader() abort
    let bname_no_ext = expand('%:t:r')
    let lines = ['https://leetcode.com/problems/'
                \ . (empty(bname_no_ext) ? 'TODO' : bname_no_ext),
                \ 'Complexity: runtime O(TODO), space O(TODO).']
    let lines = s:CommentifyLines(lines) + ['']
    call append(0, lines)
endfunction

" Inserts alternative solution header above cursor line
function! s:AddLcAltHeader() abort
    let lines = ['Alternative Solution: TODO.',
                \ 'Complexity: runtime O(TODO), space O(TODO).']
    let lines = [''] + s:CommentifyLines(lines) + ['', '']
    call append(line('.'), lines)
    call cursor(line('.') + len(lines), 0)
endfunction

let s:parent = expand('<sfile>:p:h') .. '/'
function! s:MaybeSetupBuffer() abort
    if expand('%:p')[:len(s:parent) - 1] !=# s:parent || !empty(&buftype)
                \ || !&modifiable
        return
    endif

    command! -bar -buffer LCMainHeader call s:AddLcMainHeader()
    command! -bar -buffer LCAltHeader call s:AddLcAltHeader()

    if line('$') == 1 && empty(getline(1)) && !empty(&commentstring)
        LCMainHeader
    endif

    setlocal spell
    redraw " force a redraw now so the message has a chance to show
    echo "Use :LCAltHeader to add a header for an alternative solution."
endfunction

augroup challenge_solutions_leetcode
    autocmd!
    autocmd BufNewFile,BufReadPost *
                \  silent! LspAutoOff
                \| call timer_start(0, {-> s:MaybeSetupBuffer()})
augroup END
