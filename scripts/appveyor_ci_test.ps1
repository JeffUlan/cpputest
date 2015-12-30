﻿
# Load functions from the helper file
. (Join-Path (Split-Path $MyInvocation.MyCommand.Path) 'appveyor_helpers.ps1')

function Publish-TestResults($files)
{
    $anyFailures = $FALSE

    # Upload results to AppVeyor one by one
    $files | foreach {
        $testsuite = ([xml](get-content $_.Name)).testsuite

        foreach ($testcase in $testsuite.testcase) {
            if ($testcase.failure) {
                Add-AppveyorTest $testcase.name -Outcome Failed -FileName $testsuite.name -ErrorMessage $testcase.failure.message
                Add-AppveyorMessage "$($testcase.name) failed" -Category Error
                $anyFailures = $TRUE
            }
            elseif ($testcase.skipped) {
                Add-AppveyorTest $testcase.name -Outcome Ignored -Filename $testsuite.name
            }
            else {
                Add-AppveyorTest $testcase.name -Outcome Passed -FileName $testsuite.name
            }
        }

        Remove-Item $_.Name
    }

    if ($anyFailures -eq $TRUE){
        write-host "Failing build as there are broken tests"
        $host.SetShouldExit(1)
    }
}

function Invoke-Tests($executable)
{
    # Run tests and output the results using junit
    $TestCommand = "$executable -ojunit"
    Write-Host $TestCommand
    Invoke-Expression $TestCommand
}

function Invoke-CygwinTests($executable)
{
    # Assume cygwin is located at C:\cygwin for now
    $cygwin_bin = "C:\cygwin\bin"

    # Get the full path to the executable
    $cygwin_folder = . "${cygwin_bin}\cygpath.exe" (Resolve-Path ".")
    $cygwin_exe = . "${cygwin_bin}\cygpath.exe" $executable
    
    # Run tests from the cygwin prompt
    $test_command = "${cygwin_exe} -ojunit"
    $cygwin_command = "${cygwin_bin}\bash.exe --login -c 'cd ${cygwin_folder} ; ${test_command}'"

    Write-Host $test_command
    Invoke-Expression $cygwin_command
}

switch ($env:PlatformToolset)
{
    'Cygwin'
    {
        Invoke-CygwinTests 'cpputest_build\CppUTestTests.exe'
        Invoke-CygwinTests 'cpputest_build\CppUTestExtTests.exe'
    }

    'MinGW'
    {
        $mingw_path = 'C:\Tools\mingw32\bin'
        if ($env:Platform -eq 'x64')
        {
            $mingw_path = 'C:\Tools\mingw64\bin'
        }

        Add-PathFolder $mingw_path
        Invoke-Tests '.\cpputest_build\tests\CppUTestTests.exe'
        Invoke-Tests '.\cpputest_build\tests\CppUTestExt\CppUTestExtTests.exe'
        Remove-PathFolder $mingw_path
    }

    default
    {
        Invoke-Tests '.\cpputest_build\AllTests.exe'
    }
}

Publish-TestResults (Get-ChildItem 'cpputest_*.xml')
