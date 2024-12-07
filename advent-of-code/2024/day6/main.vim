" Nvim 0.11; but should work in Vim too :-)
" Remember: (legacy) Vim script is sloooooow!

let s:dirs = [[0, -1], [1, 0], [0, 1], [-1, 0]]
let s:map = readfile('input')
let s:w = s:map[0]->len()
let s:h = s:map->len()

function! s:WalkEscapes(x, y, dir, obs_x, obs_y, visited_dirs) abort
    let x = a:x
    let y = a:y
    let dir = a:dir
    let visited_dirs = copy(a:visited_dirs)
    silent! unlet visited_dirs[$'{x},{y},{dir}']
    while x >= 0 && x < s:w && y >= 0 && y < s:h
        let k = $'{x},{y},{dir}'
        if visited_dirs->has_key(k) | return v:false | endif
        let visited_dirs[k] = v:true

        while v:true
            let [dx, dy] = s:dirs[dir]
            let next_x = x + dx
            let next_y = y + dy
            if next_x < 0 || next_x >= s:w || next_y < 0 || next_y >= s:h
                        \ || (s:map[next_y][next_x] != '#'
                        \     && (next_x != a:obs_x || next_y != a:obs_y))
                break
            endif
            let dir = (dir + 1) % s:dirs->len()
        endwhile
        let x = next_x
        let y = next_y
    endwhile
    return v:true
endfunction

function! s:DoIt() abort
    for guard_y in range(s:h)
        let guard_x = stridx(s:map[guard_y], '^')
        if guard_x != -1 | break | endif
    endfor

    let x = guard_x
    let y = guard_y
    let dir = 0
    let p1_visited = {}
    let p1_visited_dirs = {}
    let p2 = 0
    while x >= 0 && x < s:w && y >= 0 && y < s:h
        let k = $'{x},{y}'
        if !p1_visited->has_key(k) && (x != guard_x || y != guard_y)
            let [dx, dy] = s:dirs[dir]
            if !s:WalkEscapes(x - dx, y - dy, dir, x, y, p1_visited_dirs)
                let p2 += 1
            endif
        endif
        let p1_visited[k] = v:true
        let p1_visited_dirs[$'{x},{y},{dir}'] = v:true
        echo 'Day6: P1:' p1_visited->len() 'P2:' p2 '...' | redraw

        while v:true
            let [dx, dy] = s:dirs[dir]
            let next_x = x + dx
            let next_y = y + dy
            if next_x < 0 || next_x >= s:w || next_y < 0 || next_y >= s:h
                        \ || s:map[next_y][next_x] != '#'
                break
            endif
            let dir = (dir + 1) % s:dirs->len()
        endwhile
        let x = next_x
        let y = next_y
    endwhile

    echom 'Day6: P1:' p1_visited->len() 'P2:' p2
endfunction

call s:DoIt()
unlet! s:dirs s:map s:w s:h
