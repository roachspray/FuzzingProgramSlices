#!/bin/bash
clang-4.0 -I. -emit-llvm -c tlm.c
clang-4.0 -I. -o tlm tlm.c
clang-4.0 -I. -o gen_tlm_file gen_tlm_file.c
