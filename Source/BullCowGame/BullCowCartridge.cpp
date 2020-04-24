// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(HiddenWordsList);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame() 
{
    bGameOver = false;
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len() * 2;

    // Welcome the Player
    PrintLine(TEXT("Welcome to the Bull-Cow Game!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type your guess and hit ENTER to continue.")); // Prompt Player For Guess
    // PrintLine(TEXT("The hidden word is: %s"), *HiddenWord); // Debug Line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press Enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString& PlayerGuess)
{
    if (PlayerGuess == HiddenWord) 
    {
        PrintLine(FString::Printf(TEXT("You guessed correctly. Congratulations!")));
        EndGame();
        return;
    }

    // Lose a life
    --Lives; 

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    PrintLine(TEXT("You have %i lives remaining."), Lives);
    
    if(!IsIsogram(PlayerGuess))
    {
        PrintLine(TEXT("No repeating letters! Try again!"));
        return;
    }

    if (PlayerGuess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The Hidden Word is %i letters long.\nTry again!"), HiddenWord.Len());
    }
    else
    {
        PrintLine(TEXT("You guessed incorrectly. Try again!"));
    }

    //Show the player Bulls and Cows
    FBullCowCount Score = GetBullsCows(PlayerGuess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullsCows(const FString& PlayerGuess) const
{
    FBullCowCount BullCowCount;
    for (int32 i = 0; i < PlayerGuess.Len(); i++)
    {
        if (PlayerGuess[i] == HiddenWord[i])
        {
            BullCowCount.Bulls++;
            continue;
        }
        
        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if (PlayerGuess[i] == HiddenWord[j])
            {
                BullCowCount.Cows++;
                break;
            }
        }
    }
    return BullCowCount;
}