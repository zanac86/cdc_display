set prog=D:\ST\STM32Cube\STM32CubeProgrammer\bin\STM32_Programmer_CLI.exe

set fw=.\Release\test_cdc4.elf

%prog% -c port=SWD -d %fw% -rst -run
