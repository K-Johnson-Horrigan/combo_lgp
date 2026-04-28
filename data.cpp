#ifndef DATA
#define DATA
#include <fstream>
class Data {
public:
	// generational data vectors
	std::vector<double> mean_error_values;
	std::vector<double> min_error_values;

	std::vector<double> mean_bits_values;
	std::vector<double> min_bits_values;

	std::vector<int> mean_length_values;
	std::vector<int> min_error_length_values;

	std::vector<int> mean_A_length_values;
	std::vector<int> min_A_error_length_values;

	std::vector<double> mean_non_struct_ef;
	std::vector<double> min_non_struct_ef;

	std::vector<double> mean_non_sem_ef;
	std::vector<double> min_non_sem_ef;

	// current generation data
	double total_error_values = 0;
	double min_error_value = -1;

	int total_bits_values = 0;
	int min_bits_value = -1;

	int total_length_values = 0;
	int min_error_length_value = -1;

	int total_A_length_values = 0;
	int min_A_error_length_value = -1;

	double total_non_struct_ef_props = 0;
	double min_non_struct_ef_prop = -1;

	double total_non_sem_ef_props = 0;
	double min_non_sem_ef_prop = -1;

	bool track_ineffective = false;

	void next_generation(int pop_size) {
		mean_error_values.push_back(total_error_values / pop_size);
		min_error_values.push_back(min_error_value);

		min_error_value = -1;
		total_error_values = 0;

		mean_bits_values.push_back(total_bits_values / pop_size);
		min_bits_values.push_back(min_bits_value);

		min_bits_value = -1;
		total_bits_values = 0;

		mean_length_values.push_back(total_length_values / pop_size);
		min_error_length_values.push_back(min_error_length_value);

		min_error_length_value = -1;
		total_length_values = 0;

		mean_A_length_values.push_back(total_A_length_values / pop_size);
		min_A_error_length_values.push_back(min_A_error_length_value);

		min_A_error_length_value = -1;
		total_A_length_values = 0;
		
		if(track_ineffective){
			mean_non_struct_ef.push_back(total_non_struct_ef_props / pop_size);
			min_non_struct_ef.push_back(min_non_struct_ef_prop);

			min_non_struct_ef_prop = -1;
			total_non_struct_ef_props = 0;

			mean_non_sem_ef.push_back(total_non_sem_ef_props / pop_size);
			min_non_sem_ef.push_back(min_non_sem_ef_prop);

			min_non_sem_ef_prop = -1;
			total_non_sem_ef_props = 0;
		}
	}

	void add_error_and_length_values(double bits_val, double error_val, int length_val, int A_length_value){
		total_error_values += error_val;
		total_bits_values += bits_val;
		total_length_values += length_val;
		total_A_length_values += A_length_value;
		if (min_error_value == -1 || error_val < min_error_value) {
			min_error_value = error_val;
			min_bits_value = bits_val;
			min_error_length_value = length_val;
			min_A_error_length_value = A_length_value;
		}
	}


	void add_non_struct_ef_prop(double non_struct_ef) {
		total_non_struct_ef_props += non_struct_ef;
		if (min_non_struct_ef_prop == -1 || non_struct_ef < min_non_struct_ef_prop) {
			min_non_struct_ef_prop = non_struct_ef;
		}
	}

	void add_non_sem_ef_prop(double non_sem_ef) {
		total_non_sem_ef_props += non_sem_ef;
		if (min_non_sem_ef_prop == -1 || non_sem_ef < min_non_sem_ef_prop) {
			min_non_sem_ef_prop = non_sem_ef;
		}
	}

	void write_data(std::string file_name = "out_data.csv", std::string line_prefix_header = "", std::string line_prefix = "") {
		//todo: check vectors are all the same size
		std::ofstream data_file;
		data_file.open(file_name);
		data_file << line_prefix_header << "generation,mean_error,min_error,mean_bits_mistmatching,min_error_bits_mismatching,mean_length,min_error_length,mean_A_length,min_A_error_length";
		if(track_ineffective){ data_file << ",mean_non_struct_ef,min_non_struct_ef,mean_non_sem_ef,min_non_sem_ef"; }
		data_file << "\n";
		for (int i = 0; i < mean_error_values.size(); i++) {
			data_file << line_prefix << i;

			data_file << "," << mean_error_values.at(i) << "," << min_error_values.at(i);
			data_file << "," << mean_bits_values.at(i) << "," << min_bits_values.at(i);
			data_file << "," << mean_length_values.at(i) << "," << min_error_length_values.at(i);
			data_file << "," << mean_A_length_values.at(i) << "," << min_A_error_length_values.at(i);
			if(track_ineffective){
				data_file << "," << mean_non_struct_ef.at(i) << "," << min_non_struct_ef.at(i);
				data_file << "," << mean_non_sem_ef.at(i) << "," << min_non_sem_ef.at(i);
			}
			data_file << "\n";
		}
		data_file.close();
	}
};
#endif