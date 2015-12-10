# This is a simple shell tests script

# Compile the whole
make lora&& make CommandLine.o
# 
cd tests
make && ./CommandLineTest
