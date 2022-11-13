using namespace System.IO
using namespace System.Collections

$ErrorActionPreference = "Stop"

class DependencyFile {
    [string] $Location
    [string] $Destination
    
    DependencyFile([string] $Location) {
        $this.Location = $Location
    }

    DependencyFile([hashtable] $Data) {
        $this.Location = $Data["Location"]
        $this.Destination = $Data["Destination"]
    }
}

class Dependency {
    [string] $Name
    [uri] $Repository

    [Generic.Dictionary[string, DependencyFile]] $Paths = @{
        Lib = "lib/";
        Include = @{Location = "include/"; Destination = "include/$($this.Name)/"};
        Bin = $null;
    }

    Dependency([string] $Name, [string] $Repository, [hashtable] $Paths = $this.Paths) {
        $this.Name = $Name
        $this.Repository = $Repository
        $this.Paths = $Paths
    }

    Dependency([hashtable] $Data) {
        $this.Name = $Data["Name"]
        $this.Repository = $Data["Repository"]

        if ($Data.ContainsKey("Paths")) {
            foreach ($path in $Data["Paths"]) {
                if ($this.Paths.ContainsKey($path.Name)) {
                    $this.Paths[$path.Name] = $path.Value
                } else {
                    $this.Paths.Add($path.Name, $path.Value)
                }
            }
        }
    }

    [void] Install([DirectoryInfo] $InstallLocation) {

        Push-Location $InstallLocation

        Invoke-WebRequest -Uri $this.Repository -OutFile "$($this.Name).zip"

        foreach ($path in $this.Paths.GetEnumerator()) {
            if ($path.Name -eq "Include") {}
        }

        Pop-Location $InstallLocation
    }
}

[Dependency[]] $Dependencies = @(
    @{
        Name = "libarchive";
        Repository = "https://www.libarchive.org/downloads/libarchive-v3.5.0-win64.zip";
        Paths = @{
            Bin = "bin/"
        }
    };
    @{
        Name = "libarchive";
        Repository = "https://www.libarchive.org/downloads/libarchive-v3.5.0-win64.zip";
        Paths = @{
            Bin = "bin/"
        }
    };
)

$Dependencies | ForEach-Object {

}