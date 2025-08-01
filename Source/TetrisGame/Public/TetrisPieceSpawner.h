#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisPieceSpawner.generated.h"

class ATetrisPiece;
class ATetrisBoard;

UCLASS()
class TETRISGAME_API ATetrisPieceSpawner : public AActor
{
    GENERATED_BODY()

public:
    ATetrisPieceSpawner();

    // Spawn a new random piece
    UFUNCTION(BlueprintCallable, Category = "Tetris")
    ATetrisPiece* SpawnNewPiece();

    // Get the next piece type (for preview)
    UFUNCTION(BlueprintCallable, Category = "Tetris")
    TSubclassOf<ATetrisPiece> GetNextPieceType() const;

    // Set the board reference for proper piece positioning
    UFUNCTION(BlueprintCallable, Category = "Tetris")
    void SetBoardReference(ATetrisBoard* InBoard);

protected:
    // Array of all possible piece types
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tetris")
    TArray<TSubclassOf<ATetrisPiece>> PieceTypes;

    // Next piece to spawn
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tetris")
    TSubclassOf<ATetrisPiece> NextPieceType;

public:
    // Reference to the game board for positioning
    // Visual representation of spawn point
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* SpawnPoint;

private:
    // Select a random piece type
    TSubclassOf<ATetrisPiece> GetRandomPieceType() const;

    UPROPERTY()
    ATetrisBoard* Board;
};
