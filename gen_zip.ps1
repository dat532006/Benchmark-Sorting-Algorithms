# Script tạo file ZIP nộp Moodle
# Sử dụng: ./gen_zip.ps1 <MSSV>
# ./gen_zip.ps1 24120037-24120030

param(
    [Parameter(Mandatory=$true)]
    [string]$MSSV
)

# Compile test_gen
g++ -std=c++2b -O2 -o test_gen.exe test_gen.cpp

if ($LASTEXITCODE -ne 0) {
    Write-Host "Compile failed. Kiem tra g++ hoac test_gen.cpp"
    exit 1
}

# Tao cau truc thu muc
New-Item -ItemType Directory -Force -Path "$MSSV/int" | Out-Null
New-Item -ItemType Directory -Force -Path "$MSSV/strlexi" | Out-Null
New-Item -ItemType Directory -Force -Path "$MSSV/strlenlexi" | Out-Null

# Sinh 5 test cho moi bai
$problems = @("int", "strlexi", "strlenlexi")

foreach ($problem in $problems) {
    for ($i = 1; $i -le 5; $i++) {
        $num = "{0:D3}" -f $i
        $outPath = Join-Path (Get-Location) "$MSSV/$problem/test$num.in"
        $lines = & .\test_gen.exe $problem $i
        $content = ($lines -join "`n") + "`n"
        [System.IO.File]::WriteAllText($outPath, $content, (New-Object System.Text.UTF8Encoding $false))
        Write-Host "Generated $MSSV/$problem/test$num.in"
    }
}

# Dong ZIP
$zipName = "$MSSV.zip"

if (Test-Path $zipName) {
    Remove-Item $zipName
}

Compress-Archive -Path $MSSV -DestinationPath $zipName -Force

Write-Host "=== Created $zipName ==="
Get-Item $zipName

# Cleanup folder tam
Remove-Item -Recurse -Force $MSSV