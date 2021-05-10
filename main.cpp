#include <iostream>

#include "include/EarleyParser.h"


int main() {
  std::cout << "Enter your grammar, in such order(if your grammar contains "
               "epsilon rules please type it as `EPS`)\n"
               "1) A word that you want to check\n"
               "2) Number of Nonterminals and the list of Nonterminals on the next line\n"
               "3) Number of Alphabet symbols and symbols themselves\n"
               "4) Number of rules and Rules for grammar\n"
               "5) Starting symbol\n\n";

  std::string word;
  std::cin >> word;

  int32_t n = 0;
  std::cin >> n;
  std::list<std::string> nonterminals;
  for (int32_t i = 0; i < n; ++i) {
    std::string nonterminal;
    std::cin >> nonterminal;
    nonterminals.push_back(nonterminal);
  }

  std::cin >> n;
  std::list<std::string> alphabet;
  for (int32_t i = 0; i < n; ++i) {
    std::string alphabet_symbol;
    std::cin >> alphabet_symbol;
    alphabet.push_back(alphabet_symbol);
  }


  std::cin >> n;
  std::list<GrammarRule> rules_list;
  for (int32_t i = 0; i < n; ++i) {
    std::string rule;
    std::cin >> rule;
    rules_list.emplace_back(rule);
  }

  std::string starting_symbol;
  std::cin >> starting_symbol;


  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);
  EarleyParser earley_parser(grammar);


  std::cout <<
           (earley_parser.Parse(word) ? "Word belongs to grammar language" :
                                                 "Word doesn't belong to grammar language");
  return 0;
}
