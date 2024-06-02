# 1 "/home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-SSE-310-MPS3_FVP-CM85/sct/VHT_Corstone_SSE-310.sct"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "/home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-SSE-310-MPS3_FVP-CM85/sct/VHT_Corstone_SSE-310.sct"
# 69 "/home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-SSE-310-MPS3_FVP-CM85/sct/VHT_Corstone_SSE-310.sct"
LR_ROM0 0x00000000 0x00200000 {

  ER_ROM0 0x00000000 0x00200000 {
    *.o (RESET, +First)
    *(InRoot$$Sections)
    *(+RO +XO)
  }







  RW_NOINIT 0x20000000 UNINIT (0x00020000 - 0x00000C00 - 0x00000200 - 0) {
    *(.bss.noinit)
  }

  RW_RAM0 AlignExpr(+0, 8) (0x00020000 - 0x00000C00 - 0x00000200 - 0 - AlignExpr(ImageLength(RW_NOINIT), 8)) {
    *(+RW +ZI)
  }


  ARM_LIB_HEAP (AlignExpr(+0, 8)) EMPTY 0x00000C00 { ; Reserve empty region for heap
  }


  ARM_LIB_STACK (0x20000000 + 0x00020000 - 0) EMPTY -0x00000200 { ; Reserve empty region for stack
  }







  RW_RAM1 0x20200000 0x00020000 {
   .ANY (+RW +ZI)
  }
# 121 "/home/ubuntu/new/AVH-RTTHREAD-DEMO-RECAN/rt-thread/bsp/arm/AVH-SSE-310-MPS3_FVP-CM85/sct/VHT_Corstone_SSE-310.sct"
}


LR_ROM1 0x00200000 0x00200000 {
  ER_ROM1 +0 0x00200000 {
   .ANY (+RO +XO)
  }
}
