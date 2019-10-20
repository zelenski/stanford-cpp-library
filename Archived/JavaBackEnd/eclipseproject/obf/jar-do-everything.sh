#!/bin/bash
# This shell script runs all 3 parts of the JAR-making process:
# 1) make the JAR (un-obfuscated)
# 2) obfuscate it
# 3) copy it to the provided/ dir below

./makejar.sh
./obfuscate.sh
./copyobf.sh
