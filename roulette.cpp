
#include <ctime>
#include <iostream>
#include <limits>
#include <stdlib.h>
using namespace std;
const int MIN_BET = 100;

enum GameType {
    COLOR,
    NUMBER
};

void print_menu();
void print_balance(int balance);
void print_profit(int profit);
void print_number_options();
void print_color_options();
void print_bet_options();
void print_game_options();
void print_preroll_info(GameType gameType, int bet, int number);
void print_drawing(int rolled_number);

bool can_place_bet(int balance, int bet);

void playRoulette(int &balance, int &total_profit);
int place_bet(int &balance);
int choose_number(GameType gameType);
int randomize_number();

GameType get_game_type();
int get_multi(GameType gameType);
int get_menu_choice(string choice_text);
int get_bet();
int get_number();
int get_color();

int main() {

    srand(time(0));
    int balance = 1000;
    int total_profit = 0;

    cout << "----------- Roulette -----------" << endl;

    bool shutdown = false;
    int menu_choice;
    while (!shutdown) {
        print_menu();
        menu_choice = get_menu_choice("Ange menyval: ");

        switch (menu_choice) {
        case 1: /* play */
            if (can_place_bet(balance, MIN_BET)) {
                playRoulette(balance, total_profit);
            } else {
                cout << "Ditt saldo är mindre än lägsta insatsen. Du har förlorat." << endl;
            }
            break;
        case 2: /*print balance*/
            print_balance(balance);
            break;
        case 3: /*print total winnings*/
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

/// @brief Main roulette logic. Uses 'balance' and 'total_profit' to manipulate the game state.
/// Calls for functions to help handle placing bets, choosing number or color, rolling the game and determine win/loss.
/// @param balance player balance
/// @param total_profit  player total profit
void playRoulette(int &balance, int &total_profit) {
    int bet = place_bet(balance);
    GameType game_type = get_game_type();
    int player_number = choose_number(game_type);

    int multi = get_multi(game_type);

    print_preroll_info(game_type, bet, player_number);

    int rolled_number = randomize_number();
    print_drawing(rolled_number);

    if (game_type == COLOR) {
        // Modulo decides if rolled number is even or odd.
        // Player number for black is 2, so it also gets % 2.
        rolled_number = rolled_number % 2; // 0 is black, 1 is red
        player_number = player_number % 2;
    }

    bool win = false;
    // decide win
    if (rolled_number == player_number) {
        win = true;
    }

    // Win/Loss calculations and prints.
    if (win) {
        int winBalance = bet * multi;
        cout << endl
             << "Grattis, du vann " << winBalance << "kr!!!" << endl;
        balance += winBalance;
        total_profit += winBalance - bet;
    } else {
        cout << endl
             << "Du förlorade." << endl;
    }

    cout << endl;
    print_profit(total_profit);

    cout << "Tryck Enter för att fortsätta." << endl;
    // Determine we only have to player press Enter and nothing follows.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

/// @brief Randomizes from 1 - 36.
/// @return int in range 1 - 36.
int randomize_number() {
    return rand() % 36 + 1;
}

/// @brief Determines if color or number should be chosen.
/// @param gameType Type of value to choose.
/// @return The number chosen by the player.
int choose_number(GameType gameType) {
    int player_number = 0;

    if (gameType == COLOR) {
        player_number = get_color();
    } else {
        player_number = get_number();
    }
    return player_number;
}

/// @brief Returns the multiplier for the game type.
/// @param gameType
/// @return 2 if COLOR, 10 otherwise (NUMBER).
int get_multi(GameType gameType) {
    if (gameType == COLOR) {
        return 2;
    } else {
        return 10;
    }
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

/// @brief Prompts the user for an input of an int in range 1 to 36. Notifies the user of inputs out of range.
/// @return Int in range 1 to 36.
int get_number() {
    print_number_options();
    while (true) {
        int number_choice = get_menu_choice("Nummerval [1 - 36]: ");
        if (number_choice > 0 && number_choice < 37) {
            return number_choice;
        } else {
            cout << "Valet stöds inte. Försök igen." << endl;
        }
    }
}

/// @brief Prompts the user for the choice of color, 1 or 2.
/// @return Int representing the color chosen, 1 is Red, 2 is Black.
int get_color() {
    print_color_options();
    while (true) {
        int color_choice = get_menu_choice("Färgval: ");
        if (color_choice == 1 || color_choice == 2) {
            return color_choice;
        } else {
            cout << "Valet finns inte. Försök igen." << endl;
        }
    }
}

/// @brief Prompts the user to choose a bet amount. 1 for 100, 2 for 300, 3 for 500.
/// @return The bet amount of 100, 300 or 500.
int get_bet() {
    while (true) {
        print_bet_options();
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

/// @brief Prompts the user to choose a game type for the round.
/// @return The game type chosen, 1 for COLOR, 2 for NUMBER.
GameType get_game_type() {
    print_game_options();
    while (true) {
        int choice = get_menu_choice("Ange speltypsval: ");
        if (choice == 1) {
            return COLOR;
        } else if (choice == 2) {
            return NUMBER;
        } else {
            cout << "Valet stöds ej. Försök igen." << endl;
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
        bet = get_bet();
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
    cout << "[2]...Saldo" << endl;
    cout << "[3]...Total vinst" << endl;
    cout << "[0]...Avsluta" << endl;
    cout << "--------------------------------" << endl;
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
void print_bet_options() {
    cout << "Välj insats:" << endl;
    cout << "[1]...100kr" << endl;
    cout << "[2]...300kr" << endl;
    cout << "[3]...500kr" << endl;
}
void print_game_options() {
    cout << "Välj speltyp:" << endl;
    cout << "[1]...Färg" << endl;
    cout << "[2]...Nummer" << endl;
}
void print_number_options() {
    cout << "Välj ett nummer mellan 1 och 36:" << endl;
}
void print_color_options() {
    cout << "Välj färg:" << endl;
    cout << "[1]...Röd" << endl;
    cout << "[2]...Svart" << endl;
}

/// @brief Prints bet information based on inparameters. Prints chosen color if that type of round or the number chosen.
/// @param gameType RED or BLACK.
/// @param bet Amount of bet.
/// @param number Number chosen.
void print_preroll_info(GameType gameType, int bet, int number) {
    cout << endl
         << "Du har lagt " << bet << "kr på ";
    if (gameType == COLOR) {
        if (number == 1) {
            cout << "RÖD." << endl;
        } else {
            cout << "SVART." << endl;
        }
    } else {
        cout << "nummer " << number << "." << endl;
    }
}

/// @brief Prints the roulette roll. Presents infromation about the roll.
/// @param rolled_number The rolled number.
void print_drawing(int rolled_number) {
    cout << endl
         << "Rullningen startar..." << endl;

    cout << "Bollen stannar på... " << rolled_number << "!" << endl;
    cout << rolled_number << " är ett ";
    int color = rolled_number % 2;
    if (color == 1) {
        cout << "RÖTT";
    } else {
        cout << "SVART";
    }
    cout << " nummer!" << endl;
}
