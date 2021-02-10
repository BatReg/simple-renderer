@echo off

if not exist build (
    mkdir build
)

pushd build
if [%1] == [] (
    call cmake ../ -A x64
) else (
    if not [%2] == [] (
        call cmake ../ -A x64 -D%1=%2
    ) else (
        call cmake ../ -A x64
    )
)

popd build

pause