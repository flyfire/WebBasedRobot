@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM 


"F:\Program Files\IAR Systems\Embedded Workbench 6.0 Evaluation\common\bin\cspybat" "F:\Program Files\IAR Systems\Embedded Workbench 6.0 Evaluation\arm\bin\armproc.dll" "F:\Program Files\IAR Systems\Embedded Workbench 6.0 Evaluation\arm\bin\armlmiftdi.dll"  %1 --plugin "F:\Program Files\IAR Systems\Embedded Workbench 6.0 Evaluation\arm\bin\armbat.dll" --flash_loader "F:\Program Files\IAR Systems\Embedded Workbench 6.0 Evaluation\arm\config\flashloader\TexasInstruments\FlashLM3Sx8xx.board" --backend -B "--endian=little" "--cpu=Cortex-M3" "--fpu=None" "-p" "F:\Program Files\IAR Systems\Embedded Workbench 6.0 Evaluation\arm\CONFIG\debugger\TexasInstruments\iolm3sxxxx.ddf" "--drv_verify_download" "--semihosting" "--device=LM3Sx9xx" "--lmiftdi_speed=500" 


