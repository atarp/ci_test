program find_next_exception
    implicit none
    
    integer :: year, a, b, c, k, p, q, M_val, N_val, d, e
    logical :: found
    
    found = .false.
    
    do year = 2026, 9999
        a = mod(year, 19)
        b = mod(year, 4)
        c = mod(year, 7)
        k = year / 100
        p = (13 + 8 * k) / 25
        q = k / 4
        M_val = mod(15 - p + k - q, 30)
        N_val = mod(4 + k - q, 7)
        d = mod(19 * a + M_val, 30)
        e = mod(2 * b + 4 * c + 6 * d + N_val, 7)
        
        if (d == 29 .and. e == 6) then
            print '(A,I0,A)', "Year ", year, ": Exception d=29, e=6 (April 19)"
            found = .true.
            exit
        end if
        
        if (d == 28 .and. e == 6) then
            if (mod(11 * M_val + 11, 30) < 19) then
                print '(A,I0,A)', "Year ", year, ": Exception d=28, e=6 (April 18)"
                found = .true.
                exit
            end if
        end if
    end do
    
    if (.not. found) then
        print *, "No exception found in the searched range."
    end if
    
end program find_next_exception
