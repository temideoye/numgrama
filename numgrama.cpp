/*
 * Numgrama.
 *
 * Returns the gramatically correct English phrase of input decimal number.
 * Decimal range: [1, 1001)
 *
 * For range > 1000, include the following number notations in groups:
 *   googolplex googol octillion septillion sextillion
 *   quintillion quadrillion trillion billion million
 *
 * Copyright: June 2019, Temi Adeoye
 * License: See LICENSE for details
 */

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::find;
using std::replace;
using std::stod;
using std::stoi;
using std::string;
using std::vector;

vector<string> units = {"zero", "one", "two",   "three", "four",
                        "five", "six", "seven", "eight", "nine"};

vector<string> earlytens = {"ten",      "eleven",  "twelve",  "thirteen",
                            "fourteen", "fifteen", "sixteen", "seventeen",
                            "eighteen", "nineteen"};

vector<string> latetens = {"twenty", "thirty",  "fourty", "fifty",
                           "sixty",  "seventy", "eighty", "ninety"};

// for range [1, 1001)
vector<string> groups = {"billion", "million", "thousand"};

vector<string> seperator(string);
string join(vector<string>&);
string cardinality(int);

int main(int argc, char** argv) {
  // Compute english phrase.
  if (argc < 2) {
    cout << "Numgrama takes exactly one argument, none was provided." << endl;
    return 1;
  }
  string strarg(argv[1]);

  // strip comma from argument
  replace(strarg.begin(), strarg.end(), ',', '\0');

  // for range [1, 1001)
  if (stod(strarg) > 1000) {
    cout << "Acceptable input decimal range is [1, 1001)" << endl;
    return 1;
  }

  string whole = strarg;
  string fractional = "";

  // check for fractional parts
  string::iterator dotiter = find(strarg.begin(), strarg.end(), '.');
  if (dotiter != strarg.end()) {
    whole = string(strarg.begin(), dotiter);
    fractional = string(dotiter, strarg.end());
  }

  vector<string> sep = seperator(whole);
  vector<string> result;
  vector<string>::const_iterator group;
  int index = 0;

  for (group = sep.begin(); group != sep.end(); ++group) {
    int value = stoi(*group);

    // append group, ignoring hundreds and 'zero thousands'
    if (value != 0) {
      result.push_back(cardinality(value));
      if (sep.size() > 1 && group + 1 != sep.end()) {
        vector<string> normgroups(groups.end() - sep.size() + 1, groups.end());
        result.push_back(normgroups[index]);
      }
    }
    ++index;
  }

  if (fractional.size()) {
    result.push_back("point");
    for (char chr : vector<char>(fractional.begin(), fractional.end())) {
      int digit = chr - '0';  // '0' == 48
      if (digit == 0) {
        result.push_back("zero");
      } else {
        result.push_back(cardinality(digit));
      }
    }
  }

  cout << join(result) << endl;

  return 0;
}

vector<string> seperator(string digits) {
  vector<string> sep;
  while (digits.size() > 3) {
    sep.insert(sep.begin(), digits.substr(digits.size() - 3));
    digits = digits.substr(0, digits.size() - 3);
  }

  // if digits not empty at the end of loop
  if (digits.size()) sep.insert(sep.begin(), digits);

  return sep;
}

string join(vector<string>& v) {
  vector<string>::iterator iter = v.begin();
  string joined_string;
  while (true) {
    joined_string.insert(joined_string.size(), *iter);
    ++iter;
    if (iter != v.end()) {
      joined_string.insert(joined_string.size(), " ");
      continue;
    }
    break;
  }

  return joined_string;
}

string cardinality(int num) {
  // Calculate cardinality of input digits.
  vector<string> cardinal;
  int ones = num % 10;
  int tens = num % 100;
  int hundreds = num % 1000;
  bool in_earlytens = tens > 9 && tens < 20;            // between 10 and 19
  bool in_latetens = tens > 19 && tens < 100;           // between 20 and 99
  bool joinable = in_earlytens || in_latetens || ones;  // between 1 and 99
  int round_hundred = hundreds - tens;  // round down to nearest hundred

  if (round_hundred) {
    cardinal.push_back(units[round_hundred / 100] + " hundred");
  }
  if (round_hundred && joinable) {
    cardinal.push_back("and");
  }
  if (in_earlytens) {
    cardinal.push_back(earlytens[tens - 10]);
  }
  if (in_latetens) {
    // index latetens 2 places left
    string net_tens = latetens[(tens / 10) - 2];
    if (ones) {
      cardinal.push_back(net_tens + "-" + units[ones]);
    } else {
      cardinal.push_back(net_tens);
    }
  }
  if (ones && !(in_earlytens || in_latetens)) {
    cardinal.push_back(units[ones]);
  }

  return join(cardinal);
}
