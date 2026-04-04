#ifndef DATA
#define DATA
#include <fstream>
class Data {
public:
	std::vector<double> mean_error_values;
	std::vector<double> min_error_values;
	std::vector<double> mean_non_struct_ef;
	std::vector<double> min_non_struct_ef;
	std::vector<double> mean_non_sem_ef;
	std::vector<double> min_non_sem_ef;

	double total_error_values = 0;
	double min_error_value = -1;
	double total_non_struct_ef_props = 0;
	double min_non_struct_ef_prop = -1;
	double total_non_sem_ef_props = 0;
	double min_non_sem_ef_prop = -1;

	void next_generation(int pop_size) {
		mean_error_values.push_back(total_error_values / pop_size);
		min_error_values.push_back(min_error_value);

		min_error_value = -1;
		total_error_values = 0;
		
		mean_non_struct_ef.push_back(total_non_struct_ef_props / pop_size);
		min_non_struct_ef.push_back(min_non_struct_ef_prop);

		min_non_struct_ef_prop = -1;
		total_non_struct_ef_props = 0;

		mean_non_sem_ef.push_back(total_non_sem_ef_props / pop_size);
		min_non_sem_ef.push_back(min_non_sem_ef_prop);

		min_non_sem_ef_prop = -1;
		total_non_sem_ef_props = 0;
	}

	void add_error_value(double error_val){
		total_error_values += error_val;
		if (min_error_value == -1 || error_val < min_error_value) {
			min_error_value = error_val;
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

	void write_data(std::string line_prefix_header = "", std::string line_prefix = "") {
		//todo: check vectors are all the same size
		std::ofstream data_file;
		data_file.open("out_data.csv");
		data_file << line_prefix_header << "generation,mean_error,min_error,mean_non_struct_ef,min_non_struct_ef,mean_non_sem_ef,min_non_sem_ef\n";
		for (int i = 0; i < mean_error_values.size(); i++) {
			data_file << line_prefix << i;
			data_file << "," << mean_error_values.at(i) << "," << min_error_values.at(i);
			data_file << "," << mean_non_struct_ef.at(i) << "," << min_non_struct_ef.at(i);
			data_file << "," << mean_non_sem_ef.at(i) << "," << min_non_sem_ef.at(i);
			data_file << "\n";
		}
		data_file.close();
	}
};
#endif