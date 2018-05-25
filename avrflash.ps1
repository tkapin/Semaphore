# Script for flashing the image from Atmel Studio by using avrdude and the usbasp programmer
#
# Setup a new external tool in the Atmel Studio's 7, 'Tools' -> 'External Tools' menu
#
# Title: &Flash USBasp
# Command: powershell.exe
# Arguments: -File "C:\_main\apps\avrdude-6.3\avrflash.ps1" -projectFileName "$(ProjectFileName)" -projectDir "$(ProjectDir)"  -imageFilePath "$(TargetName).hex"
# Initial directory: $(TargetDir)
# Check 'Use Output Window'

Param(
    [Parameter(Mandatory=$true)] [string]$projectFileName,
    [Parameter(Mandatory=$true)] [string]$projectDir,
    [Parameter(Mandatory=$true)] [string]$imageFilePath
)

# table of Atmel Studio device ID to avrdude device ID
$d2d = @{"ATtiny85" = "t85"; "ATmega328p" = "m328p"}

# due to the bug in Atmel Studio, the path to project file has to be composed form $(ProjectFileName) and $(ProjectDir) variables
[string] $projectFilePath = "$projectDir\\$([System.IO.Path]::GetFileName($projectFileName))"

# get the device ID from the project file and translate to avrdude's device ID
[string] $avrDudeFilePath = "$PSScriptRoot\avrdude.exe"
[string] $atmelStudioDevice = (Select-Xml -Path $projectFilePath -XPath "/x:Project/x:PropertyGroup/x:avrdevice" -Namespace @{"x" = "http://schemas.microsoft.com/developer/msbuild/2003"})[0].Node.InnerText
$avrDudeDevice = $d2d[$atmelStudioDevice]

# flash
Write-Output "Device identified as $atmelStudioDevice ($avrDudeDevice)."
Write-Output "Executing '$avrDudeFilePath -p $avrDudeDevice -c usbasp -U flash:w:$imageFilePath' ..."
& $avrDudeFilePath -p $avrDudeDevice -c usbasp -U flash:w:$imageFilePath
if ($lastExitCode -ne 0) {
    thorw "Avrdude failed to flash image '$imageFilePath'"
}
Write-Output "Successfully flashed image '$imageFilePath' to $atmelStudioDevice ($avrDudeDevice)."