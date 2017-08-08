#!/bin/bash

st-flash write build/main.bin 0x08000000
st-flash reset