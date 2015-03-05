for /R %%f in (*.c;*.cpp;*.cs;) do astyle.exe --style=ansi --indent=spaces=4 -M80 -H -f -p -U -c -w -n "%%f"
