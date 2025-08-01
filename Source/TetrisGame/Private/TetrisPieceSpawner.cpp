#include "TetrisPieceSpawner.h"
#include "TetrisPiece.h"
#include "TetrisBoard.h"
#include "Kismet/GameplayStatics.h"

ATetrisPieceSpawner::ATetrisPieceSpawner()
{
    // Create and setup spawn point component
    SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
    RootComponent = SpawnPoint;

    // Initialize with a random next piece
    NextPieceType = nullptr;
    Board = nullptr;
}

void ATetrisPieceSpawner::SetBoardReference(ATetrisBoard* InBoard)
{
    Board = InBoard;
}

ATetrisPiece* ATetrisPieceSpawner::SpawnNewPiece()
{
    if(PieceTypes.Num() == 0) return nullptr;

    // Get current next piece
    TSubclassOf<ATetrisPiece> PieceToSpawn = NextPieceType ? NextPieceType : GetRandomPieceType();
    
    // Generate new next piece
    NextPieceType = GetRandomPieceType();

    // Spawn the piece
    if(PieceToSpawn && Board)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        if (!Board) return nullptr;

        // Use exact spawn point transform
        FTransform SpawnTransform = SpawnPoint->GetComponentTransform();
        return GetWorld()->SpawnActor<ATetrisPiece>(PieceToSpawn, SpawnTransform, SpawnParams);
    }

    return nullptr;
}

TSubclassOf<ATetrisPiece> ATetrisPieceSpawner::GetNextPieceType() const
{
    return NextPieceType;
}

TSubclassOf<ATetrisPiece> ATetrisPieceSpawner::GetRandomPieceType() const
{
    if(PieceTypes.Num() == 0) return nullptr;
    return PieceTypes[FMath::RandRange(0, PieceTypes.Num() - 1)];
}
