#
#    This build the whole thing. Use examples:
#
#       .\build.ps1 target=rp3 exec=true
#       .\build.ps1 target=rp2 exec=false
#       .\build.ps1 target=rp1 exec=true
#
#     Rafael
#


$target=$args[0]
$exec=$args[1]
$ErrorActionPreference = "Stop"

Write-Host "Building..."

if ( $target -eq "target=rp1" )
{
  Write-Host "     B U I L I D I N G    P I   1           "
  ./Pi1.bat
}
elseif ( $target -eq "target=rp2" )
{
  Write-Host "     B U I L I D I N G    P I   2           "
  ./Pi2.bat
}
elseif ( $target -eq "target=rp3" )
{
  Write-Host "     B U I L I D I N G    P I   3           "
  ./Pi3-64.bat
}
else{
  Write-Host "Unknown platform"
  exit
}

Write-Host "Success"

if ( $exec -eq "exec=true" )
{
  if ( $target -eq "target=rp1" )
  {
    Write-Host "         P I    1 ' S     U A R T 0          "
    Write-Host "= = = = = = = = = = = = = = = = = = = = = = = = "
    Write-host "     (Please note PI 1 Not supported by QEMU)"
    Write-host "     (So you may see some strange behaviour)"
    qemu-system-arm -M raspi2 -kernel output/kernel.img -drive file=SDCard/SDCard.img,if=sd,format=raw -serial stdio
  }
  elseif ( $target -eq "target=rp2" )
  {
    Write-Host "         P I    2 ' S     U A R T 0          "
    Write-Host "= = = = = = = = = = = = = = = = = = = = "
    qemu-system-arm -M raspi2 -kernel output/kernel7.img -drive file=SDCard/SDCard.img,if=sd,format=raw -serial stdio
  }
  elseif ( $target -eq "target=rp3" )
  {
    Write-Host "         P I   3 ' S     U A R T 0          "
    Write-Host "= = = = = = = = = = = = = = = = = = = = "
    qemu-system-aarch64 -M raspi3 -kernel output/kernel8.img -drive file=SDCard/SDCard.img,if=sd,format=raw -serial stdio
  }
}
