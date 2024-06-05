# Project Inormation

This is a demo project for porting RT-Thread RTOS on Arm AVH with FVP_MPS2_Cortex-M85 model.

# Get Started

It use makefile to build this project.

- `make help` : To get some helpful information.
- `make build`: To build this project with cbuild tools.
- `make clean`: To clean temp built objects.
- `make run`: To run the built out sample file (*.axf).
- `make debug`: The operation `make build + make run`.
- `make all`: The operation `make clean + make build + make run`.
- `make stop`: To stop running *.axf file.


# Log msg

```bash
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ make help 
make help       -> Show this help msg.
make source     -> Install env parameters.
make build      -> Build thie demo.
make clean      -> Clean object files.
make run        -> Run this demo.
make debug      -> Build & run this demo.
make all        -> Source & clean & build & run all together.
make stop    -> Stop to run this demo.
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ make clean
Clean ...
rm -rf ./Objects/
rm -rf out
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ make build
Building ...
cbuild --packs ./AVH-FVP_MPS2_Cortex-M85.cprj
info cbuild: Build Invocation 1.2.0 (C) 2022 ARM
(cbuildgen): Build Process Manager 1.3.0 (C) 2022 Arm Ltd. and Contributors
M650: Command completed successfully.
(cbuildgen): Build Process Manager 1.3.0 (C) 2022 Arm Ltd. and Contributors
M652: Generated file for project build: '/home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85/Objects/CMakeLists.txt'
-- The ASM compiler identification is ARMClang
-- Found assembler: /opt/armcompiler/bin/armclang
-- Configuring done
-- Generating done
-- Build files have been written to: /home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85/Objects
[55/55] Linking C executable image.axf
"/home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85/RTE/Device/ARMCM85/ARMCM85_ac6.sct", line 22 (column 7): Warning: L6314W: No section matches pattern *(.bss.noinit).
Program Size: Code=37976 RO-data=5560 RW-data=468 ZI-data=52776  
Finished: 0 information, 1 warning and 0 error messages.
info cbuild: build finished successfully!
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ make run
Running ...
/opt/VHT/bin/FVP_MPS2_Cortex-M85 --stat --simlimit 80000000 -f ./vht_config.txt out/image.axf &
telnetterminal2: Listening for serial connection on port 5000
telnetterminal1: Listening for serial connection on port 5001
telnetterminal0: Listening for serial connection on port 5002

 \ | /
- RT -     Thread Operating System
 / | \     5.2.0 build Jun  5 2024 23:41:46
 2006 - 2024 Copyright by RT-Thread team
Debug server create success, accepting clients @ port 12345 ...
Conncting debug server ... Conncted debug server ok ...
msh > 
msh > ls
ls: command not found.

msh > 
msh > help
RT-Thread shell commands:
pin              - pin [option]
clear            - clear the terminal screen
version          - show RT-Thread version information
list             - list objects
help             - RT-Thread shell help
ps               - List threads in the system
free             - Show the memory usage in the system


msh > 
msh > ps
thread       pri  status      sp     stack size max used left tick   error  tcb addr
------------ ---  ------- ---------- ----------  ------  ---------- ------- ----------
debug-clien   10  running 0x00000044 0x00000800    76%   0x00000013 OK      0x20002978
tshell        20  suspend 0x0000007c 0x00001000    03%   0x0000000a EINTRPT 0x200018a8
tidle0        31  ready   0x00000044 0x00000100    26%   0x00000018 OK      0x20000768
timer          4  suspend 0x00000074 0x00000200    22%   0x0000000a EINTRPT 0x2000042c
main          10  suspend 0x00000234 0x00000800    30%   0x00000013 EINTRPT 0x20000da0

msh > 
msh > free
memheap       pool size  max used size available size
------------ ---------- ------------- --------------
heap         258048     9824          248224

msh > 
msh > version

 \ | /
- RT -     Thread Operating System
 / | \     5.2.0 build Jun  5 2024 23:41:46
 2006 - 2024 Copyright by RT-Thread team

msh > 
msh > echo 
echo: command not found.

msh > ^C
Stopping simulation...


Info: /OSCI/SystemC: Simulation stopped by user.

ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
--- FVP_MPS2_Cortex_M85 statistics: -------------------------------------------
Simulated time                          : 1.165802s
User time                               : 0.385812s
System time                             : 0.017409s
Wall time                               : 23.490982s
Performance index                       : 0.05
cpu0                                    :   1.24 MIPS (    29143818 Inst)
cpu1                                    :   0.00 MIPS (           0 Inst)
Total                                   :   1.24 MIPS (    29143818 Inst)
Memory highwater mark                   : 0x170e7000 bytes ( 0.360 GB )
-------------------------------------------------------------------------------

ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 
ubuntu@arm-43ecd4886e664dffa086d3a69133a5e5:~/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-FVP_MPS2_Cortex-M85$ 

```


