/* =====================================================================
 * Title:        Mul_fp32.c
 * Description:
 *
 * Date:         
 *
 * ===================================================================== */

/*
 * Copyright (C) 2025 ETH Zurich and University of Bologna.
 *
 * Authors:
 * - Philip Wiese, ETH Zurich
 * - Calin Diaconu, University of Bologna
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DeeployBasicMath.h"

void Mul_fp32_fp32( const float32_t *__restrict__ pSrcA,
                    const float32_t *__restrict__ pSrcB,
                    float32_t *__restrict__ pDstC,
                    int32_t max_len, int32_t* shape1, int32_t* shape2){

//prepare the product length
uint32_t prod1[max_len];
uint32_t prod2[max_len];

uint32_t mul1=1;
 for(int i=max_len-1; i>=0;i--){
    prod1[i]=mul1;
    mul1=mul1*shape1[i];
}
uint32_t mul2=1;
for(int i=max_len-1; i>=0;i--){
    prod2[i]=mul2;
    mul2=mul2*shape2[i];
}


uint32_t shape_out[max_len];
for(int i=0; i<max_len;i++){
    shape_out[i]=MAX(shape1[i], shape2[i]);
}

uint32_t mul_out=1;
uint32_t prod_out[max_len];
for(int i=max_len-1; i>=0;i--){//3 2 1 0 
    prod_out[i]=mul_out;
    mul_out=mul_out*shape_out[i];
}
//mul out doubles as size of output
uint32_t posfinalA;
uint32_t posfinalB;
uint32_t rem=0;
uint32_t quotient=0;
for(uint32_t i=0; i<mul_out;i++){
    rem=i;
    quotient=0;
    posfinalA=0;
    posfinalB=0;
    for(uint32_t dim=0; dim<max_len;dim++){
        quotient=rem/prod_out[dim];
        rem=rem%prod_out[dim];

        if (shape1[dim]==1){
            posfinalA=posfinalA+0;
        }else{
            posfinalA=posfinalA+quotient*prod1[dim];
        }
        if (shape2[dim]==1){
            posfinalB=posfinalB+0;
        }else{
            posfinalB=posfinalB+quotient*prod2[dim];
        }
        //posfinalA=posfinalA+posA[dim]*prod1[dim];
        //posfinalB=posfinalB+posB[dim]*prod2[dim];
    }
    pDstC[i]=pSrcA[posfinalA]*pSrcB[posfinalB];
    }
}