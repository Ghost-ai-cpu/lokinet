[CmdletBinding()]
param ()
$web = New-Object System.Net.WebClient
if( -not ( Test-Path $env:APPDATA\.worktipsnet -PathType Container ) )
{
  worktipsnet.exe -g
}


$web.DownloadFile("https://seed.worktipsnet.org/worktipsnet.signed", "$env:APPDATA\.worktipsnet\bootstrap.signed")
