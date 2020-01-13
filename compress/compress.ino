#include <M5StickC.h>
#include "minilzo.h"

#define IN_LEN      (32*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

unsigned char __LZO_MMODEL in  [ IN_LEN ];
unsigned char __LZO_MMODEL out [ OUT_LEN ];

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);

int testmini()
{
    int r;
    lzo_uint in_len;
    lzo_uint out_len;
    lzo_uint new_len;

    printf("\nLZO real-time data compression library (v%s, %s).\n",
           lzo_version_string(), lzo_version_date());
    printf("Copyright (C) 1996-2017 Markus Franz Xaver Johannes Oberhumer\nAll Rights Reserved.\n\n");

/*
 * Step 1: initialize the LZO library
 */
    if (lzo_init() != LZO_E_OK)
    {
        printf("internal error - lzo_init() failed !!!\n");
        printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
        return 3;
    }

/*
 * Step 2: prepare the input block that will get compressed.
 *         We just fill it with zeros in this example program,
 *         but you would use your real-world data here.
 */
    in_len = IN_LEN;
    lzo_memset(in,0,in_len);

/*
 * Step 3: compress from 'in' to 'out' with LZO1X-1
 */
    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
    if (r == LZO_E_OK)
        printf("compressed %lu bytes into %lu bytes\n",
            (unsigned long) in_len, (unsigned long) out_len);
    else
    {
        /* this should NEVER happen */
        printf("internal error - compression failed: %d\n", r);
        return 2;
    }
    /* check for an incompressible block */
    if (out_len >= in_len)
    {
        printf("This block contains incompressible data.\n");
        return 0;
    }
    
/*
 * Step 4: decompress again, now going from 'out' to 'in'
 */
    new_len = in_len;
    r = lzo1x_decompress(out,out_len,in,&new_len,NULL);
    if (r == LZO_E_OK && new_len == in_len)
        printf("decompressed %lu bytes back into %lu bytes\n",
            (unsigned long) out_len, (unsigned long) in_len);
    else
    {
        /* this should NEVER happen */
        printf("internal error - decompression failed: %d\n", r);
        return 1;
    }

    printf("\nminiLZO simple compression test passed.\n");
    return 0;
}


void setup() {
  // put your setup code here, to run once:
  testmini();
}

void loop() {
  // put your main code here, to run repeatedly:

}
