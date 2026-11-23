# Verifie qu'aucune fonctionnalite C++ hors du cours n'a ete introduite.
#   powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1
$ErrorActionPreference = "Stop"

$racine = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)

# Motifs interdits : lambda, std::function, optional, variant, ... et
# allocation brute (exception : l'instance unique du Singleton, comme dans le cours).
$motifs = @(
    '\[\s*[&=]?\s*\]\s*\(',
    'std::function',
    'std::optional',
    'std::variant',
    'std::any',
    'std::string_view',
    'std::filesystem',
    'std::ranges',
    'std::thread',
    'std::mutex',
    'std::async',
    'std::regex',
    'std::chrono',
    'std::format',
    'if\s+constexpr',
    'co_await',
    '\bprintf\b',
    '\bmalloc\b',
    '\bgoto\b',
    'new\s+(?!CatalogueCartes)'
)

$fichiers = @()
$fichiers += Get-ChildItem -Path (Join-Path $racine "src") -Recurse -Include *.cpp, *.h -ErrorAction SilentlyContinue
$fichiers += Get-ChildItem -Path (Join-Path $racine "tests") -Recurse -Include *.cpp, *.h -ErrorAction SilentlyContinue

$total = 0
foreach ($motif in $motifs) {
    $resultats = @($fichiers | Select-String -Pattern $motif)
    if ($resultats.Count -gt 0) {
        $total += $resultats.Count
        Write-Host "MOTIF INTERDIT [$motif]" -ForegroundColor Red
        foreach ($r in $resultats) {
            Write-Host ("  " + $r.Path + ":" + $r.LineNumber + "  " + $r.Line.Trim())
        }
    }
}
if ($total -gt 0) { throw "$total occurrence(s) hors du cadre du cours" }
Write-Host "Aucune fonctionnalite hors cours detectee" -ForegroundColor Green
