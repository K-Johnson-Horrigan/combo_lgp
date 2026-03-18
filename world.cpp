#ifndef WORLD
#define WORLD

#include "program.cpp"

long MAX =  std::numeric_limits<uint32_t>::max();

struct SettingsStruct{
    int pop_size = 10;
    int starting_prog_size = 2;
    double crossover_probability = 0.3;
    double mutation_probability = 0.3;
    double size_punishment_factor = 0.5;
};

class World{
public:
    std::vector<Program*> programs_A;
    std::vector<Program*> programs_B;
    std::vector<double> program_errors;
    SettingsStruct* settings;
    
    World(SettingsStruct* _s): settings(_s){}
    ~World(){
        if(programs_A.size() != 0 && programs_B.size() != 0){
            clear_program_populations();
        }
    }

    void clear_program_populations(){
        for(int i = 0; i < settings->pop_size; i++){
            delete programs_A.at(i);
            delete programs_B.at(i);
        }
        programs_A.clear();
        programs_B.clear();
    }
    
    void initialize_populations(){
        for(int i = 0; i < settings->pop_size; i++){
            Program* prog_a = new Program(0);
            prog_a->build_random_program(settings->starting_prog_size);
            programs_A.push_back(prog_a);
            
            Program* prog_b = new Program(1);
            prog_b->build_random_program(settings->starting_prog_size);
            programs_B.push_back(prog_b);

            program_errors.push_back(MAX);
        }
        calculate_pop_error();
    }

    uint32_t calculate_correct_answer(uint32_t input_1, uint32_t input_2){
        return input_1 ^ input_2;
    }

    void calculate_pop_error(){
        for(int i = 0; i < settings->pop_size; i++){
            
            std::vector<uint32_t> registers {(uint32_t)(rand() % (MAX + 1)), (uint32_t)(rand() % (MAX + 1)), (uint32_t)(rand() % (MAX + 1)), (uint32_t)(rand() % (MAX + 1))};
            uint32_t correct_answer = calculate_correct_answer(registers.at(0), registers.at(1));

            programs_A.at(i)->execute_program(&registers);

            // set inputs to outputs and scramble outputs 
            registers.at(0) = registers.at(2);
            registers.at(1) = registers.at(3);
            registers.at(2) = (uint32_t)(rand() % (MAX + 1));
            registers.at(3) = (uint32_t)(rand() % (MAX + 1));

            programs_B.at(i)->execute_program(&registers);

            // originally program B was only going to have 1 output register; for coding simplicity it now has 2. The answer is expected in output 2
            correct_answer ^= registers.at(3);
            int mismatching_bits = std::bitset<32>(correct_answer).count();
            int prog_length = programs_A.at(i)->lines.size() + programs_B.at(i)->lines.size();
            program_errors.at(i) =  mismatching_bits + settings->size_punishment_factor * prog_length;
        }
    }

    double get_min_error(){
        return *std::min_element(program_errors.begin(), program_errors.end());
    }

    void print_best_program_pair(){
        int best_i = std::distance(std::begin(program_errors), std::min_element(program_errors.begin(), program_errors.end()));
        std::cout << "** PROGRAM A **" << std::endl;
        programs_A.at(best_i)->print_program();
        std::cout << "** PROGRAM B **" << std::endl;
        programs_B.at(best_i)->print_program();
    }


    void cross_programs(Program* program_1, Program* program_2){
        int prog_1_size = program_1->lines.size();
        int prog_2_size = program_2->lines.size(); 
        int program_1_cross_loc = rand() % prog_1_size;
        int program_2_cross_loc = rand() % prog_2_size;
        
        std::vector<Line*> lines_holder;
        // take everything cross point 1 onwards from prog 1 and put it into placeholder
        for(int i = program_1_cross_loc; i < prog_1_size; i++){
            lines_holder.push_back(program_1->lines.at(i));
        }
      
        // shrink if required prog 1 to be cross point 1 + (size prog 2 - cross point 2)s
        if(prog_1_size > program_1_cross_loc + (prog_2_size - program_2_cross_loc)){
            program_1->lines.resize(program_1_cross_loc + (prog_2_size - program_2_cross_loc));
        }

        // take everything cross point 2 onwards from prog 2 and put it in and after cross point 1 in prog 1
        for(int i = 0; i < prog_2_size - program_2_cross_loc; i++){
            if(program_1_cross_loc + i < prog_1_size) program_1->lines.at(program_1_cross_loc + i) = program_2->lines.at(program_2_cross_loc + i);
            else program_1->lines.push_back(program_2->lines.at(program_2_cross_loc + i));
        }

        // shrink if required prog 2 to be cross point 1 + (size prog 2 - cross point 2)
        if(prog_2_size > program_2_cross_loc + (prog_1_size - program_1_cross_loc)){
            program_2->lines.resize(program_2_cross_loc + (prog_1_size - program_1_cross_loc));
        }

        // take everything from the placeholder and put it in and after cross point 2 in prog 2
        for(int i = 0; i < prog_1_size - program_1_cross_loc; i++){
            if(program_2_cross_loc + i < prog_2_size) program_2->lines.at(program_2_cross_loc + i) = lines_holder.at(i);
            else program_2->lines.push_back(lines_holder.at(i));
        }
    } 

    void run_generation(){
        std::vector<Program*> child_programs_A;
        std::vector<Program*> child_programs_B;
        
        for(int i = 0; i < settings->pop_size; i += 2){
            // pick parents 
            int pos_1 = rand() % settings->pop_size;
            int pos_2 = rand() % settings->pop_size;
            int parent_1_pos = (program_errors.at(pos_1) < program_errors.at(pos_2)) ? pos_1 : pos_2;

            pos_1 = rand() % settings->pop_size;
            pos_2 = rand() % settings->pop_size;
            int parent_2_pos = (program_errors.at(pos_1) < program_errors.at(pos_2)) ? pos_1 : pos_2;

            // create child objects
            Program* prog_a_child_1 = new Program(programs_A.at(parent_1_pos));
            Program* prog_a_child_2 = new Program(programs_A.at(parent_2_pos));
            Program* prog_b_child_1 = new Program(programs_B.at(parent_1_pos));
            Program* prog_b_child_2 = new Program(programs_B.at(parent_2_pos));

            // cross children 
            if(((double)rand()/(double)RAND_MAX) < settings->crossover_probability){
                cross_programs(prog_a_child_1, prog_a_child_2);
                cross_programs(prog_b_child_1, prog_b_child_2);
            }

            // mutate children 
            if(((double)rand()/(double)RAND_MAX) < settings->mutation_probability){
                prog_a_child_1->mutate();
                prog_b_child_1->mutate();
                prog_a_child_2->mutate();
                prog_b_child_2->mutate();
            }
            child_programs_A.push_back(prog_a_child_1);
            child_programs_A.push_back(prog_a_child_2);
            child_programs_B.push_back(prog_b_child_1);
            child_programs_B.push_back(prog_b_child_2);
        }
        clear_program_populations();
        programs_A = child_programs_A;
        programs_B = child_programs_B;
        calculate_pop_error();
    }
};


#endif