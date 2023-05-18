prev_random = 0

current_random = 0cbf
// trampoline ID A
call trampoline
	
jmp to .L2 // L2 is the end of the loop body

.L5

  current_random = ef81
  // trampoline ID B
  call trampoline
  
  compare inp[i] with 'a' (i.e., with $97)
  
  if not equal jump to .L3

  current_random = ae5a
  // trampoline ID C
  call trampoline
  
  increment ca (see instruction addl)

.L3

  current_random = ee16
  // trampoline ID D
  call trampoline

  compare inp[i] with 'b' (i.e., with $98)
  
  if not equal jump to .L4
  
  current_random = cbe3
  // trampoline ID E
  call trampoline

  increment cb (see instruction addl)

.L4

  current_random = e501
  // trampoline ID F
  call trampoline

  increment i

.L2
	
  current_random = 272b
  // trampoline ID G
  trampoline

  compare inp[i] with zero

  if not equal jump to .L5 // L5 is the first instruction inside the loop body

  current_random = a6e2
  // trampoline ID H
  call trampoline

  compute ca+cb (see addl)

  if stack is  ok jump to .L7 (see "je")

  current_random = cf4f
  // trampoline ID I
  call trampoline

  call __stack_chk_fail

.L7

  current_random = 18c6
  // trampoline ID J
  call trampoline

  return

Definition of trampoline:
            x =    prev_random >> 1 xor current_random
	    increment shm[x]
            prev_random = current_random
