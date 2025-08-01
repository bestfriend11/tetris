#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TetrisPlayerController.generated.h"

class ATetrisPiece;
class ATetrisBoard;

UCLASS()
class TETRISGAME_API ATetrisPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATetrisPlayerController();

	// Set the current active piece
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	void SetCurrentPiece(ATetrisPiece* Piece);

	// Handle score updates
	UFUNCTION(BlueprintImplementableEvent)
	void HandleScoreChanged(int32 NewScore, int32 NewLevel);


	// Input handlers
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveLeft();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveDown();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void RotatePiece();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HardDrop();

	UFUNCTION(BlueprintCallable, Category = "Tetris")
	ATetrisPiece* GetCurrentPiece() const;
	
	UFUNCTION(BlueprintCallable, Category = "Tetris")
	ATetrisBoard* GetGameBoard() const;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:


	// Current active piece
	UPROPERTY()
	ATetrisPiece* CurrentPiece;

	// Reference to the game board
	UPROPERTY()
	ATetrisBoard* GameBoard;

};
