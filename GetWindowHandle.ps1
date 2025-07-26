Get-Process | Where-Object { $_.MainWindowTitle -ne "" } | Select-Object ProcessName, MainWindowTitle, MainWindowHandle | Format-Table -AutoSize
