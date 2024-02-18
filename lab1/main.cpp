#include <iostream>
#include <numeric>
#include <fstream>
#include <sstream>
#include <vector>

std::string vector_to_string(const std::vector<std::string>& vec)
{
    std::string result = "";

    for (int i = 0; i < vec.size(); ++i)
    {
        result += vec[i];

        if (i < vec.size() - 1) {
            result += ' ';
        }
    }

    return result;
}

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::istringstream stream(input);
    std::vector<std::string> tokens;
    std::string token;

    while (std::getline(stream, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

// "yesssss" "yes%%s"
bool if_string_like_mask(const std::string& str, const std::string& mask)
{
    if (str.length() < mask.length())
    {
        return false;
    }
    
    char char_repeat;

    auto it_regex = mask.begin();
    auto it_str = str.begin();
    while (it_regex != mask.end() && it_str != str.end())
    {
        if (*it_regex == '%')
        {
            char_repeat = *it_str;

            while (*it_str == char_repeat)
            {
                ++it_str;
            }

            ++it_regex;
        }
        else
        {
            if (*it_regex != *it_str && *it_str != char_repeat)
            {
                return false;
            }

            ++it_str;
            ++it_regex;
        }
    }

    return it_str == str.end() && it_regex == mask.end();
}


std::string replace_char_with_count(const std::string& str)
{
    std::string new_str;

    auto it = str.begin();
    while (it != str.end())
    {
        char char_repeat = *it;
        int char_count = 0;
        while (it != str.end() && *it == char_repeat)
        {
            ++char_count;
            ++it;
        }

        if (char_count > 1)
        {
            new_str += char_repeat;
            new_str += '(' + std::to_string(char_count) + ')';
        }
        else
        {
            new_str += char_repeat;
        }
    }

    return new_str;
}


void filter_file(const std::string& input_file, const std::string& output_file)
{
    std::ifstream input(input_file);
    std::ofstream output(output_file);

    if (input.is_open() && output.is_open())
    {
        std::string line;

        std::cout << "Містиме вхідного файлу: " << std::endl;
        while (std::getline(input, line))
        {
            if (line == "") continue;

            std::cout << line << std::endl;
            output << replace_char_with_count(line) << std::endl;
        }
        std::cout << std::endl;

        input.close();
    }
}

void print_words_that_fit_mask(const std::string& input_file, const std::string& mask)
{
    std::cout << "Маска: '" << mask << "'" << std::endl;

    std::ifstream input(input_file);
    if (input.is_open())
    {
        std::string line;
        while(std::getline(input, line))
        {
            if (line == "") continue;

            std::vector<std::string> words = splitString(line, ' ');
            std::cout << "Усі слова: " << vector_to_string(words) << std::endl;

            std::cout << "Слова, які підходять під маску: ";
            for (std::string word : words)
            {
                if (if_string_like_mask(word, mask))
                {
                    std::cout << word << ' ';
                }
            }
            std::cout << std::endl;
        }
    }
}

int main()
{
    std::vector<std::string> masks = {"%hgf%l", "ye%", "He%!", "Aw%some!"};
    int tests_count = masks.size();

    for (int i = 1; i <= tests_count; i++)
    {
        print_words_that_fit_mask("in/" + std::to_string(i) + "_in.txt", masks[i - 1]);
    }

    for (int i = 1; i <= tests_count; ++i)
    {
        filter_file("in/" + std::to_string(i) + "_in.txt", "out/" + std::to_string(i) + "_out.txt");
    }
    


    return 0;
}
