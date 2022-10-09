#ifndef __Parser_h__
#define __Parser_h__

#include <unordered_map>
#include <map>
#include <string>
#include <utility>

#ifndef __Matr_vals__
#define __Matr_vals__
struct pair_hash{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

using coords = std::pair<int, int>;
#endif  //__Matr_vals__

// for complex number first is real part, second is imaginary part
// for rational number first is numerator, second is denominator
// for other values first is the value, second is zero (meaningless)
using pair_str_vals = std::pair<std::string, std::string>;

template<class T>
class Matrix;

class Parser{
public:
    void parse_matrix(const char* filename);
    void parse_vector(const char* filename);

    int get_rows_number() const;
    int get_columns_number() const;
    std::pair<int, int> get_dims() const;
    std::string get_type() const;
    std::unordered_map<coords, pair_str_vals, pair_hash > get_vals() const;

    void print_matrix_config() const;
    void print_matrix_values() const;
private:
    // set delim1, delim2, delim3 based on type
    void _set_delimeters();
    void _parse_matrix_config(std::ifstream& file_data);
    void _parse_matrix_values(std::ifstream& file_data);
    void _parse_and_set_matr_value(const std::string& line);
    coords _parse_coords(std::stringstream& line);
    pair_str_vals _parse_val(std::stringstream& line);

    pair_str_vals __parse_complex(std::stringstream& line);
    pair_str_vals __parse_rational(std::stringstream& line);
    pair_str_vals __parse_single_val(std::stringstream& line);

    int rows, columns;
    std::string type;

    // delimerets for values: "delim_start value1 delim_middle value2 delim_end"
    bool is_single_val;
    char delim_start, delim_middle, delim_end;

    std::unordered_map<coords, pair_str_vals, pair_hash > matrix_vals;
};

#endif  //__Parser_h__