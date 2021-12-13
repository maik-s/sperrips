# Ping replaying gibt unterschiedliche Werte bei prctl aus

Auf dem alten kernel 5.4 failed die aufrufe. Liegt daran dass danach neue funktionien für CAP hinzugefügt werden
Siehe:
- https://github.com/torvalds/linux/blame/master/kernel/sys.c#L2481
- https://github.com/torvalds/linux/commit/201698626fbca1cf1a3b686ba14cf2a056500716

- https://github.com/torvalds/linux/commit/124ea650d3072b005457faed69909221c2905a1f



# prctl1
param1: 23 -> PR_CAPBSET_READ
PR_CAPBSET_READ usage: https://elixir.bootlin.com/linux/latest/source/security/commoncap.c#L1275
cap_valid: https://elixir.bootlin.com/linux/latest/source/include/uapi/linux/capability.h#L422
param2 36 = CAP_BLOCK_SUSPEND  vs 44

Binary search in: _initialize_libcap -> aufgerufen von ld
https://git.kernel.org/pub/scm/linux/kernel/git/morgan/libcap.git/tree/libcap/libcap.h?h=libcap-2.32#n212

parameter: binsearc(0,64) => mid =32
siehe python script für nachrechnung

abhängig von last_cap : 5.4.158: https://elixir.bootlin.com/linux/v5.4.158/source/include/uapi/linux/capability.h#L370

Full stack trace :

gdb-peda$ where
#0  cap_get_bound (cap=cap@entry=0x20) at cap_proc.c:225
#1  0x00007ffff7fc0406 in _initialize_libcap () at cap_alloc.c:20
#2  0x00007ffff7fe0b8a in call_init (l=<optimized out>, argc=argc@entry=0x3, argv=argv@entry=0x7fffffffe538,
    env=env@entry=0x7fffffffe558) at dl-init.c:72
#3  0x00007ffff7fe0c91 in call_init (env=0x7fffffffe558, argv=0x7fffffffe538, argc=0x3, l=<optimized out>) at dl-init.c:30
#4  _dl_init (main_map=0x7ffff7ffe190, argc=0x3, argv=0x7fffffffe538, env=0x7fffffffe558) at dl-init.c:119
#5  0x00007ffff7fd013a in _dl_start_user () from /lib64/ld-linux-x86-64.so.2

# prctl2
# prctl3