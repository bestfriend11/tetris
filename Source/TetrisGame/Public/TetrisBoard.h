#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TetrisBoard.generated.h"

class ATetrisPiece;

/*
 * Coordinate system:
 * - X axis: Left/Right movement (positive = right)
 * - Y axis: In/Out of board (positive = into screen)
 * - Z axis: Up/Down movement (positive = up)
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoardInitializedSignature, int32, Width, int32, Height);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPieceLockedSignature, ATetrisPiece*, LockedPiece, FVector, PieceLocation, FRotator, PieceRotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewPieceSpawnedSignature, ATetrisPiece*, NewPiece);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLinesClearedSignature, int32, LinesCleared, int32, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPieceMovementFailedSignature, FVector, AttemptedPosition);

UCLASS()
class TETRISGAME_API ATetrisBoard : public AActor
{
	GENERATED_BODY()

public:
	ATetrisBoard();

	// Initialize the board with default dimensions
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void Initialize();

	// Check if position is valid for current piece
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	bool IsValidPosition(ATetrisPiece* Piece, FVector Offset = FVector::ZeroVector) const;

	// Lock a piece in place on the board
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void LockPiece(ATetrisPiece* Piece);

	void CheckForCompletedLines();
	// Check for completed lines and clear them
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	int32 ClearLines();

	// Attempt to move piece with validation
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	bool TryMovePiece(ATetrisPiece* Piece, FVector Direction);

public:
	// Spawn a new piece on the board
	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void SpawnNewPiece();

	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void HandlePieceLocked();

	/** Called when board is initialized */
	UPROPERTY(BlueprintAssignable, Category = "Tetris Events")
	FOnBoardInitializedSignature OnBoardInitialized;

	/** Called when a piece is locked in place */
	UPROPERTY(BlueprintAssignable, Category = "Tetris Events")
	FOnPieceLockedSignature OnPieceLocked;

	/** Called when a new piece is spawned */
	UPROPERTY(BlueprintAssignable, Category = "Tetris Events")
	FOnNewPieceSpawnedSignature OnNewPieceSpawned;

	/** Called when one or more lines are cleared */
	UPROPERTY(BlueprintAssignable, Category = "Tetris Events")
	FOnLinesClearedSignature OnLinesCleared;

	/** Called when game over condition is reached */
	UPROPERTY(BlueprintAssignable, Category = "Tetris Events")
	FOnGameOverSignature OnGameOver;

    /** Called when piece movement fails (hits bottom or side) */
    UPROPERTY(BlueprintAssignable, Category = "Tetris Events")
    FOnPieceMovementFailedSignature OnPieceMovementFailed;

    /** Updates boundary positions - call from construction script */
    UFUNCTION(BlueprintCallable, Category = "Tetris Board")
    void UpdateBoundaries();

	UFUNCTION(BlueprintCallable, Category = "Tetris Board")
	void DrawDebugGrid();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tetris Board")
	int32 CurrentScore = 0;

	// Size of each cell in world units
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board")
	float CellSize = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	int32 Width;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tetris Board")
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board")
	ATetrisPiece* CurrentPiece = nullptr;

protected:
	// Board dimensions
	FTimerHandle DropTimerHandle;
	float DropInterval = 1.0f;
	float FastDropInterval = 0.05f;
	float CurrentDropInterval;

	// Visual representation of board bounds
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* BoardBounds;

	// Boundary visualization components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* LeftBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RightBoundary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TopBoundary;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* BottomBoundary;

    // Spawner class to use
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tetris Board")
    TSubclassOf<class ATetrisPieceSpawner> SpawnerClass;

    // Tetris piece spawner instance (owned by board)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tetris Board")
    TObjectPtr<class ATetrisPieceSpawner> Spawner;

    UPROPERTY()
    bool bIsInitialized = false;

	// 2D array representing occupied board cells
	TArray<TArray<bool>> Grid;
};
