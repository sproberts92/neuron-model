@echo off

for /l %%x in (1, 1, %1) do (		
	echo %%x
	bin\neural-growth.exe
)