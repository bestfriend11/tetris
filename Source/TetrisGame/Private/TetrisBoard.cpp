#include "TetrisBoard.h"
#include "TetrisPiece.h"
#include "TetrisPieceSpawner.h"
#include "Kismet/GameplayStatics.h"

#pragma optimize("", off)

ATetrisBoard::ATetrisBoard()
{
    // Create and setup board bounds component
    BoardBounds = CreateDefaultSubobject<USceneComponent>(TEXT("BoardBounds"));
    RootComponent = BoardBounds;

    // Create boundary components
    LeftBoundary = CreateDefaultSubobject<USceneComponent>(TEXT("LeftBoundary"));
    RightBoundary = CreateDefaultSubobject<USceneComponent>(TEXT("RightBoundary"));
    TopBoundary = CreateDefaultSubobject<USceneComponent>(TEXT("TopBoundary"));
    BottomBoundary = CreateDefaultSubobject<USceneComponent>(TEXT("BottomBoundary"));

    // Attach boundaries to board bounds
    LeftBoundary->SetupAttachment(BoardBounds);
    RightBoundary->SetupAttachment(BoardBounds);
    TopBoundary->SetupAttachment(BoardBounds);
    BottomBoundary->SetupAttachment(BoardBounds);

    // Default dimensions
    Width = 10;
    Height = 20;
    CellSize = 100.f;
    CurrentScore = 0;
    bIsInitialized = false;
    Spawner = nullptr;
}

void ATetrisBoard::UpdateBoundaries()
{
    // Calculate full dimensions with (0,0) at top-left
    float FullWidth = Width * CellSize;
    float FullHeight = Height * CellSize;

    if (LeftBoundary)
    {
        LeftBoundary->SetRelativeLocation(FVector(-CellSize/2, 0, FullHeight + CellSize/2));
    }
    if (RightBoundary)
    {
        RightBoundary->SetRelativeLocation(FVector(FullWidth + CellSize/2, 0, FullHeight + CellSize/2));
    }
    if (TopBoundary)
    {
        TopBoundary->SetRelativeLocation(FVector(0, 0, FullHeight + CellSize/2));
    }
    if (BottomBoundary)
    {
        BottomBoundary->SetRelativeLocation(FVector(0, 0, -CellSize/2));
    }
}

void ATetrisBoard::DrawDebugGrid()
{
    if (!GetWorld()) return;
    
    const float Duration = 0.0f; // Persistent debug drawing
    const float Thickness = 2.0f;
    const FColor LineColor = FColor::Green;
    const FColor TextColor = FColor::White;
    
    // Draw vertical grid lines (top to bottom)
    for (int32 x = 0; x <= Width; x++)
    {
        FVector Start = FVector(x * CellSize, 0, Height * CellSize);
        FVector End = FVector(x * CellSize, 0, 0);
        DrawDebugLine(GetWorld(), Start, End, LineColor, false, Duration, 0, Thickness);
        
        // Draw x index at top
        FVector TextLoc = FVector(x * CellSize, 0, Height * CellSize + 50);
        DrawDebugString(GetWorld(), TextLoc, FString::Printf(TEXT("%d"), x), nullptr, TextColor, Duration);
    }
    
    // Draw horizontal grid lines (left to right)
    for (int32 y = 0; y <= Height; y++)
    {
        FVector Start = FVector(0, 0, Height * CellSize - y * CellSize);
        FVector End = FVector(Width * CellSize, 0, Height * CellSize - y * CellSize);
        DrawDebugLine(GetWorld(), Start, End, LineColor, false, Duration, 0, Thickness);
        
        // Draw y index at left
        FVector TextLoc = FVector(-50, 0, Height * CellSize - y * CellSize);
        DrawDebugString(GetWorld(), TextLoc, FString::Printf(TEXT("%d"), y), nullptr, TextColor, Duration);
    }
}

void ATetrisBoard::Initialize()
{
    if (Width <= 0 || Height <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("TetrisBoard::Initialize - Invalid dimensions: %dx%d"), Width, Height);
        return;
    }
    bIsInitialized = true;

    // Draw debug grid
    DrawDebugGrid();
    
    // Initialize grid
    Grid.SetNum(Height);
    for(int32 y = 0; y < Height; y++)
    {
        Grid[y].SetNum(Width);
        for(int32 x = 0; x < Width; x++)
        {
            Grid[y][x] = false;
        }
    }

    UpdateBoundaries();

    // Broadcast initialization event
    OnBoardInitialized.Broadcast(Width, Height);

    // Create spawner from specified class if none exists
    if (SpawnerClass && !Spawner)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        Spawner = GetWorld()->SpawnActor<ATetrisPieceSpawner>(SpawnerClass, SpawnParams);
        if (Spawner)
        {
            // Position spawner at top center (aligned with new coordinate system)
            FVector SpawnLocation = FVector(
                Width * CellSize / 2,  // Center horizontally
                0,
                Height * CellSize + CellSize/2  // Above top boundary
            );
            Spawner->SetActorLocation(SpawnLocation);
            Spawner->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            Spawner->SetBoardReference(this);
        }
    }
}

bool ATetrisBoard::IsValidPosition(ATetrisPiece* Piece, FVector Offset) const
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("TetrisBoard::IsValidPosition - Board not initialized"));
        return false;
    }

    if (!Piece)
    {
        UE_LOG(LogTemp, Warning, TEXT("TetrisBoard::IsValidPosition - Null piece"));
        OnPieceMovementFailed.Broadcast(Offset);
        return false;
    }

    bool bIsValid = true;

    if(!Piece) 
    {
        OnPieceMovementFailed.Broadcast(Offset);
        return false;
    }

    for(auto Block : Piece->Blocks)
    {
        if(!Block) continue;

        FVector BlockLocation = Block->GetComponentLocation() + Offset;
        FVector2D GridPos = FVector2D(
            FMath::RoundToInt(BlockLocation.X / 100.f),
            FMath::RoundToInt(BlockLocation.Y / 100.f)
        );

        // Check bounds
        if(GridPos.X < 0 || GridPos.X >= Width || GridPos.Y < 0)
        {
            OnPieceMovementFailed.Broadcast(BlockLocation + Offset);
            bIsValid = false;
            break;
        }

        // Check if already occupied (only check above visible board)
        if(GridPos.Y < Height && Grid[GridPos.Y][GridPos.X])
        {
            OnPieceMovementFailed.Broadcast(BlockLocation + Offset);
            bIsValid = false;
            break;
        }
    }
    return bIsValid;
}

void ATetrisBoard::LockPiece(ATetrisPiece* Piece)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("TetrisBoard::LockPiece - Board not initialized"));
        return;
    }

    if (!Piece)
    {
        UE_LOG(LogTemp, Warning, TEXT("TetrisBoard::LockPiece - Null piece"));
        return;
    }

    for (auto Block : Piece->Blocks)
    {
        if (!Block) continue;

        FVector BlockLocation = Block->GetComponentLocation();
        FVector2D GridPos = FVector2D(
            FMath::RoundToInt(BlockLocation.X / 100.f),
            FMath::RoundToInt(BlockLocation.Y / 100.f)
        );

        // Check if the grid position is within the bounds of the visible board
        if (GridPos.X >= 0 && GridPos.X < Width && GridPos.Y >= 0)
        {
            // Lock the block in place by setting the corresponding grid position to true
            if (GridPos.Y < Height) // Only lock within the visible board height
            {
                Grid[GridPos.Y][GridPos.X] = true;
            }
        }
    }

    // Broadcast piece locked event with position and rotation
    if(Piece)
    {
        OnPieceLocked.Broadcast(Piece, Piece->GetActorLocation(), Piece->GetActorRotation());
    }

    // After locking the piece, check for completed lines
    CheckForCompletedLines();
}

void ATetrisBoard::CheckForCompletedLines()
{
    for (int Y = Height - 1; Y >= 0; --Y)
    {
        bool LineCompleted = true;

        for (int X = 0; X < Width; ++X)
        {
            if (!Grid[Y][X])
            {
                LineCompleted = false;
                break;
            }
        }

        if (LineCompleted)
        {
            // Remove the completed line
            for (int X = 0; X < Width; ++X)
            {
                Grid[Y][X] = false;
            }

            // Shift all lines above the completed line down by one
            for (int CurrentY = Y; CurrentY < Height - 1; ++CurrentY)
            {
                for (int X = 0; X < Width; ++X)
                {
                    Grid[CurrentY][X] = Grid[CurrentY + 1][X];
                }
            }

            // Optionally, update the score here
            // UpdateScore();
        }
    }
}


int32 ATetrisBoard::ClearLines()
{
    int32 LinesCleared = 0;

    for(int32 y = Height - 1; y >= 0; y--)
    {
        bool bLineComplete = true;
        for(int32 x = 0; x < Width; x++)
        {
            if(!Grid[y][x])
            {
                bLineComplete = false;
                break;
            }
        }

        if(bLineComplete)
        {
            // Remove the line
            Grid.RemoveAt(y);
            // Add new empty line at top
            Grid.Insert(TArray<bool>(), 0);
            Grid[0].SetNum(Width);
            
            LinesCleared++;
            y++; // Re-check current position as lines have shifted down
        }
    }

    // Update score and broadcast lines cleared event
    if(LinesCleared > 0)
    {
        // Simple scoring - more points for more lines cleared at once
        CurrentScore += LinesCleared * LinesCleared * 100;
        OnLinesCleared.Broadcast(LinesCleared, CurrentScore);
    }

    return LinesCleared;
}

bool ATetrisBoard::TryMovePiece(ATetrisPiece* Piece, FVector Direction)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("TetrisBoard::TryMovePiece - Board not initialized"));
        return false;
    }

    if (!Piece)
    {
        UE_LOG(LogTemp, Warning, TEXT("TetrisBoard::TryMovePiece - Null piece"));
        return false;
    }

    if (!IsValidPosition(Piece, Direction * Piece->BlockSize))
    {
        return false;
    }

    bool bMoved = Piece->Move(Direction);
    if (bMoved)
    {
        return true;
    }
    return false;
}

void ATetrisBoard::SpawnNewPiece()
{
    // Clear previous piece's event binding
    if(CurrentPiece)
    {
        CurrentPiece->OnPieceLocked.RemoveAll(this);
    }

    if (!Spawner)
    {
        UE_LOG(LogTemp, Error, TEXT("TetrisBoard::SpawnNewPiece - No spawner available"));
        OnGameOver.Broadcast();
        return;
    }

    // Spawn new piece using board's spawner
    CurrentPiece = Spawner->SpawnNewPiece();
    
    // Check for game over (piece couldn't spawn in valid position)
    if(!CurrentPiece || !IsValidPosition(CurrentPiece))
    {
        OnGameOver.Broadcast();
        return;
    }

    // Broadcast new piece spawned event
    OnNewPieceSpawned.Broadcast(CurrentPiece);

    // Bind to the new piece's locked event
    CurrentPiece->OnPieceLocked.AddDynamic(this, &ATetrisBoard::HandlePieceLocked);
}

void ATetrisBoard::HandlePieceLocked()
{
    if(!CurrentPiece) return;
    
    // Lock the current piece
    LockPiece(CurrentPiece);
    
    // Check for completed lines
    CheckForCompletedLines();
    
    // Spawn a new piece
    SpawnNewPiece();
}

#pragma optimize("", on)