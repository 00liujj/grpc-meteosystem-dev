#!/usr/bin/env bash

awk '{ for (i = 0.0; i <= 45.0; i+= 1.25) printf("Lat. %.2f No. of pts. %d \t Lat. %.2f No. of pts. %d\n", \
    i, 2.0+(90.0/1.25)*cos(i*(atan2(0, -1)/180)), i+45.0, 2.0+(90.0/1.25)*cos((i+45.0)*(atan2(0, -1)/180))); \
    for (i = 0.0; i <= 90.0; i+= 1.25) printf ("%d,", int(2.0+(90.0/1.25)*cos(i*(atan2(0, -1)/180)))*4-4); \
    exit 1 }'
