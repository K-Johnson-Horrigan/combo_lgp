#ifndef MAIN
#define MAIN

#include <iostream>
#include <experimental/random>
#include <tuple>
#include <bitset>
#include <algorithm>
#include "world.cpp"

int main(){
    
    SettingsStruct s;
    s.pop_size = 100;
    if(s.pop_size % 2 == 1){
        std::cout << "Please use an even population size!" << std::endl;
        exit(0);
    }
    
    s.crossover_probability = 0.1;
    s.mutation_probability = 0.3;
    s.size_punishment_factor = 0.1;
    s.track_data = 1;
    s.track_ineffective = 0;
    s.verbose = 0; // print the best solution of each update if true
    s.world_type = 1;
    // world type 0: prog A does AND only, prog B does OR only
    // world type 1: prog A and B do AND and OR
    // world type 1: prog A does AND and OR

    int _seed = 1200;
    for(int _world_type = 0; _world_type <= 2; _world_type++){
        s.starting_prog_size = 4;
        if(_world_type == 2) s.starting_prog_size = 8;
        s.world_type = _world_type;
        
        std::vector<double> param_list = {0.01, 0.05, 0.1, 0.5, 1, 5};
        for(int param_i = 0; param_i < param_list.size(); param_i++){
            std::cout << "world, param: " << _world_type << ", " << param_list.at(param_i) << std::endl;
            s.size_punishment_factor = param_list.at(param_i);

            for(int rep_count = 0; rep_count < 20; rep_count ++){
                s.seed = _seed;
                _seed++;

                srand(s.seed);
                World * world = new World(&s);
                world->initialize_populations();

                if(s.verbose) {
                    std::cout << "generation 0 min error: " << world->get_min_error() << std::endl; 
                    world->print_best_program_pair();
                }
                if (s.track_data) world->data.next_generation(s.pop_size);

                for (int g = 1; g < 1000; g++){
                    world->run_generation();
                    
                    if(s.verbose) {
                        std::cout << "\ngeneration " << g << " min error: " << world->get_min_error() << std::endl;
                        world->print_best_program_pair();
                    }
                }
                
                std::string line_prefix_header = "test,seed,cross_prob,mut_prob,size_punishment,pop_size,world_type,";
                std::string line_prefix = "punish_factor," + std::to_string(s.seed) + "," +  std::to_string(s.crossover_probability) + "," ;
                line_prefix += std::to_string(s.mutation_probability) + "," +  std::to_string(s.size_punishment_factor) + "," ;
                line_prefix += std::to_string(s.pop_size) + "," +  std::to_string(s.world_type) + "," ;

                std::string file_name = "data/seed_"+std::to_string(s.seed)+"_world_" + std::to_string(s.world_type) + "_punish_factor_" + std::to_string(s.crossover_probability) +"_data.csv";
                if(s.track_data) world->data.write_data(file_name, line_prefix_header, line_prefix);
                
                delete world;
            }
        }    
    }
}

#endif
