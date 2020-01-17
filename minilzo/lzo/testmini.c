/* testmini.c -- very simple test program for the miniLZO library

   This file is part of the LZO real-time data compression library.

   Copyright (C) 1996-2017 Markus Franz Xaver Johannes Oberhumer
   All Rights Reserved.

   The LZO library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   The LZO library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with the LZO library; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

   Markus F.X.J. Oberhumer
   <markus@oberhumer.com>
   http://www.oberhumer.com/opensource/lzo/
 */


#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

/*************************************************************************
// This program shows the basic usage of the LZO library.
// We will compress a block of data and decompress again.
//
// For more information, documentation, example programs and other support
// files (like Makefiles and build scripts) please download the full LZO
// package from
//    http://www.oberhumer.com/opensource/lzo/
**************************************************************************/

/* First let's include "minizo.h". */

#include "minilzo.h"
#include <string.h>


/* We want to compress the data block at 'in' with length 'IN_LEN' to
 * the block at 'out'. Because the input block may be incompressible,
 * we must provide a little more output space in case that compression
 * is not possible.
 */

#define IN_LEN      (128*1024ul)
#define OUT_LEN     (IN_LEN + IN_LEN / 16 + 64 + 3)

static unsigned char __LZO_MMODEL in  [ IN_LEN ];
static unsigned char __LZO_MMODEL out [ OUT_LEN ];


/* Work-memory needed for compression. Allocate memory in units
 * of 'lzo_align_t' (instead of 'char') to make sure it is properly aligned.
 */

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);


/*************************************************************************
//
**************************************************************************/

//char *fromPath = "/Users/garin/Documents/mywork/lzo/lzo/file";
//char *toPath = "/Users/garin/Documents/mywork/lzo/lzo/compressed";
char *fromPath = "/Users/mac/Downloads/LearnArduino/hop/frames/image-%04d.bin";
char *toPath = "/Users/mac/Downloads/LearnArduino/hop/compress/image-%04d.bin";
char *lenPath = "/Users/mac/Downloads/LearnArduino/hop/lendata.h";

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

    long pathlen = pathconf(".", _PC_PATH_MAX);
    char *buf;
    if ((buf = (char *)malloc((size_t)pathlen)) != NULL) {
        printf("current working directory: %s\n", getcwd(buf, (size_t)pathlen));
    }

    const int FILENUM = 799;
    const int BUFFLEN = 110;
    
    long totalOrigin = 0;
    long totalCompressed = 0;
    
//    FILE *fw = fopen(toPath, "wb+");
//    if (fw == NULL) {
//        printf("cannot open file.\n");
//        return 0;
//    }

    char *bytes = (char *)malloc(BUFFLEN * sizeof(char));
//    memset(bytes, 0, BUFFLEN * sizeof(char));
//    char *info = "#include <pgmspace.h>\nPROGMEM const int FRAME_NUM = %lu;\nPROGMEM const unsigned char frames[] = {";
//    sprintf(bytes, info, FILENUM);
//    fwrite(bytes, strlen(bytes), 1, fw);

    unsigned long lengths[FILENUM];
    for (int idx = 1; idx <= FILENUM; idx++) {
        
        memset(bytes, 0, BUFFLEN * sizeof(char));
        sprintf(bytes, fromPath, idx);
        
        printf("try opening file %s.\n", bytes);
        
        FILE *fr = fopen(bytes, "rb");  // r for read, b for binary
        if (fr == NULL) {
            printf("cannot open file.\n");
            return 0;
        }
        fseek(fr, 0, SEEK_END); // seek to end of file
        long size = ftell(fr); // get current file pointer
        fseek(fr, 0, SEEK_SET); // seek back to beginning of file
        // proceed with allocating memory and reading the file
        
        if (size > 1024 * 1024) {
            printf("size of file: %.2f MB\n", size / 1024.0 / 1024.0);
        } else if (size > 1024) {
            printf("size of file: %.2f kB\n", size / 1024.0);
        } else {
            printf("size of file: %ld B\n", size);
        }
        
        totalOrigin += size;
/*
 * Step 2: prepare the input block that will get compressed.
 *         We just fill it with zeros in this example program,
 *         but you would use your real-world data here.
 */
//    in_len = IN_LEN;
//    lzo_memset(in,0,in_len);

        in_len = size;
        fread(in, IN_LEN, 1, fr);
        fclose(fr);
        
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
        
        memset(bytes, 0, BUFFLEN * sizeof(char));
        sprintf(bytes, toPath, idx);
        FILE *fw = fopen(bytes, "wb+");
        if (fw == NULL) {
            printf("cannot open file.\n");
            return 0;
        }
        fwrite(out, out_len, 1, fw);
        fclose(fw);

//        for (int b = 0; b < out_len; b++) {
//            memset(bytes, 0, BUFFLEN * sizeof(char));
//            sprintf(bytes, "0x%x,", out[b]);
//            fwrite(bytes, strlen(bytes), 1, fw);
//        }
//        fwrite("\n", 1, 1, fw);
//
        lengths[idx] = out_len;
        totalCompressed += out_len;
    }
    
    char *info = "PROGMEM const int lengths[] = {";
    
    FILE *fw = fopen(lenPath, "w+");
    fwrite(info, strlen(info), 1, fw);

    for (int idx = 1; idx <= FILENUM; idx++) {
        memset(bytes, 0, BUFFLEN * sizeof(char));
        sprintf(bytes, "%lu,", lengths[idx]);
        fwrite(bytes, strlen(bytes), 1, fw);
    }

    info = "};";
    fwrite(info, strlen(info), 1, fw);
    fclose(fw);
    
    printf("Total: %ld -> %ld, compress rate: %lf", totalOrigin, totalCompressed, totalCompressed * 1.0 / totalOrigin);

/*
 * Step 3: compress from 'in' to 'out' with LZO1X-1
 */
//    r = lzo1x_1_compress(in,in_len,out,&out_len,wrkmem);
//    if (r == LZO_E_OK)
//        printf("compressed %lu bytes into %lu bytes\n",
//            (unsigned long) in_len, (unsigned long) out_len);
//    else
//    {
//        /* this should NEVER happen */
//        printf("internal error - compression failed: %d\n", r);
//        return 2;
//    }
//    /* check for an incompressible block */
//    if (out_len >= in_len)
//    {
//        printf("This block contains incompressible data.\n");
//        return 0;
//    }
//
//    FILE *fw = fopen(toPath, "wb");
//    fwrite(out, out_len, 1, fw);

/*
 * Step 4: decompress again, now going from 'out' to 'in'
 */
//    new_len = in_len;
//    r = lzo1x_decompress(out,out_len,in,&new_len,NULL);
//    if (r == LZO_E_OK && new_len == in_len)
//        printf("decompressed %lu bytes back into %lu bytes\n",
//            (unsigned long) out_len, (unsigned long) in_len);
//    else
//    {
//        /* this should NEVER happen */
//        printf("internal error - decompression failed: %d\n", r);
//        return 1;
//    }
//
//    printf("\nminiLZO simple compression test passed.\n");
    return 0;
}


/* vim:set ts=4 sw=4 et: */
