#include "TetrisPiece.h"
#include "Components/StaticMeshComponent.h"

ATetrisPiece::ATetrisPiece()
{
    PrimaryActorTick.bCanEverTick = true;
    BlockSize = 100.f;
}

bool ATetrisPiece::Move(FVector Direction)
{
    FVector Movement = Direction * BlockSize;
    AddActorWorldOffset(Movement);
    return true;
}

void ATetrisPiece::Rotate()
{
    FRotator Rotation(0, 90, 0);
    AddActorWorldRotation(Rotation);
}
