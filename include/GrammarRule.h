#include <string>
#include <utility>
#include <vector>
#include <list>
#include <unordered_set>
#include <string_view>



const std::string rule_delimiter = {"->"};

class GrammarRule {
public:
  std::string rule_lhs_;
  std::string rule_rhs_;

 public:
  GrammarRule(const std::string& rule);
  GrammarRule(std::string rule_lhs, std::string rule_rhs);

  GrammarRule(const GrammarRule& other) = default;
  GrammarRule(GrammarRule&& other) = default;

  GrammarRule& operator=(const GrammarRule& other) = default;
  GrammarRule& operator=(GrammarRule&& other) = default;

  friend std::istream& operator>>(std::istream& in, GrammarRule& rule);
  friend std::ostream& operator<<(std::ostream& out, const GrammarRule& rule);


};


