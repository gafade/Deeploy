/* =====================================================================
 * Title:        Mul.h
 * Description:
 *
 * Date:         
 *
 * ===================================================================== */

/*
 * Copyright (C) 2022 ETH Zurich and University of Bologna.
 *
 * Authors:
 * - Moritz Scherer, ETH Zurich
 * - Philip Wiese, ETH Zurich
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

#ifndef __DEEPLOY_BASIC_MATH_MUL_KERNEL_HEADER_
#define __DEEPLOY_BASIC_MATH_MUL_KERNEL_HEADER_

#include "DeeployBasicMath.h"

/*
 *
 */

/******************************************************************************/
/*                              Multiplication (32bit)                        */
/******************************************************************************/

void Mul_fp32_fp32( const float32_t *__restrict__ pSrcA,
                    const float32_t *__restrict__ pSrcB,
                    float32_t *__restrict__ pDstC,
                    int32_t* inputA, int32_t shapeA_len,
                    int32_t* inputB,  int32_t shapeB_len);

void Mul_s8_s8_s32( const int8_t *__restrict__ pSrcA, int32_t A_offset,
                    const int8_t *__restrict__ pSrcB, int32_t B_offset,
                    int32_t *__restrict__ pDstC, int32_t C_offset,
                    int32_t* inputA, int32_t shapeA_len,
                    int32_t* inputB,  int32_t shapeB_len);

void Mul_s_s_s32( const void *__restrict__ pSrcA,int32_t typeA, int64_t A_offset,
                    const void *__restrict__ pSrcB,int32_t typeB, int64_t B_offset,
                    int32_t *__restrict__ pDstC, int64_t C_offset,
                    int32_t* inputA, int32_t shapeA_len,
                    int32_t* inputB,  int32_t shapeB_len);


#endif //__DEEPLOY_BASIC_MATH_MUL_KERNEL_HEADER_