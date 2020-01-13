//
//  main.c
//  lzo
//
//  Created by garin on 2020/1/13.
//  Copyright © 2020 xiaomi. All rights reserved.
//

#include <stdio.h>
#include "testmini.h"

int main(int argc, const char * argv[]) {
    if (argc < 0 && argv == NULL)   /* avoid warning about unused args */
        return 0;

    testmini();

    return 0;
}
