program gauss_easter
    implicit none
    
    integer :: i, n_args, n_years, year, month, day, ios
    character(len=32) :: arg
    character(len=64) :: note
    integer, allocatable :: years(:)
    
    n_args = command_argument_count()
    
    if (n_args > 0) then
        allocate(years(n_args))
        do i = 1, n_args
            call get_command_argument(i, arg)
            read(arg, *, iostat=ios) years(i)
            if (ios /= 0) then
                print *, "Error: Invalid year argument: ", trim(arg)
                stop
            end if
        end do
        n_years = n_args
    else
        print *, "Enter the number of years to calculate:"
        read(*, *, iostat=ios) n_years
        if (ios /= 0 .or. n_years < 1) then
            print *, "Error: Invalid number of years"
            stop
        end if
        
        allocate(years(n_years))
        print *, "Enter ", n_years, " year number(s):"
        read(*, *, iostat=ios) years
        if (ios /= 0) then
            print *, "Error: Could not read year values"
            stop
        end if
    end if
    
    print *, ""
    
    do i = 1, n_years
        year = years(i)
        call calculate_easter(year, month, day, note)
        if (month == 3) then
            print '(A,I0,A,I0,A)', "Year ", year, ": Easter Sunday is March ", day, trim(note)
        else
            print '(A,I0,A,I0,A)', "Year ", year, ": Easter Sunday is April ", day, trim(note)
        end if
    end do
    
    if (allocated(years)) deallocate(years)
    
contains

    subroutine calculate_easter(year, month, day, note)
        integer, intent(in) :: year
        integer, intent(out) :: month
        integer, intent(out) :: day
        character(len=64), intent(out) :: note
        
        integer :: a, b, c, k, p, q, M_val, N_val, d, e
        
        note = ""
        
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
        
        if (d + e <= 9) then
            month = 3
            day = 22 + d + e
        else
            month = 4
            day = d + e - 9
        end if
        
        if (d == 29 .and. e == 6) then
            month = 4
            day = 19
            note = "  [Exception: d=29, e=6]"
        end if
        
        if (d == 28 .and. e == 6) then
            if (mod(11 * M_val + 11, 30) < 19) then
                month = 4
                day = 18
                note = "  [Exception: d=28, e=6]"
            end if
        end if
    end subroutine calculate_easter

end program gauss_easter
