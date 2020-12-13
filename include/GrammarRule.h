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

  explicit GrammarRule(const std::string& rule);
  GrammarRule(std::string rule_lhs, std::string rule_rhs);
};
