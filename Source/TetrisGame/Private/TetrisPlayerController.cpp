#include "TetrisPlayerController.h"
#include "TetrisPiece.h"
#include "TetrisBoard.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

ATetrisPlayerController::ATetrisPlayerController()
{
	CurrentPiece = nullptr;
	GameBoard = nullptr;
}

void ATetrisPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Setup input mapping context
	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Tetris")), 0);
	}

	// Find game board in level
	TArray<AActor *> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(this, ATetrisBoard::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		GameBoard = Cast<ATetrisBoard>(FoundActors[0]);
	}
}

void ATetrisPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ATetrisPlayerController::MoveLeft()
{
	if (CurrentPiece && GameBoard)
	{
		FVector MovementDirection = -GameBoard->GetActorForwardVector();
		GameBoard->TryMovePiece(CurrentPiece, MovementDirection);
	}
}

void ATetrisPlayerController::MoveRight()
{
	if (CurrentPiece && GameBoard)
	{
		FVector MovementDirection = GameBoard->GetActorForwardVector();
		GameBoard->TryMovePiece(CurrentPiece, MovementDirection);
	}
}

void ATetrisPlayerController::MoveDown()
{
	if (CurrentPiece && GameBoard)
	{
		FVector MovementDirection = -GameBoard->GetActorUpVector();
		if (!GameBoard->TryMovePiece(CurrentPiece, MovementDirection))
		{
			GameBoard->LockPiece(CurrentPiece);
			// TODO: Spawn new piece
		}
	}
}

void ATetrisPlayerController::RotatePiece()
{
	if (CurrentPiece)
	{
		CurrentPiece->Rotate();
	}
}

void ATetrisPlayerController::HardDrop()
{
	if (CurrentPiece && GameBoard)
	{
		FVector MovementDirection = -GameBoard->GetActorUpVector();
		while (GameBoard->TryMovePiece(CurrentPiece, MovementDirection))
		{
			// Movement successful, continue dropping
		}
		GameBoard->LockPiece(CurrentPiece);
		// TODO: Spawn new piece
	}
}


ATetrisPiece* ATetrisPlayerController::GetCurrentPiece() const
{
	return CurrentPiece;
}

ATetrisBoard* ATetrisPlayerController::GetGameBoard() const
{
	return GameBoard;
}

void ATetrisPlayerController::SetCurrentPiece(ATetrisPiece *Piece)
{
	CurrentPiece = Piece;
}
