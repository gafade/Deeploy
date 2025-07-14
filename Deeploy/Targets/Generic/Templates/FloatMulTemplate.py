# ----------------------------------------------------------------------
#
# File: MulTemplate.py
#
# Last edited: 02.09.2022
#
# Copyright (C) 2022, ETH Zurich and University of Bologna.
#
# Author: Moritz Scherer, ETH Zurich
#
# ----------------------------------------------------------------------
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the License); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an AS IS BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from Deeploy.DeeployTypes import NodeTemplate

referenceTemplate =NodeTemplate("""
// Float Mul (Name: ${nodeName}, Op: ${nodeOp})
BEGIN_SINGLE_CORE

${A_type.typeName} ref_${C}_${A} = ${A};
${B_type.typeName} ref_${C}_${B} = ${B};
${C_type.typeName} ref_${C}_${C} = ${C};
//reshape the smallest one to ensure they have the same len of shapes
//this process is independant of any type of A or B

int32_t max_len_${C}=MAX(${shapeA_len}, ${shapeB_len});
int32_t shape1_${C}[max_len_${C}];
int32_t shape2_${C}[max_len_${C}];

int32_t inputA_${C}[${shapeA_len}]=${shapeA};
int32_t inputB_${C}[${shapeB_len}]=${shapeB};

for(int i=0; i<max_len_${C};i++){
    if(i<max_len_${C}-${shapeA_len}){
    shape1_${C}[i]=1;
    }else{
        shape1_${C}[i]=inputA_${C}[i-max_len_${C}+${shapeA_len}];
    }
}
for(int i=0; i<max_len_${C};i++){
    if(i<max_len_${C}-${shapeB_len}){
    shape2_${C}[i]=1;
    }else{
        shape2_${C}[i]=inputB_${C}[i-max_len_${C}+${shapeB_len}];
    }
}

Mul_fp${A_type.referencedType.typeWidth}_fp${B_type.referencedType.typeWidth}(
ref_${C}_${A} , ref_${C}_${B}, ref_${C}_${C}, 
max_len_${C}, shape1_${C}, shape2_${C}
);
//probably here free the arrays of _${C}

END_SINGLE_CORE
""")

