# rsgain Chocolatey Package
When the user installs the package, the contents of `config/choco/tools` will be copied to `%ChocolateyInstall%/lib/rsgain/tools`. Any `*.exe` files in `config/choco/tools` will then automatically have a shim created at `%ChocolateyInstall%/bin`. As Chocolatey adds `%ChocolateyInstall%/bin`

# Build Process
## Modify the Package
1. Modify the metadata defined in `config/choco/rsgain.nuspec` (i.e.: `package.metadata.version`)
2. Place the contents of the built `rsgain-<VERSION>-win64.zip` into `config/choco/tools`

## Build the Package
```powershell
cd config/choco
choco pack
```

## Test the Package
```powershell
choco install rsgain --force --debug --verbose --source .
(Get-Command rsgain).Source
rsgain --version
```

## Push the Package
```powershell
choco push rsgain.<VERSION>.nupkg --source https://push.chocolatey.org/ --api-key <APIKEY>
```
