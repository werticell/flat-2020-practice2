#include "include/EarleyParser.h"
#include <gtest/gtest.h>


TEST(First_example_test, FIRST) {

  std::string word = "(x+y)*z";
  std::list<std::string> nonterminals {"S", "P", "T"};
  std::list<std::string> alphabet {"x", "y", "z", "*", "(", ")", "+"};
  std::list<GrammarRule> rules_list = {GrammarRule("S->S+P"), GrammarRule("S->P"),
                                     GrammarRule("P->P*T"), GrammarRule("P->T"),
                                     GrammarRule("T->(S)"), GrammarRule("T->x"),
                                     GrammarRule("T->y"), GrammarRule("T->z")};
  std::string starting_symbol = "S";

  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);
  EarleyParser earley_parser;
  EXPECT_EQ(true, earley_parser.Parse(grammar, word));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}