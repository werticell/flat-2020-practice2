#include "GrammarRule.h"


class Grammar {
 public:
  std::list<std::string> nonterminals_;
  std::list<std::string> alphabet_;
  std::list<GrammarRule> rules_list_;
  std::string starting_symbol_;

  Grammar(const std::list<std::string>& nonterminals,
          const std::list<std::string>& alphabet,
          const std::list<GrammarRule>& rules,
          const std::string& starting_symbol);

  Grammar(const std::list<GrammarRule>& rules,
          const std::string& starting_symbol);
};


