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

void prepare(const int32_t* __restrict__ inputA, int32_t shapeA_len,
            const int32_t* __restrict__ inputB,  int32_t shapeB_len, 
            int32_t max_len,
            int32_t* shape1, int32_t* shape2,
            int32_t* prod1, int32_t* prod2,
            int32_t* shape_out, int32_t* mul_out){//mul_out int* to modify it

    //prepend the "1"s
    for(int i=0; i<max_len;i++){
        if(i<max_len-shapeA_len){
        shape1[i]=1;
        }else{
            shape1[i]=inputA[i-max_len+shapeA_len];
        }
    }
    for(int i=0; i<max_len;i++){
        if(i<max_len-shapeB_len){
        shape2[i]=1;
        }else{
            shape2[i]=inputB[i-max_len+shapeB_len];
        }
    }

    //prepare the product length
    uint32_t mul1=1;
    for(int i=max_len-1; i>=0;i--){//3 2 1 0 
        prod1[i]=mul1;
        mul1=mul1*shape1[i];
    }
    uint32_t mul2=1;
    for(int i=max_len-1; i>=0;i--){//3 2 1 0 
        prod2[i]=mul2;
        mul2=mul2*shape2[i];
    }

    //shape_out
    for(int i=0; i<max_len;i++){
        shape_out[i]=MAX(shape1[i], shape2[i]);
        *mul_out=(*mul_out)*shape_out[i];
    }


}


void Mul_s8_s8_s32( const int8_t *__restrict__ pSrcA, int32_t A_offset,
                    const int8_t *__restrict__ pSrcB, int32_t B_offset,
                    int32_t *__restrict__ pDstC, int32_t C_offset,
                    int32_t* inputA, int32_t shapeA_len,
                    int32_t* inputB,  int32_t shapeB_len){

int32_t max_len=MAX(shapeA_len,shapeB_len);
int32_t shape1[max_len];
int32_t shape2[max_len];

//prepare the product length
uint32_t prod1[max_len];
uint32_t prod2[max_len];

//shape_out
uint32_t shape_out[max_len];
uint32_t mul_out=1;
//mul out doubles as size of output

prepare(inputA, inputB, shapeA_len, shapeB_len,
max_len, shape1, shape2,prod1, prod2,shape_out,&mul_out );


uint32_t posfinalA;
uint32_t posfinalB;

uint32_t quotients[max_len];
for(uint32_t i=0; i<max_len;i++){
    quotients[i]=0;
}
for(uint32_t i=0; i<mul_out;i++){
    posfinalA=0;
    posfinalB=0;

    //Mixed Radix Conversion to get the indices, but we know the previous result
    for(int32_t dim=max_len-1; dim>=0;dim--){
        if (quotients[dim]>=shape_out[dim]){
            quotients[dim]=0;
            quotients[dim-1]+=1;//should always stop before "quotients[0]==shape_out[0]" --> "quotients[-1]"
        }else{
            break;//quotients are all correctly updated
        }
    }
    for(int32_t dim=0; dim<max_len;dim++){
        if (shape1[dim]!=1){
            posfinalA=posfinalA+quotients[dim]*prod1[dim];
        }//else posfinalA+=quotients[dim]*0
        if (shape2[dim]!=1){
            posfinalB=posfinalB+quotients[dim]*prod2[dim];
        }//else posfinalB+=quotients[dim]*0
        }
    pDstC[i]=(pSrcA[posfinalA]+A_offset)*(pSrcB[posfinalB]+B_offset) + C_offset;
    quotients[max_len-1]+=1;
    } 
    
}

void Mul_s_s_s32( const void *__restrict__ pSrcA,int32_t typeA, int64_t A_offset,
                    const void *__restrict__ pSrcB,int32_t typeB, int64_t B_offset,
                    int32_t *__restrict__ pDstC, int64_t C_offset,
                    int32_t* inputA, int32_t shapeA_len,
                    int32_t* inputB,  int32_t shapeB_len){

/*switch(typeA){
    case(8):
    int8_t* pSrcA_8=(int8_t*) pSrcA;
    case(16):
    int16_t* pSrcA_16=(int16_t*) pSrcA;
    case(32):
    int32_t* pSrcA_32=(int32_t*) pSrcA;
    case(64):
    int64_t* pSrcA_64=(int64_t*) pSrcA;
}

switch(typeB){
    case(8):
    int8_t* pSrcB_c=(int8_t*) pSrcB;
    case(16):
    int16_t* pSrcB_c=(int16_t*) pSrcB;
    case(32):
    int32_t* pSrcB_c=(int32_t*) pSrcB;
    case(64):
    int64_t* pSrcB_c=(int64_t*) pSrcB;
}*/

//The old Template
int32_t max_len=MAX(shapeA_len,shapeB_len);
int32_t shape1[max_len];
int32_t shape2[max_len];

//int32_t inputA[shapeA_len];
//int32_t inputB[shapeB_len];

//prepend the "1"s
for(int i=0; i<max_len;i++){
    if(i<max_len-shapeA_len){
    shape1[i]=1;
    }else{
        shape1[i]=inputA[i-max_len+shapeA_len];
    }
}
for(int i=0; i<max_len;i++){
    if(i<max_len-shapeB_len){
    shape2[i]=1;
    }else{
        shape2[i]=inputB[i-max_len+shapeB_len];
    }
}
//endofTemplate

//prepare the product length
uint32_t prod1[max_len];
uint32_t prod2[max_len];

uint32_t mul1=1;
 for(int i=max_len-1; i>=0;i--){//3 2 1 0 
    prod1[i]=mul1;
    mul1=mul1*shape1[i];
}
uint32_t mul2=1;
for(int i=max_len-1; i>=0;i--){//3 2 1 0 
    prod2[i]=mul2;
    mul2=mul2*shape2[i];
}

//shape_out
uint32_t shape_out[max_len];
uint32_t mul_out=1;
for(int i=0; i<max_len;i++){
    shape_out[i]=MAX(shape1[i], shape2[i]);
    mul_out=mul_out*shape_out[i];
}
//mul out doubles as size of output

uint32_t posfinalA;
uint32_t posfinalB;

uint32_t quotients[max_len];
for(uint32_t i=0; i<max_len;i++){
    quotients[i]=0;
}
for(uint32_t i=0; i<mul_out;i++){
    posfinalA=0;
    posfinalB=0;

    //Mixed Radix Conversion to get the indices, but we know the previous number's result

    //quotients[-1]+=1;
    //if quotients[-1]==shape_out[-1]
    //  quotients[-1]=0;
    //  quotients[-2]+=1;
    //  if quotients[-2]==shape_out[-2]
    //      quotients[-2]=0
    //      ...
    for(int32_t dim=max_len-1; dim>=0;dim--){
        if (quotients[dim]>=shape_out[dim]){
            quotients[dim]=0;
            quotients[dim-1]+=1;//should always stop before "quotients[0]==shape_out[0]" --> "quotients[-1]"
        }else{
            break;//quotients are all correctly updated
        }
    }
    for(int32_t dim=0; dim<max_len;dim++){
        if (shape1[dim]!=1){
            posfinalA=posfinalA+quotients[dim]*prod1[dim];
        }//else posfinalA+=quotients[dim]*0
        if (shape2[dim]!=1){
            posfinalB=posfinalB+quotients[dim]*prod2[dim];
        }//else posfinalB+=quotients[dim]*0
        }
    quotients[max_len-1]+=1;
    
    switch(typeA){
    case(8):

        switch(typeB){
    case(8):
            pDstC[i]=(((int8_t*)pSrcA)[posfinalA]+A_offset)*(((int8_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(16):
            pDstC[i]=(((int8_t*)pSrcA)[posfinalA]+A_offset)*(((int16_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(32):
            pDstC[i]=(((int8_t*)pSrcA)[posfinalA]+A_offset)*(((int32_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(64):
            pDstC[i]=(((int8_t*)pSrcA)[posfinalA]+A_offset)*(((int64_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    
}
break;
    case(16):

        switch(typeB){
    case(8):
            pDstC[i]=(((int16_t*)pSrcA)[posfinalA]+A_offset)*(((int8_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(16):
            pDstC[i]=(((int16_t*)pSrcA)[posfinalA]+A_offset)*(((int16_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(32):
            pDstC[i]=(((int16_t*)pSrcA)[posfinalA]+A_offset)*(((int32_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(64):
            pDstC[i]=(((int16_t*)pSrcA)[posfinalA]+A_offset)*(((int64_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;}
break;
    case(32):

            switch(typeB){
    case(8):
            pDstC[i]=(((int32_t*)pSrcA)[posfinalA]+A_offset)*(((int8_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(16):
            pDstC[i]=(((int32_t*)pSrcA)[posfinalA]+A_offset)*(((int16_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(32):
            pDstC[i]=(((int32_t*)pSrcA)[posfinalA]+A_offset)*(((int32_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(64):
            pDstC[i]=(((int32_t*)pSrcA)[posfinalA]+A_offset)*(((int64_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;}
break;
    case(64):
    
                switch(typeB){
    case(8):
            pDstC[i]=(((int64_t*)pSrcA)[posfinalA]+A_offset)*(((int8_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(16):
            pDstC[i]=(((int64_t*)pSrcA)[posfinalA]+A_offset)*(((int16_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(32):
            pDstC[i]=(((int64_t*)pSrcA)[posfinalA]+A_offset)*(((int32_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
    case(64):
            pDstC[i]=(((int64_t*)pSrcA)[posfinalA]+A_offset)*(((int64_t*)pSrcB)[posfinalB]+B_offset) + C_offset;
            break;
}
break;
    }
    //pDstC[i]=(pSrcA_c[posfinalA]+A_offset)*(pSrcB_c[posfinalB]+B_offset) + C_offset;
    
    }
    
}