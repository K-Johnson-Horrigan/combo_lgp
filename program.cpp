/**
 * This file includes code describing program individuals
 * which have component lines and nodes, and have individual operations
 * such as random initialization, mutation, and printing 
 */

#ifndef PROGRAM_CPP
#define PROGRAM_CPP

#include <experimental/random>
#include <tuple>
#include <bitset>
#include <iostream>
#include <vector>

enum SubNodeType {
    INPUT_1 = 0,
    INPUT_2,
    OUTPUT_1,
    OUTPUT_2,
    NOT, 
    AND,
    OR
};
std::string SUBTYPE_STRINGS[7] = {"INPUT_1", "INPUT_2", "OUTPUT_1", "OUTPUT_2", "NOT", "AND", "OR"};


class Node{
    public:
        int subtype;
        Node(int _snt): subtype(_snt){}
        Node(const Node* other): subtype(other->subtype){}
};

class Line{
    public:
        std::vector<Node*> nodes;
        int program_type; 
        
        Line(int _pt = 0): program_type(_pt){}
        ~Line(){ clear(); }
        Line(const Line* other) {
            program_type = other->program_type;
            for(int i = 0; i < other->nodes.size(); i++){
                nodes.push_back(new Node(other->nodes.at(i)->subtype));
            }
        }
        
        void clear(){
            for(int i = 0; i < nodes.size(); i++) delete nodes.at(i);
            nodes.clear();
        }

        void make_random_line(){
            clear();

            // pick a random output register to assign to
            Node* anchor = new Node((rand() % 2)+2); 
            
            nodes.push_back(anchor);
            
            int condition = rand() % 3;
            if(condition == 0){ // assign from some other register
                nodes.emplace_back(new Node(rand() % 4));
            } 
            else if(condition == 1){ // NOT a register
                nodes.emplace_back(new Node(NOT));
                nodes.emplace_back(new Node(rand() % 4));
            }
            else{ // OP registers
                nodes.emplace_back(new Node(rand() % 4));
                nodes.emplace_back(new Node(program_type + 5));
                nodes.emplace_back(new Node(rand() % 4));
            } 
        }

        void mutate(){
            // pick a random position in the line
            int node_pos = rand() % nodes.size();
            Node *node = nodes.at(node_pos);
            if(node->subtype == INPUT_1) node->subtype = INPUT_2;
            else if(node->subtype == INPUT_2) node->subtype = INPUT_1;
            else if(node->subtype == OUTPUT_1) node->subtype = OUTPUT_2;
            else if(node->subtype == OUTPUT_2) node->subtype = OUTPUT_1;
            else if(node->subtype == OR || node->subtype == AND){
                delete nodes.at(3);
                nodes.pop_back();
                Node *reg = nodes.at(1);
                nodes.at(1) = nodes.at(2);
                nodes.at(1)->subtype = NOT;
                nodes.at(2) = reg;
            }
            else if(node->subtype == NOT){
                Node *reg = nodes.at(2);
                nodes.at(2) = nodes.at(1);
                if(program_type == 0) nodes.at(2)->subtype = AND;
                else nodes.at(2)->subtype = OR;
                nodes.at(1) = reg;
                nodes.emplace_back(new Node(rand() % 4));
            }
            else{
                std::cout << "ERROR: invalid node subtype in line mutate" << std::endl;
                exit(0);
            }
        }

        void print_line(){
            std::cout << SUBTYPE_STRINGS[nodes.at(0)->subtype] << " = ";
            for(int i = 1; i < nodes.size(); i++) {
                std::cout << SUBTYPE_STRINGS[nodes.at(i)->subtype] << " ";
            }
            std::cout << std::endl;
        }

        void execute_line(std::vector<uint32_t> *registers){ // i1, i2, o1, o2
            if(nodes.size() == 2){
                registers->at(nodes.at(0)->subtype) = registers->at(nodes.at(1)->subtype);
            } 
            else if(nodes.size() == 3){
                registers->at(nodes.at(0)->subtype) = ~ registers->at(nodes.at(2)->subtype);
            }
            else if(nodes.size() == 4){
                registers->at(nodes.at(0)->subtype) = (program_type == 0) ?
                    registers->at(nodes.at(1)->subtype) & registers->at(nodes.at(3)->subtype) :
                    registers->at(nodes.at(1)->subtype) | registers->at(nodes.at(3)->subtype);
            }
            else{
                std::cout << "ERROR: too many nodes" << std::endl;
            }
        }
};

 
class Program{
    public:
        int program_type = 0;
        std::vector<Line*> lines;

        Program(int _pt = 0):   program_type(_pt){}
        ~Program(){clear();   }
        Program(const Program* other): program_type(other->program_type) {
          for(int i = 0; i < other->lines.size(); i ++ ){
            lines.emplace_back(new Line(other->lines.at(i)));
          }
        }
        
        void clear(){
            for(auto line: lines) delete line;
            lines.clear();
        }

        void build_random_program(int n = 1){
            for (int i = 0; i < n; i++){
                Line *line = new Line(program_type); 
                line->make_random_line();
                lines.push_back(line);
            }
        }

        void mutate(){
            // choose a random line
            int line_pos = (rand() % lines.size());
            Line* line = lines.at(line_pos);
            if(rand() % 2 == 1){ // major mutation
                delete line;
                Line * new_line = new Line(program_type); 
                new_line->make_random_line();
                lines.at(line_pos) = new_line;
            }
            else{ // minor mutation
                line->mutate();
            }
        }

        void print_program(){
            for(auto line: lines){
                line->print_line();
            }
        }

        void execute_program(std::vector<uint32_t>* registers) {
            for(auto line : lines){
                line->execute_line(registers);
            }
        }
};

#endif