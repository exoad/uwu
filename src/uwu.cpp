/**
 * @file uwu.cpp
 * @author exoaduwu
 * @brief An uwu/owo game :)
 * @version 1.0w0
 * @date 2022-04-08
 *
 * @copyright Copyright (c) 2022
 *
 * Have fun! >w<
 *
 * Don't change any of the values below :P
 *
 * Also there might be some odd conventions used around here. This
 * is because I combined 2 header files into one :P.
 */

#include <assert.h>
#include <stdio.h>

#include <chrono>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string.h>
#include <thread>
#include <utility>
#include <vector>
#include <future>

#ifdef _WIN32
#ifdef _Y_DEBUGGER 
#include "yttrium_debugger.h"
#include "i.h"
#include "i_helper.h"
#define Y_DEBUGGER_ENABLED
#include "y_curses.hxx"
#endif
#endif

using namespace std;

#define sleep(n) std::this_thread::sleep_for(std::chrono::milliseconds(n))
#define wipeWin() printf("\033[H\033[J");
#define bufferize std::cin.fail(); std::cin.clear();
#define debug(n) std::cout << "DEBUG: " << #n << " (" << &n << ") = " << n << std::endl;

namespace push_conf {
  const bool USE_DEBUG = false;
  const long TLE = 0.7L;
  const int MAX_THREADED = 8;
  const string INPUT_KEY = "k";
};

typedef struct {
  long time_s = 1.0L;
  bool judge = false;
} game_setup;

namespace uwutil {
  array<string, 5> faces = { "UwU", "OwO", ">w<", "Ow<", "XwX" };
  uint8_t BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35,
    CYAN = 36, WHITE = 37, BLACK_BG = 40, RED_BG = 41, GREEN_BG = 42,
    YELLOW_BG = 43, BLUE_BG = 44, MAGENTA_BG = 45, CYAN_BG = 46,
    WHITE_BG = 47;

  string brder(string str) {
    int len = sizeof(str) / sizeof(str[0]);
    string temp = "";
    while (len--) {
      temp += '=';
    }
    return temp + temp;
  }

  /**
   * @brief Ansi BOLD the message
   *
   * @param str the string in question
   * @return string a string with bolded ansi escape codes
   */
  string bold(string str) { return "\033[1m" + str + "\033[0m"; }

  /**
   * @brief Colorize the message with the given integers above for the colors
   *        avaliable for ANSI :3
   *
   * @param str the string in question
   * @param color the color needed
   * @param highlight highlight or no :3
   * @return string The final string!
   */
  string colorize(string str, uint8_t color, bool highlight) {
    string temp = "\033[";
    if (highlight) {
      temp += "1;";
    }
    temp += to_string(color) + "m" + str + "\033[0m";
    return temp;
  }

  const array<char, 2> mouth_aval = { 'w', 'W' };
  /**
   * @brief A char represents the avaliable eye
   * The boolean after represents if that char_eye_piece is able to be used with
   * others.
   *
   */
  const map<char, bool> eyes_aval = { {'O', true},  {'o', true},  {'U', true},  {'u', true},
                          {'X', true}, {'3', false}, {'Q', false}, {'>', true},
                          {'<', true},  {'0', true},  {'~', true},  {'q', false},
                          {'x', false}, {'.', false}, {';', false}, {'*', false},
                          {'^', true},  {'T', false}, {'`', true} };

  set<string> __combos() {
    set<string> combos;
    for (int i = 0; i < eyes_aval.size() * eyes_aval.size(); i++) {
      auto x = eyes_aval.begin();
      advance(x, rand() % eyes_aval.size());
      char eye = x->first;
      char second_eye = '\0';
      string s = "";
      bool eye_can_use = x->second;
      auto t = mouth_aval.begin();
      advance(t, rand() % mouth_aval.size());
      char mouth = *t;
      if (eye_can_use) {
        auto x = eyes_aval.begin();
        advance(x, rand() % eyes_aval.size());
        second_eye = x->first;
        s += eye;
        s += second_eye;
      }
      if (!eye_can_use) {
        s = string(1, eye) + string(1, mouth) + string(1, eye);
      }
      else {
        for (int j = 0; j < eyes_aval.size(); j++) {
          auto y = eyes_aval.begin();
          advance(y, rand() % eyes_aval.size());
          second_eye = y->first;
          bool second_eye_can_use = y->second;
          if (second_eye_can_use) {
            s = string(1, eye) + string(1, mouth) + string(1, second_eye);
            break;
          }
        }
      }
      if (combos.find(s) == combos.end()) {
        combos.insert(s);
        if (push_conf::USE_DEBUG) {
          debug(s)
        }
      }

    }
    return combos;
  }

  string game_set_tostring(game_setup gs) {
    stringstream ss;
    ss << "Time: " << gs.time_s << " sec\nJudge: " << (gs.judge ? "Yes" : "No");
    return ss.str();
  }

  string __get_combos() {
    set<string> combos_s = __combos();
    stringstream ss;
    for (auto x : combos_s) {
      ss << x << " ";
      if (push_conf::USE_DEBUG) debug(x)
    }
    return ss.str();
  }
  const string TOKENIZERS = __get_combos();
  const string OWOIFY_API = "https://nekos.life/api/v2/owoify?text=";
  const string HEADER = "== " + colorize("OwO Welcome!", GREEN, true) + " ==";
  const string BEGIN_TEXT =
    "\nVersion: 1.0\nMade by " + colorize("exoad", MAGENTA, false) + "\n" +
    colorize("!! >w< Best to be used on with ANSI-support !!", RED, true) +
    "\nType as many of the following (in any "
    "case): [owo uwu >w< qwq -w- 3w3 twt ^w^ ...].\nPress " +
    colorize("[ " + push_conf::INPUT_KEY + " ]", BLUE_BG, false) +
    " to "
    "continue!!\n";
  const string MENU =
    colorize("\nType the choice in the '[]' to enter that task!\n", BLUE,
      false) +
    "\n\t" + colorize("[1]", GREEN, false) + " - Play the game! ^w^\n\t" +
    colorize("[2]", GREEN, false) + " - Guide! >wO\n\t" +
    colorize("[3]", GREEN, false) + " - Quit this program TwT\n\n";
  const string AVALIABLE_COMBOS =
    colorize("\nHere are all the avaliable combos:\n", BLUE, false) + "\n" +
    *(&TOKENIZERS) + "\nPress " + colorize("[ " + push_conf::INPUT_KEY + " ]", BLUE_BG, false) +
    " return to the menu!\n";
  const string GAME__PRESET_TEXT = colorize("\nEnter the preset timer countdown (in seconds): ", BLUE, false) + '\n';
  const string GAME__ASK_JUDGE_TEXT = colorize("\nDo you want to enable the judge?", BLUE, false) + "\nThe judge shows you if you typed something wrong or not during run(y/n): \n";
  const vector<string> COUNTDOWN = { "3! Thwee!", "2! Two!", "1! Onye!", "Go!" };
  /**
   * @brief Generates the results
   *
   * @param score Score
   * @param time Time in milliseconds
   * @param correct Correctly typed
   * @param wrong Wrongly typed
   * @param total Total typed (correct + wrong)
   * @param all_types A vector containing all of the typed values
   * @return string
   */
  inline string brd(long score, long time, int correct, int wrong, int total,
    vector<string>& all_types) {
    stringstream ss;
    ss << "== TwT The End ==\nHere are you stats:\nScore: " << score
      << "\nTime: " << time << " ms\nW/C/T:" << wrong << "/" << correct << "/"
      << total << "\n\n";
    return ss.str();
  }

  inline string __null_parser(string str) { return str; }

  using PAGE = tuple<void (*)(), int, string>;

  /// @brief A constructor to form the basis of the board
  struct owo_stats {
    long score = 0.0L;
    int correct = 0.0L;
    int wrong = 0.0L;
    int total = 0.0L;
    vector<string> all_types = {};
    vector<string> only_right = {};
    vector<string> only_wrong = {};
  };

  string __kick_wify(string val) {
    string temp = val;
    cout << temp.size() << endl;
    temp = val.substr(8, val.size() - 2);

    return temp;
  }

  bool __check_agreement(string str) {
    if (str == "y" || str == "Y" || str == "yes" || str == "Yes" || str == "YES" || str == "n" || str == "N" || str == "no" || str == "No" || str == "NO") {
      return true;
    }
    return false;
  }

  bool __check_ny_agreement(string str) {
    if (str == "y" || str == "Y" || str == "yes" || str == "Yes" || str == "YES") {
      return true;
    }
    else if (str == "n" || str == "N" || str == "no" || str == "No" || str == "NO") {
      return false;
    }
    return false;
  }

  void timed_print(float ms, vector<string> str) {
    for (int i = 0; i < str.size(); i++) {
      cout << str[i] << endl;
      this_thread::sleep_for(chrono::milliseconds((int64_t)ms));
    }
  }

  bool find_mouth(char c) {
    for (char c : mouth_aval) {
      if (c == c) {
        return true;
      }
    }
    return false;
  }

  string v_tostring(vector<string> v) {
    stringstream ss;
    ss << "[ ";
    for (auto x : v) {
      ss << x << " ";
    }
    ss << "]";
    return ss.str();
  }

  /**
   * @brief Returns a random face OwO
   *
   * @return string - the face UwU
   */
  inline string randFace() { return faces[rand() % faces.size()]; }

  /// @brief The Window Page constructor so everything isn't a string and
  /// hardcoded in, which ofc would be pretty bad .w.
  class windowo_page {
  public:
    string HEADER, END, CONTENT;

    /**
     * @brief Construct a new windowo page object
     *
     * @param header_page The title
     * @param content The Content uwu
     */
    windowo_page(string header_page, string content) {
      this->HEADER +=
        "== " + colorize(randFace() + " " + header_page, GREEN, true) + " ==";
      this->CONTENT = content;
      this->END = brder(this->HEADER);
      bold(HEADER);
    }

    /**
     * @brief Returns the generated string
     *
     * @return string - the final menu message!
     */
    string returnAs() {
      return HEADER + (CONTENT == "" ? "" : CONTENT + END) + "\n";
    }
  };

  string stats_tostring(owo_stats str) {
    stringstream ss;
    ss << "Score: " << str.score << "\nW/C/T:" << str.wrong << "/" << str.correct << "/" << str.total << "\nAll_RAW: " << v_tostring(str.all_types) << "\nAll_OK: " << v_tostring(str.only_right) << "\nAll_INVALID: " << v_tostring(str.only_wrong);
    return ss.str();
  }

  bool __check_ll64(long x) {
    if (x >= 1 && x <= 9223372036854775807) return true;
    return false;
  }

  /// @brief The UwU class that generates and determines after the time limit runs
  /// out. This class is not used to handle things that happens during the lapse.
  class uwu_combo {
  public:
    /**
     * @brief Construct a new uwu combo object
     * EMPTY
     */
    uwu_combo() {}

    /**
     * @brief Get the OwO object
     *
     * @return vector<owo_stats>
     */
    owo_stats getOwO(vector<string> uwus) {
      owo_stats final_sc;
      for (int i = 0; i < uwus.size(); i++) {
        cout << uwus[i] << " " << validate_combo(uwus[i]) << endl;
        if (validate_combo(uwus[i])) {
          final_sc.score += 1;
          final_sc.only_right.push_back(uwus[i]);
        }
        else {
          final_sc.wrong += 1;
          final_sc.only_wrong.push_back(uwus[i]);
        }
        final_sc.total += 1;
      }
      final_sc.all_types = uwus;
      // calculate the score which is the amount of correct typed over the amount of wrong typed and the total
      final_sc.score = (final_sc.score * 100) / (final_sc.wrong + final_sc.score);
      return final_sc;
    }
    bool validate_combo(string str) {
      if (str.length() > 3) return false;
      char eye = str[0];
      char mouth = str[1];
      char eye2 = str[2];
      if (eyes_aval.find(eye) == eyes_aval.end() || !find_mouth(mouth) || eyes_aval.find(eye2) == eyes_aval.end()) return false;
      if ((!(eyes_aval.find(eye)->second) || (!(eyes_aval.find(eye2)->second))) && (eye != eye2)) return false;
      return true;
    }
  };

  /**
   * @brief Prints a string char by char with a delay so it looks cool :P
   *
   * @param str The char*\/ to print :3
   * @param delay The delay in milliseconds
   */
  inline void fancy_print(string str, long delay = 0) {
    for (char x : str) {
      cout << x;
      fflush(stdout);
      sleep(delay);
    }
  }
};

string upper(string str) {
  stringstream ss;
  for (char x : str) {
    ss << toupper(x);
  }
  return ss.str();
}

using namespace uwutil;

windowo_page START_PG = windowo_page("Welcome!", uwutil::BEGIN_TEXT);
windowo_page MENU_PG = windowo_page("Menu", uwutil::MENU);
windowo_page GUIDE_PG = windowo_page("Guide", uwutil::AVALIABLE_COMBOS);
windowo_page ASK_TIME_PG = windowo_page("Preset Timer", uwutil::GAME__PRESET_TEXT);
windowo_page ASK_JUDGE_PG = windowo_page("Judge Preset", uwutil::GAME__ASK_JUDGE_TEXT);
windowo_page END_PG = windowo_page("Bye!", "");

static void start();
static void guide();
static void quit();

static const map<int, void (*)()> pages = {
  {1, start},
  {2, guide},
  {3, quit},
};

/**
 * @brief Checks if the given value of state exists in the page list
 *
 * @param s The integer representing the state entered
 * @return true
 * @return false
 */
bool pg_contains(int s) { return pages.find(s) != pages.end(); }

void __run_choice(int s) {
  assert(pg_contains(s));
  auto t = pages.find(s);
  void (*fun)() = t->second;
  fun();
}

void __after() {
  wipeWin() fancy_print(MENU_PG.returnAs(), 0.7);
  int choice, goofy = 0;
  cin.fail();
  cin.clear();
  cin >> choice;
  cin.ignore();
  if (pg_contains(choice)) {
    __run_choice(choice);
  }
  while (!pg_contains(choice)) {
    cout << "Invalid choice!\n";
    goofy++;
    if (goofy >= 5) {
      __after();
      return;
    }
    cin >> choice;
    if (pg_contains(choice)) {
      break;
    }
    cin.ignore();
  }
  __run_choice(choice);
  fflush(stdout);
}



void stopwatch(long seconds) {
  long start = time(0);
  long end = start + seconds;
  while (time(0) < end) {
    sleep(1);
  }
  cout << "End!\nPress any key to continue: " << endl;
  return;
}

future<void> stopwatch_t;
uwutil::uwu_combo ONLY_STATIC;

vector<string> mass_collect(long judge) {
  bufferize
    vector<string> uwus;
  while (true) {
    auto status = stopwatch_t.wait_for(0ms);
    if (status == future_status::ready) {
      stopwatch_t.get();
      break;
    }
    string str; cin >> str;
    uwus.push_back(str);
    if (judge) {
      cout << (ONLY_STATIC.validate_combo(str) ? colorize("OK : ", GREEN, true) : colorize("INVALID : ", RED, true)) << " " << str << endl;
    }
  }
  cin.ignore();
  cin.clear();
  cin.fail();
  fflush(stdout);
  return uwus;
}

void __bomb(game_setup tiy) {
  wipeWin() bufferize
    uwutil::timed_print(1000, uwutil::COUNTDOWN);
  stopwatch_t = async(launch::async, stopwatch, tiy.time_s);
  vector<string> uwus = mass_collect(tiy.judge);
  fflush(stdout);
  wipeWin()
    cout << "Results: " << endl;
  cout << stats_tostring(ONLY_STATIC.getOwO(uwus)) << endl;
  fflush(stdout);
  bufferize
    exit(0);
}

/// @brief Start the game, or the main menu
void start() {
  game_setup newgame;
  wipeWin()
    bufferize
    long choice = 0.0L;
  cout << ASK_TIME_PG.returnAs();
  cin >> choice;
  if (uwutil::__check_ll64(choice)) {
    newgame.time_s = choice;
  }
  while (!uwutil::__check_ll64(choice)) {
    cout << "Invalid choice!\n";
    cin >> choice;
    if (uwutil::__check_ll64(choice)) {
      newgame.time_s = choice;
      break;
    }
  }

  wipeWin()
    bufferize

    string c;
  cout << ASK_JUDGE_PG.returnAs();
  cin >> c;
  if (uwutil::__check_agreement(c)) {
    newgame.judge = uwutil::__check_ny_agreement(c);
  }
  while (!uwutil::__check_agreement(c)) {
    cout << "Invalid! (y/n)\n";
    cin >> c;
    if (uwutil::__check_agreement(c)) {
      newgame.judge = uwutil::__check_ny_agreement(c);
      break;
    }
  }
  __bomb(newgame);
}

/// @brief Quit phrase
void quit() {
  cout << END_PG.returnAs();
  exit(0);
}
void guide() {
  wipeWin()

    cout << GUIDE_PG.returnAs();
  cin.ignore();
  __after();
}

/// @brief Kicks the program to boot :flushed:
void __kick() {
  fancy_print(START_PG.returnAs(), 1);
  int goofy = 0;
  string c;cin >> c;
  if (upper(c) == push_conf::INPUT_KEY || c == push_conf::INPUT_KEY) {
    __after();
  }
  while (upper(c) != push_conf::INPUT_KEY || c != push_conf::INPUT_KEY) {
    cout << "Type: " << push_conf::INPUT_KEY << endl;
    cin >> c;
    if (upper(c) == push_conf::INPUT_KEY || c == push_conf::INPUT_KEY) {
      break;
    }
    goofy++;
    if (goofy >= 5) {
      __kick();
      return;
    }
  }
  cin.fail();
  cin.clear();
  __after();
}

void goofy_debug() {
  uwutil::uwu_combo c;
  while (true) {
    string str;
    cin >> str;
    cout << (c.validate_combo(str) ? "Valid" : "Invalid") << endl;
  }
}

void __main() {
  __kick();
}

/**
 * @brief The main function :3
 *
 * @return int - State ofc
 */
int main(int argc, char** argv) { __main(); }
