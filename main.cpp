#ifndef MAIN
#define MAIN

#include <iostream>
#include <experimental/random>
#include <tuple>
#include <bitset>
#include <algorithm>
#include "world.cpp"

int main(){
    srand(213);
    SettingsStruct s;
    s.pop_size = 100;
    if(s.pop_size % 2 == 1){
        std::cout << "Please use an even population size!" << std::endl;
        exit(0);
    }
    s.starting_prog_size = 4;
    s.crossover_probability = 0.1;
    s.mutation_probability = 0.3;
    s.size_punishment_factor = 0.1;
    World * world = new World(&s);
    
    world->initialize_populations();
    std::cout << "generation 0 min error: " << world->get_min_error() << std::endl; 
    world->print_best_program_pair();
    if (s.track_data) world->data.next_generation(s.pop_size);

    for (int g = 1; g < 1000; g++){
        world->run_generation();
        std::cout << "\ngeneration " << g << " min error: " << world->get_min_error() << std::endl;
        world->print_best_program_pair();
    }
    
    if(s.track_data) world->data.write_data();
    delete world;
}

#endif
