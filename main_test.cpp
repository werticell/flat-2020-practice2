#include "include/EarleyParser.h"
#include <gtest/gtest.h>


TEST(Test_parser, FirstGrammar) {

  std::string word = "(x+y)*z";
  std::list<std::string> nonterminals {"S", "P", "T"};
  std::list<std::string> alphabet {"x", "y", "z", "*", "(", ")", "+"};
  std::list<GrammarRule> rules_list = {GrammarRule("S->S+P"), GrammarRule("S->P"),
                                     GrammarRule("P->P*T"), GrammarRule("P->T"),
                                     GrammarRule("T->(S)"), GrammarRule("T->x"),
                                     GrammarRule("T->y"), GrammarRule("T->z")};
  std::string starting_symbol = "S";

  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);
  EarleyParser earley_parser(grammar);
  EXPECT_EQ(true, earley_parser.Parse(word));
}

TEST(Test_parser, First_doesnt_belongs) {

  std::string word = "(x+y*z+)x*x)";
  std::list<std::string> nonterminals {"S", "P", "T"};
  std::list<std::string> alphabet {"x", "y", "z", "*", "(", ")", "+"};
  std::list<GrammarRule> rules_list = {GrammarRule("S->S+P"), GrammarRule("S->P"),
                                       GrammarRule("P->P*T"), GrammarRule("P->T"),
                                       GrammarRule("T->(S)"), GrammarRule("T->x"),
                                       GrammarRule("T->y"), GrammarRule("T->z")};
  std::string starting_symbol = "S";

  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);
  EarleyParser earley_parser(grammar);
  EXPECT_EQ(false, earley_parser.Parse(word));
}

TEST(Test_parser, Second_belongs) {

  std::string word = "(()()((())))()";
  std::list<std::string> nonterminals {"S"};
  std::list<std::string> alphabet {"(", ")", "e"};
  std::list<GrammarRule> rules_list = {GrammarRule("S->SS"), GrammarRule("S->EPS"),
                                       GrammarRule("S->(S)")};
  std::string starting_symbol = "S";

  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);
  EarleyParser earley_parser(grammar);
  EXPECT_EQ(true, earley_parser.Parse(word));
}

TEST(Test_parser, Second_doesnt_belongs) {

  std::string word = "(()())()((()))(()()";
  std::list<std::string> nonterminals {"S"};
  std::list<std::string> alphabet {"(", ")", "e"};
  std::list<GrammarRule> rules_list = {GrammarRule("S->SS"), GrammarRule("S->EPS"),
                                       GrammarRule("S->(S)")};
  std::string starting_symbol = "S";

  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);
  EarleyParser earley_parser(grammar);
  EXPECT_EQ(false, earley_parser.Parse(word));
}

TEST(Exceptions, empty_state) {
  ASSERT_ANY_THROW(GrammarRule("->(S)(S)"));
}

TEST(Exceptions, long_start) {
  ASSERT_ANY_THROW(GrammarRule("S()->(S)(S)"));
}


TEST(Scan, FIRST) {
  std::vector<std::unordered_set<EarleySituation, EarleySituationHash>> earley_sets(2);
  earley_sets[0] = {EarleySituation(GrammarRule("S->S+P"), 0, 0),
                      EarleySituation(GrammarRule("S->P"), 1, 0),
                      EarleySituation(GrammarRule("S->y"), 0, 0),
                      EarleySituation(GrammarRule("S->y"), 1, 0),
                      EarleySituation(GrammarRule("S->x"), 1, 0),
                      EarleySituation(GrammarRule("S->(P)y"), 3, 0),
                      EarleySituation(GrammarRule("S->(T)y"), 3, 0)
                  };

  EarleyFunctions::Scan(earley_sets, 0, std::string(1, 'y'));
  auto& first = earley_sets[1];
  ASSERT_EQ(earley_sets[1].size(), 3);
  ASSERT_TRUE(first.find(EarleySituation(GrammarRule("S->(P)y"), 4, 0))
            != first.end());
  ASSERT_TRUE(first.find(EarleySituation(GrammarRule("S->y"), 1, 0)) != first.end());
  ASSERT_TRUE(first.find(EarleySituation(GrammarRule("S->(T)y"), 4, 0)) != first.end());
}

TEST(Predict, FIRST) {
  std::list<std::string> nonterminals {"S", "P", "T"};
  std::list<std::string> alphabet {"x", "y", "z", "*", "(", ")", "+"};
  std::list<GrammarRule> rules_list = {GrammarRule("S->S+P"), GrammarRule("S->P"),
                                       GrammarRule("P->P*T"), GrammarRule("P->T"),
                                       GrammarRule("T->(S)"), GrammarRule("T->x"),
                                       GrammarRule("T->y"), GrammarRule("T->z"),
                                       GrammarRule("T->(P)y"), GrammarRule("T->(T)y")
  };
  std::string starting_symbol = "S";

  Grammar grammar(nonterminals, alphabet, rules_list, starting_symbol);

  std::vector<std::unordered_set<EarleySituation, EarleySituationHash>> earley_sets(3);
  earley_sets[2] =   {EarleySituation(GrammarRule("S->S+P"), 0, 2),
                      EarleySituation(GrammarRule("S->P"), 1, 2),
                      EarleySituation(GrammarRule("S->y"), 0, 2),
                      EarleySituation(GrammarRule("S->y"), 1, 2),
                      EarleySituation(GrammarRule("T->(P)y"), 1, 2),
                      EarleySituation(GrammarRule("T->(T)y"), 3, 2),
                      EarleySituation(GrammarRule("P->P*T"), 2, 2),
  };

  std::list<EarleySituation> result = {EarleySituation(GrammarRule("P->P*T"),0, 2),
                                       EarleySituation(GrammarRule("P->T"),0 , 2),
                                       EarleySituation(GrammarRule("T->(S)"), 0, 2),
                                       EarleySituation(GrammarRule("T->x"), 0, 2),
                                       EarleySituation(GrammarRule("T->y"), 0, 2),
                                       EarleySituation(GrammarRule("T->z"), 0, 2),
                                       EarleySituation(GrammarRule("T->(P)y"), 0, 2),
                                       EarleySituation(GrammarRule("T->(T)y"), 0, 2),
                                       EarleySituation(GrammarRule("S->P"), 0, 2)
  };

  for (const auto& situation : earley_sets[2]) {
    result.push_back(situation);
  }

  EarleyFunctions::Predict(earley_sets, grammar, 2);
  auto& first = earley_sets[2];
  ASSERT_EQ(earley_sets[2].size(), result.size());
  for (const auto& situation : result) {
    ASSERT_TRUE(first.find(situation) != first.end());
  }
}


TEST(Complete, FIRST) {

  std::vector<std::unordered_set<EarleySituation, EarleySituationHash>> earley_sets(5);
  earley_sets[4] = {EarleySituation(GrammarRule("S->S+P"), 1, 2),

                      EarleySituation(GrammarRule("T->y"), 1, 3),
                      EarleySituation(GrammarRule("T->(P)y"), 4, 3),
                      EarleySituation(GrammarRule("S->S+P"), 3, 3),

                      EarleySituation(GrammarRule("S->P"), 1, 2),
                      EarleySituation(GrammarRule("T->(T)y"), 4, 2),
                      EarleySituation(GrammarRule("T->(S)"), 3, 2),
                      EarleySituation(GrammarRule("P->P*T"), 3, 2),
  };

  earley_sets[3] = {EarleySituation(GrammarRule("S->S+P"), 0, 1),
                      EarleySituation(GrammarRule("T->(S)"), 1, 1),
                      EarleySituation(GrammarRule("S->(T)y"), 1, 1),
                      EarleySituation(GrammarRule("P->P*T"), 2, 1),

  };

  earley_sets[2] = {EarleySituation(GrammarRule("S->S+P"), 0, 0),
                      EarleySituation(GrammarRule("T->(S)"), 1, 0),

                      EarleySituation(GrammarRule("S->(T)y"), 1, 0),
                      EarleySituation(GrammarRule("P->P*T"), 2, 0),

                      EarleySituation(GrammarRule("S->P"), 0, 0),
                      EarleySituation(GrammarRule("S->(P)y"), 1, 0),
                      };


  std::list<EarleySituation> result = {EarleySituation(GrammarRule("S->S+P"), 1, 1),
                                       EarleySituation(GrammarRule("T->(S)"), 2, 1),
                                       EarleySituation(GrammarRule("S->(T)y"), 2, 1),
                                       EarleySituation(GrammarRule("P->P*T"), 3, 1),
                                        // from second
                                       EarleySituation(GrammarRule("S->S+P"), 1, 0),
                                       EarleySituation(GrammarRule("T->(S)"), 2, 0),
                                       EarleySituation(GrammarRule("S->(T)y"), 2, 0),
                                       EarleySituation(GrammarRule("P->P*T"), 3, 0),
                                       EarleySituation(GrammarRule("S->P"), 1, 0),
                                       EarleySituation(GrammarRule("S->(P)y"), 2, 0),
                                       };

  for (const auto& situation : earley_sets[4]) {
    result.push_back(situation);
  }


  EarleyFunctions::Complete(earley_sets, 4);
  auto& first = earley_sets[4];
  ASSERT_EQ(earley_sets[4].size(), result.size());
  for (const auto& situation : result) {
    ASSERT_TRUE(first.find(situation) != first.end());
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
