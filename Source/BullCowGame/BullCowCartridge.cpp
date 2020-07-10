// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h" //header file
#include "HiddenWordList.h"
//#include "Math/UnrealMathUtility.h"
void UBullCowCartridge::BeginPlay() // When the game starts, the stuff that happens when the game start
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);
    // PrintLine (TEXT("MOOOOOOO"));// will show up on the board for the player
    // PrintLine (TEXT("Dont look behind you theres a giant Cow he's very sensitive"));
    // PrintLine (TEXT("SO NO STARING, PLZ!!"));
    // PrintLine (TEXT("WELCOME PLAYER TO THE WORLD OF MOOOOOOOOOOOOO"));
    // PrintLine(TEXT("Enter some input"));
    // PrintLine(TEXT("hit tab to immediately  start typing in the world"));
    // PrintLine(TEXT(" then finally hit enter, youll notice some magic"));
    // FString hiddenwordLength = boost::lexical_cast<string>(hiddenWord.length());

    //PrintLine(TEXT(""),FMath::RandRange(0,10));

    //initlize words
    GetValidWords(Words);
    SetUpGame();
    // PrintLine(TEXT("The number of possible words is %i"),Words.Num());
    
    // PrintLine(TEXT("The number of valid words is %i"),GetValidWords(Words).Num());


    //wrapping the hidden word
    //PrintLine(TEXT("The HiddenWord is: %s. \n It is %i characters long"), *hiddenWord, hiddenWord.Len());
    //PrintLine(TEXT("The HiddenWord is: %s."), *hiddenWord);
    //PrintLine(FString::Printf(TEXT("The HiddenWord is: %s"), *hiddenWord));
}//end BeginPlay

// When the player hits enter, whatever code we put within this function will execute
//parameters input is what the user enters in the terminal of the game
void UBullCowCartridge::OnInput(const FString& Input) {
  //for strings we use Fstring instead, for ints we use int32
    //encoding - > in general, you should be using the TEXT*( macro when setting string varaible literals. 
    //if you do not specify the TEXT() macros, your literal will be encodd using ANSO, which is highly limeted
    //in what characters it supports. ANY ANSI literals being passed into FString need to undergo a conversin to TCHAR(native unicode encoding),
    //so it is more efficent to use TEXT()
    //Strings need to be wrapped with the TEXT Macro, so that Unreal can succesffuly encode it

    //if the game is over then do clearScreen() and setupGame() the game
    //else checking Player Guess


    //PrintLine(Input); print users input on screen
        //if the game is over , clear screen and setup the game

        if(bGameOver){
            ClearScreen();
            SetUpGame();
        }//end if statement
        else{//checking player guess
            processGuess(Input);
        }
}// UBullCowCartridge

//the purpose of this function is to intialize the number of lives and hidden word for the game.
void UBullCowCartridge::SetUpGame(){
    //start of the game
    //game code
    //welcome the player
    PrintLine (TEXT("WELCOME PLAYER TO THE WORLD OF MOOOOOO"));
    //hidden word equals all the words, from 0 to the entire continer minus one for no overflow
    hiddenWord = Isograms[FMath::RandRange(0,Isograms.Num() -1)];//should return the first word in the TArray
    //PrintLine(TEXT("ValidWords - 1 is: i%"), Isograms.Num() - 1);
    //set lives
    lives = 5;
      bGameOver = false;
     //prompt the user for a guess
    PrintLine(TEXT("you have %i lives!"), lives);
   // PrintLine(TEXT("Guess the %i letter word!"), ); //returns a 32int word
    PrintLine(TEXT("The hidden word is %s"),*hiddenWord);//debug
    PrintLine(TEXT("Enter your guess, \nand hit enter"));
    IsIsogram(hiddenWord);

}//end of class
//end the game
void UBullCowCartridge:: endGame(){
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}
//check if it is an isogram
void UBullCowCartridge:: processGuess(const FString& Guess){
            int32 length = hiddenWord.Len();//returns an integer length of the string


             if(!IsIsogram(Guess)){
                PrintLine(TEXT("No repeating letters "));
                return;
            }
             if(Guess == hiddenWord){
                PrintLine(TEXT("That was the correct word you win!"));
                endGame();//end game
                return;
            }
            //if the length of the input of the user is not the same length of the hidden word print out message
             if(length!= Guess.Len()){
                PrintLine(TEXT("you have guessed incorrectly, \n the hidden word is %i characters long, \n you have lost a life!"), hiddenWord.Len());
                PrintLine(TEXT("# of lives left:  %i "), lives);
            }

            PrintLine(TEXT("You have Lost a life"));
            lives--;
            //shows the player Bulls and Cows
            
            
           FBullCowCount Score =  GetBullCows(Guess);
            PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
            PrintLine(TEXT("Guess again, you have %i lives left"), lives);

            //check if the lives is less than 0
            if(lives <= 0){
                ClearScreen();
                PrintLine(TEXT("You have no lives left!"));
                PrintLine(TEXT("The Hidden word was: %s"), *hiddenWord);
                endGame();
                return;
            }

            
}
//return true if it is an isogram
bool UBullCowCartridge::IsIsogram(const FString& word) const{

    // int32 index = 0;
    // int32 Comparison = index + 1; 

    for(int32 Index = 0; Index < word.Len(); Index++){
        for(int32 Comparison = Index+1; Comparison < word.Len(); Comparison++){
            if(word[Index] == word[Comparison]){
                    return false;
                    break;
            }
        }
    }
        //else if none of the chracters are equal to each other than it is an isogram
        return true;
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const{
    TArray<FString> ValidWords;

    for (FString Word: WordList){

        if(Word.Len() >= 4 && Word.Len() <= 8){

            if(IsIsogram(Word)){
                ValidWords.Emplace(Word);//creates a new valid word array, and adds words that are greater than 4 but less than 8 to the nexw array
            }
            //PrintLine(TEXT("these are the words in the array %s"), *Words[i]);//words need to be dereferenced
        }
    }
    return ValidWords;
}
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const{
    FBullCowCount Count;

    //for every index Guess is same as index, Hideen BullCount ++
    //if not a bull was it a cow if yes cowCount++
    for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++){
        if(Guess[GuessIndex] == hiddenWord[GuessIndex]){
            Count.Bulls++;
            continue;
        }
        for(int32 HiddenIndex = 0; HiddenIndex < hiddenWord.Len(); HiddenIndex++){
            if (Guess[GuessIndex] == hiddenWord[HiddenIndex]){
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}


    // for(int32 i = 0; i< ValidWords.Num(); i++){
    //     PrintLine(TEXT("%s. "), *ValidWords[i]);
    // }