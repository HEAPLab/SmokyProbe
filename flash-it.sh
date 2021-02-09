#!/bin/bash
st-info --probe
st-flash write firmware/nucleo-g0/Release/smokyprobe.bin 0x8000000

