# mpi_assignment1


MSMPISetup(1).exe
msmpisdk.msi
msmpidbg.msi

run and install (leave directorys as standard)

open 4_ps.ps1 with powershell and run

( if error - Set-ExecutionPolicy Unrestricted before)



####

mpiexec -n 8 *Path to exe*\ConsoleApplication1.exe 400000

8 number of nodes
4000000 samples per nodes


