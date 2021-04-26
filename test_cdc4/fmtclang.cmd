for /R .\ %%I in (*.cpp *.c *.h) do clang-format.exe -style=file -i %%I
