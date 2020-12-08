#include "../include/Grammar.h"

Grammar::Grammar(const std::list<std::string>& nonterminals,
                 const std::list<std::string>& alphabet,
                 const std::list<GrammarRule>& rules,
                 const std::string& starting_symbol) :
                 nonterminals_(nonterminals),
                 alphabet_(alphabet),
                 rules_list_(rules),
                 starting_symbol_(starting_symbol) {}


Grammar::Grammar(const std::list<GrammarRule>& rules,
                 const std::string& starting_symbol) :
                 rules_list_(rules),
                 starting_symbol_(starting_symbol) {}