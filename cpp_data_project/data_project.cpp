// PHYS 30762 Programming in C++
// Assignment 2

// Program to compute mean, standard deviation and standard
// error of the a set of courses. Data is read from file

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<vector>
#include<algorithm>

float mean(std::vector<float> entries)
//Mean calculation
{
    float sum{0};
    float size_of_entries;
    size_of_entries = entries.size();
    for(unsigned int i = 0; i < size_of_entries; i++){
        sum += entries[i];
    }
    float mean{sum/size_of_entries};
    return mean;
}

float standard_deviation(std::vector<float> entries)
//Standard Deviation calculation
{
    float current_mean{mean(entries)};
    float sum{0};
    float size_of_entries;
    size_of_entries = entries.size();
    for(unsigned int i = 0; i < size_of_entries; i++){
        float value{entries[i]};
        sum +=  (value - current_mean)*(value - current_mean);
    }  
    float fraction{1/(size_of_entries-1)};
    float intermediate_step{fraction*sum};
    float standard_deviation;
    standard_deviation = sqrt(intermediate_step);
    return standard_deviation;
}

float error_in_mean(std::vector<float> entries)
//Error in mean calculation
{
    float standard_error{standard_deviation(entries)};
    float size_of_entries;
    size_of_entries = entries.size();
    float error_in_mean;
    error_in_mean = standard_error/sqrt(size_of_entries);
    return error_in_mean;
}

std::string ask_file_name()
//Simple function to ask for a file name (Adds .dat automatically)
{
    std::string file_name; 
    std::cout << "Please input a valid file name:" << std::endl;
    std::cin >> file_name;
    file_name = file_name + ".dat";
    return file_name;
    
}

void read_data(std::vector<std::string> &course_name_data, std::vector<int> &course_code_data, std::vector<float> &course_grade_data)
//Function using referencing to read file into 3 different vectors
{
    std::string data_file_name;
    std::ifstream myfile;
    std::string string_storage;
    data_file_name = ask_file_name();
    myfile.open(data_file_name);
    while (!myfile.eof()){
        std::getline(myfile, string_storage, '\n');
        std::istringstream converted_string(string_storage);
        float course_grade;
        std::string course_name;
        int course_code;
        std::string temporary_string;
        if(string_storage == ""){
            continue;
        }
        converted_string >> course_grade >> course_code;
        while(converted_string >> temporary_string){
            course_name += temporary_string;
            course_name += " ";
        }
        course_name_data.push_back(course_name);
        course_grade_data.push_back(course_grade);
        course_code_data.push_back(course_code); 
               
    }
    myfile.close();
}

std::vector<std::string> put_vectors_together(std::vector<std::string> course, std::vector<int> course_code, std::vector<float> grade)
//Function for merging vectors
{
    int length = course.size();
    std::vector<std::string> new_vector;
    for(unsigned int i = 0; i < length;i++){
        std::stringstream temp;
        temp << "PHYS "<< course_code[i] << " " << course[i]  << "  " << grade[i];
        new_vector.push_back(temp.str());
    }
    return new_vector;
}

std::vector<float> convert_vector_str_float(std::vector<std::string> &merged_vector)
//Function to convert vectors
{
    std::vector<float> float_vector;
    std::transform(merged_vector.begin(), merged_vector.end(), std::back_inserter(float_vector), [](std::string x) {
        std::stringstream temp(x.substr(x.length() - 4));
        float f_temp;
        temp >> std::skipws >> f_temp;
        return f_temp;
    });
    return float_vector;
}

void print_data(std::vector<std::string> merged_vector)
//Use of iterator to print out the merged vector
{
    int length = merged_vector.size();
    std::vector<std::string>::iterator merged_vector_start{merged_vector.begin()};
    std::vector<std::string>::iterator merged_vector_end{merged_vector.end()};
    std::vector<std::string>::iterator merged_vector_iterator;
    for(merged_vector_iterator = merged_vector_start; merged_vector_iterator < merged_vector_end; ++merged_vector_iterator){
        std::cout << *merged_vector_iterator << std::endl;
    }
}

void sort_name_alphabetically(std::vector<std::string> &merged_vector)
//Function to sort name alphabetically 
{
    std::sort(merged_vector.begin(), merged_vector.end(), [] (std::string x, std::string y) {
        return x.substr(11) < y.substr(11);
    });
}

void sort_code_numerically(std::vector<std::string> &merged_vector)
//Function sorting course code numerically
{
    std::sort(merged_vector.begin(), merged_vector.end(), [] (std::string x, std::string y) {
        return x.substr(5,5) < y.substr(5,5);
    });
}

void filter_code_year(int year, std::vector<std::string> &merged_vector)
//Function sorting course code by year
{
    std::vector<std::string>::iterator itr = std::remove_if(merged_vector.begin(), merged_vector.end(), [year] (std::string x){
        return std::stoi(x.substr(5,1)) != year;
    });

    merged_vector.erase(itr, merged_vector.end());
}

int ask_for_user_input()
//Function asking for an integer input for sorting data
{
    int user_input;
    std::cout << "Input an integer for the following:" << std::endl;
    std::cout << "Sort data for year 1 (1)" << std::endl;
    std::cout << "Sort data for year 2 (2)" << std::endl;
    std::cout << "Sort data for year 3 (3)" << std::endl;
    std::cout << "Sort data for year 4 (4)" << std::endl;
    std::cout << "Sort data alphabetically (5)" << std::endl;
    std::cout << "Sort data numerically by course code (6)" << std::endl;
    std::cin >> user_input;
    return user_input;
}

int main()
{
    std::vector<std::string> course_name;
    std::vector<int> course_code;
    std::vector<float> course_grade;
    read_data(course_name, course_code, course_grade);
    std::vector<std::string> combined_vector{put_vectors_together(course_name, course_code, course_grade)};
    print_data(combined_vector);
    std::vector<float> new_vector{convert_vector_str_float(combined_vector)};
    std::cout << "The Mean is: " << mean(new_vector) << std::endl;
    std::cout << "The Standard Deviation is: " << standard_deviation(new_vector) << std::endl;
    std::cout << "The Error in Mean is: " << error_in_mean(new_vector) << std::endl;

    int user_input{ask_for_user_input()};
    //Else If statements to calculate and sort data 
    if(user_input == 1){
        filter_code_year(1, combined_vector);
        print_data(combined_vector);
        std::vector<float> year_1_grades{convert_vector_str_float(combined_vector)};
        float average{mean(year_1_grades)};
        float standard_dev(standard_deviation(year_1_grades));
        float mean_error(error_in_mean(year_1_grades));
        std::cout << "The Mean is: " << average << std::endl;
        std::cout << "The Standard Deviation is: " << standard_dev << std::endl;
        std::cout << "The Error in Mean is: " << mean_error << std::endl;

    }
    else if(user_input == 2){
        filter_code_year(2, combined_vector);
        print_data(combined_vector);
        std::vector<float> year_2_grades{convert_vector_str_float(combined_vector)};
        float average{mean(year_2_grades)};
        float standard_dev(standard_deviation(year_2_grades));
        float mean_error(error_in_mean(year_2_grades));
        std::cout << "The Mean is: " << average << std::endl;
        std::cout << "The Standard Deviation is: " << standard_dev << std::endl;
        std::cout << "The Error in Mean is: " << mean_error << std::endl;
    
    }
    else if(user_input == 3){
        filter_code_year(3, combined_vector);
        print_data(combined_vector);
        std::vector<float> year_3_grades{convert_vector_str_float(combined_vector)};
        float average{mean(year_3_grades)};
        float standard_dev(standard_deviation(year_3_grades));
        float mean_error(error_in_mean(year_3_grades));
        std::cout << "The Mean is: " << average << std::endl;
        std::cout << "The Standard Deviation is: " << standard_dev << std::endl;
        std::cout << "The Error in Mean is: " << mean_error << std::endl;
    
    }
    else if(user_input == 4){
        filter_code_year(4, combined_vector);
        print_data(combined_vector);
        std::vector<float> year_4_grades{convert_vector_str_float(combined_vector)};
        float average{mean(year_4_grades)};
        float standard_dev(standard_deviation(year_4_grades));
        float mean_error(error_in_mean(year_4_grades));
        std::cout << "The Mean is: " << average << std::endl;
        std::cout << "The Standard Deviation is: " << standard_dev << std::endl;
        std::cout << "The Error in Mean is: " << mean_error << std::endl;

    }
    else if(user_input == 5){
        sort_name_alphabetically(combined_vector);
        print_data(combined_vector);
    }
    else if(user_input == 6){
        sort_code_numerically(combined_vector);
        print_data(combined_vector);
    }
   
    return 0;
}


