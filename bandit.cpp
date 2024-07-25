
#include <ctime>
#include <iostream>
#include <limits>
#include <stdlib.h>
using namespace std;

enum Symbol {
    A,
    O,
    X
};

void print_menu();
void print_bandit_menu(int bet);
void print_balance(int balance);
void print_profit(int profit);
void print_balance_options();
void print_symbol_screen(int symbol_screen[3][3]);
void print_end_of_round(int adjustment, int bet, int winning_lines);
char intToSymbol(int val);

bool can_place_bet(int balance, int bet);

int calculate_transaction(int bet, int winning_lines);

void play_Bandit(int &balance, int &total_profit);
int play_round(int bet);

int check_win(int symbol_screen[3][3]);
int check_columns(int symbol_screen[3][3]);
int check_rows(int symbol_screen[3][3]);
int check_diagonals(int symbol_screen[3][3]);
int place_bet(int &balance);
int calculate_transaction(int bet, int winning_lines);

int randomize_number();

int get_menu_choice(string choice_text);
int get_deposit();
int get_bet_amount();

int main() {

    srand(time(0));
    int balance = 1000;
    int total_profit = 0;

    cout << "----------- Enarmad Bandit -----------" << endl;

    bool shutdown = false;
    int menu_choice;
    while (!shutdown) {
        print_menu();
        menu_choice = get_menu_choice("Ange menyval: ");

        switch (menu_choice) {
        case 1: /* play */
            if (balance != 0) {
                play_Bandit(balance, total_profit);
            } else {
                cout << "Du har otillräcklig saldo. Lägg in mer pengar." << endl;
            }
            break;
        case 2:
            balance += get_deposit();
            //Hämta insats.
            break;
        case 3: /*print balance*/
            print_balance(balance);
            break;
        case 4: /*print total winnings*/
            print_profit(total_profit);
            break;
        case 0: /* Shutdown */
            shutdown = true;
            break;
        default:
            cout << "Felaktigt menyval, försök igen" << endl;
            break;
        }
    }
    return 0;
}

void play_Bandit(int &balance, int &total_profit){
    
    //take bet

    int bet = get_bet_amount();

    //Lets make this so the player can keep spinning on a chosen bet or change the bet or go back
    //So we need a menu
    int play = 1;
    while(play) {
        print_bandit_menu(bet);
        int choice = get_menu_choice("Val: ");

        switch(choice){
            
            case 1: //Play
            if(bet <= balance){
                balance -= bet;
                int adjustment = play_round(bet);
                if (adjustment > 0){
                    total_profit += (adjustment - bet);
                    balance += adjustment;
                }
            } else {
                cout << "Ditt saldo är för lågt! Lägg in mer pengar." << endl;
            }
            cout << "Saldo: " << balance << endl;
            break;
            case 2: //Change bet
            bet = get_bet_amount();
            break;
            case 3: 
            play = 0;
            default:
            cout << "Felaktigt menyval, försök igen" << endl;
            break;
        }
    
    }

    
}

int play_round(int bet){
    //Draw numbers

    int symbol_screen[3][3];

    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            symbol_screen[row][col] = randomize_number();
        }
    }

    //Display symbol screen
    print_symbol_screen(symbol_screen);

    //Calculate win
    int winning_lines = check_win(symbol_screen);
    //Print end of round information
    
    int adjustment = calculate_transaction(bet, winning_lines);
    print_end_of_round(adjustment, bet, winning_lines);

    cout << endl <<  "Tryck Enter för att fortsätta.";
    // Determine we only have to player press Enter and nothing follows.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    return adjustment;
}

int calculate_transaction(int bet, int winning_lines){
    if(winning_lines){
        if(winning_lines <= 4){
            return ((winning_lines + 1) * bet);
        } else if (winning_lines == 5){
            return (7 * bet);
        } else if (winning_lines == 10){
            return (10 * bet);
        }
    } else {
        return 0;
    }
}

int check_win(int symbol_screen[3][3]){
    int winning_lines = 0;

    winning_lines += check_rows(symbol_screen);
    winning_lines += check_columns(symbol_screen);
    winning_lines += check_diagonals(symbol_screen);

    return winning_lines;
}

int check_columns(int symbol_screen[3][3]){
    int winning_lines = 0;
    for(int col = 0; col < 3; col++){
        if(symbol_screen[0][col] == symbol_screen[1][col] && symbol_screen[1][col] == symbol_screen[2][col]){
            winning_lines++;
        }
    }
    return winning_lines;
}

int check_rows(int symbol_screen[3][3]){
    int winning_lines = 0;
    for(int row = 0; row < 3; row++){
        if(symbol_screen[row][0] == symbol_screen[row][1] && symbol_screen[row][1] == symbol_screen[row][2]){
            winning_lines++;
        }
    }
    return winning_lines;
}

int check_diagonals(int symbol_screen[3][3]){
    int winning_lines = 0;
    if(symbol_screen[0][0] == symbol_screen[1][1] && symbol_screen[1][1] == symbol_screen[2][2]){
        winning_lines++;
    }
    
    if(symbol_screen[0][2] == symbol_screen[1][1] && symbol_screen[1][1] == symbol_screen[2][0]){
        winning_lines++;
    }
    return winning_lines;
}

/// @brief Randomizes from 1 - 36.
/// @return int in range 1 - 36.
int randomize_number() {
    return rand() % 3;
}

/// @brief Checks the cin for unwanted types. If int choice is not int, 
/// the cin will be emptied and the user notified of it's error. Keeps going until the user enters an int.
/// @param choice_text Message to print ahead of the user input.
/// @return int of the choice made.
int get_menu_choice(string choice_text) {
    cout << choice_text;
    int choice;

    // Referens: https://stackoverflow.com/questions/30569789/to-check-type-of-input-in-c
    while (true) {
        cin >> choice;
        if (cin.fail()) {
            cout << "Du matade inte in ett nummer. Försök igen." << endl;
            cin.clear();
            cin.sync();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        } else {
            return choice;
        }
    }
    // Slut referens.
}

/// @brief Prompts the user for an input of an int larger than 0. Notifies the user if the bet amount is invalid.
/// @return Int.
int get_bet_amount() {
    while (true) {
        int number_choice = get_menu_choice("Insats: ");
        if (number_choice > 0) {
            return number_choice;
        } else {
            cout << "Valet stöds inte. Försök igen." << endl;
        }
    }
}


/// @brief Prompts the user to choose a deposit amount. 1 for 100, 2 for 300, 3 for 500.
/// @return The bet amount of 100, 300 or 500.
int get_deposit() {
    while (true) {
        print_balance_options();
        int bet_choice = get_menu_choice("Ange insatsval: ");

        switch (bet_choice) {
        case 1:
            return 100;
        case 2:
            return 300;
        case 3:
            return 500;
        default:
            cout << "Alternativet " << bet_choice << " finns inte. Försök igen." << endl;
            break;
        }
    }
}

/// @brief Get the bet amount from the user. Conditional to check if the user has sufficient balance for the bet.
/// Calculates the new balance based on the bet and prints it.
/// @param balance reference to the user balance.
/// @return The bet placed.
int place_bet(int &balance) {
    int bet = 0;
    while (bet == 0) {
        bet = get_deposit();
        if (!can_place_bet(balance, bet)) {
            cout << "Du har inte tillräckligt saldo för att lägga den insatsen. Försök igen." << endl;
            bet = 0;
        }
    }
    cout << endl
         << "Insats: " << bet << endl;
    balance -= bet;
    print_balance(balance);
    return bet;
}

/// @brief Checks if balance is larger or equal to bet. Determines if bet can be placed.
/// @param balance Player balance.
/// @param bet Chosen bet amount.
/// @return true if bet <= balance, otherwise false.
bool can_place_bet(int balance, int bet) {
    if (bet <= balance) {
        return true;
    }
    return false;
}

// Print-functions

void print_menu() {
    cout << endl;
    cout << "------------- Meny -------------" << endl;
    cout << "[1]...Spela" << endl;
    cout << "[2]...Lägg in pengar" << endl;
    cout << "[3]...Saldo" << endl;
    cout << "[4]...Total vinst" << endl;
    cout << "[0]...Avsluta" << endl;
    cout << "--------------------------------" << endl;
}

void print_bandit_menu(int bet) {
    cout << endl;
    cout << "[1]...Snurra[" << bet << "kr]" << endl;
    cout << "[2]...Ändra insats" << endl;
    cout << "[3]...Huvudmeny" << endl;
}
void print_balance(int balance) {
    cout << "------------ SALDO -------------" << endl
         << endl;
    cout << "Saldo: " << balance << endl;
}
void print_profit(int profit) {
    cout << "--------- TOTAL VINST ----------" << endl
         << endl;
    cout << "Total vinst: " << profit << endl;
}
void print_balance_options() {
    cout << "Hur mycket vill du lägga in?" << endl;
    cout << "[1]...100kr" << endl;
    cout << "[2]...300kr" << endl;
    cout << "[3]...500kr" << endl;
}

void print_bet_choice(){
    cout << "Välj hur mycket du vill satsa:"<< endl;
}

void print_symbol_screen(int symbol_screen[3][3]){

    cout << "---------------" << endl;
    for (int row = 0; row < 3; row++){
        
        for (int col = 0; col < 3; col++){
            cout << "| " << intToSymbol(symbol_screen[row][col]) << " |" ;
        }
        cout << endl << "---------------" << endl;
    }
        // -------------
        // | X | X | X |
        // -------------
        // | A | A | A |
        // -------------
        // | O | O | O |
        // -------------
}

char intToSymbol(int val){
    switch(val){
        case 0: return 'A';
        case 1: return 'O';
        case 2: return 'X';
        default: return '@';
    }
}

void print_end_of_round(int adjustment, int bet, int winning_lines){
    cout << endl;
    if(winning_lines){
        cout << "Du vann " << adjustment << "kr på " << winning_lines << " vinnande linje(r)!" << endl;
    } else {
        cout << "Du förlorade " << bet << "kr" << endl;
    }
}
