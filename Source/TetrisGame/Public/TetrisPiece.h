#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisPiece.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPieceLocked);

UCLASS(Blueprintable)
class TETRISGAME_API ATetrisPiece : public AActor
{
    GENERATED_BODY()

public:
    ATetrisPiece();

    // Move the piece in specified direction
    // Returns true if movement was successful
    UFUNCTION(BlueprintCallable, Category = "Tetris Piece")
    bool Move(FVector Direction);

    // Rotate the piece 90 degrees clockwise
    UFUNCTION(BlueprintCallable, Category = "Tetris Piece")
    void Rotate();

    // Event triggered when piece is locked (hits floor or another piece)
    UPROPERTY(BlueprintAssignable, Category = "Tetris Piece")
    FOnPieceLocked OnPieceLocked;

    // Array of block components that make up this piece
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tetris Piece")
    TArray<class UStaticMeshComponent*> Blocks;

    // Size of each block in the piece
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Piece")
    float BlockSize;
};
