
function Do-Something
{
    Copy-Item ..\..\output\adjacency\* .\input
}

function Start-Event($idName)
{
    Register-ObjectEvent -InputObject $watcher -EventName Created -SourceIdentifier $idName -Action { Do-Something } -ErrorAction "Stop"
}

$watchedFolder = "..\..\output\adjacency"
$watcher = New-Object System.IO.FileSystemWatcher
$watcher.Path = $watchedFolder

$watchIds = "File.Created", "File.Changed", "File.Renamed"

foreach ($id in $watchIds)
{
    Try
    {
        Start-Event $id
    }
    Catch
    {
        Write-Host $id "is already running."   
    }
}
