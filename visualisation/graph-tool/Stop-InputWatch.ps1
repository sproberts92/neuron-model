function Stop-Event($idName)
{
    Unregister-Event $idName -ErrorAction "Stop"
}

$watchIds = "File.Created", "File.Changed", "File.Renamed"

foreach ($id in $watchIds)
{
    Try
    {
        Stop-Event $id
        Write-Host $id "stopped."
    }
    Catch
    {
        Write-Host $id "is not currently running."   
    }
}