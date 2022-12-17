#ifndef __GoLEngine__
#define __GoLEngine__

#include <bitset>
#include <fstream>
#include <stdexcept>
#include <ncurses.h>
#include "Sizes.h"

class GoLEngine{
public:
    GoLEngine(): width(WIDTH), height(HEIGHT) {};
    GoLEngine(const GoLEngine& other) = default;
    ~GoLEngine() = default;

    std::bitset<WIDTH * HEIGHT> 
    get_descedant(const std::bitset<WIDTH * HEIGHT>& individ){
        auto tmp = individ;
        int m, pos;
        for (unsigned int i = 0; i < width; i++){
            for (unsigned int j = 0; j < height; j++) {
                m = count_neightbours(individ, i, j);
                //std::cout << m << " ";
                pos = i * WIDTH + j;
                if (m == 3){
                    tmp[pos] = 1;
                }
                if (m != 3 && m != 2){
                    tmp[pos] = 0;
                }
            }
            //std::cout << std::endl;
        }
        return tmp;
    }

    std::bitset<WIDTH * HEIGHT> 
    get_great_descedant(const std::bitset<WIDTH * HEIGHT>& individ, unsigned generations,
                        bool visualize = false, int wait_time=5){
        field = individ;
        if (visualize){
            init_visualize(wait_time);
            print_field();
        }
        for (unsigned i = 0; i < generations; i++){
            field = get_descedant(field);
            if (visualize){
                print_field();
            }
        }

        if (visualize){
            end_visualize();
        }
        return field;            
    }

    static void print_field(const std::bitset<WIDTH * HEIGHT>& individ);
    static void individ_to_file(const std::bitset<WIDTH * HEIGHT>& individ, const char* filename);

private:
    const unsigned width;
    const unsigned height;
    std::bitset<WIDTH * HEIGHT> field;
    std::bitset<WIDTH * HEIGHT> next_field;

    int count_neightbours(const std::bitset<WIDTH * HEIGHT>& individ, int x, int y);
    void print_field() const;
    
    void init_visualize(int wait_time) {
        initscr();
        halfdelay(wait_time);   // waittime
    }

    void end_visualize(){
        endwin();
    }
};

int GoLEngine::count_neightbours(const std::bitset<WIDTH * HEIGHT>& individ, int x, int y){
    int count = 0;
    if (x != 0 && x != (WIDTH-1) && y != 0 && y != (HEIGHT - 1)){
        count += individ[(x-1)*(width) + (y)];
        count += individ[(x)*(width) + (y-1)];
        count += individ[(x-1)*(width) + (y-1)];
        count += individ[(x+1)*(width) + (y)];

        count += individ[(x)*(width) + (y+1)];
        count += individ[(x+1)*(width) + (y+1)];
        count += individ[(x+1)*(width) + (y-1)];
        count += individ[(x-1)*(width) + (y+1)];
        return count;
    }

    if (x == 0 && y == 0){
        count += individ[1] + individ[WIDTH] + individ[WIDTH + 1]; 
        return count;
    }

    if (x == 0 && y == (HEIGHT - 1)){
        count += individ[HEIGHT - 2] + individ[WIDTH + HEIGHT - 1] + individ[WIDTH + HEIGHT - 2];
        return count;
    }

    if (x == (WIDTH - 1) && y == (HEIGHT - 1)){
        count += individ[WIDTH*HEIGHT - 2] + individ[WIDTH*(HEIGHT-1) - 1] + individ[WIDTH*(HEIGHT-1) - 2];
        return count;
    }

    if (x == (WIDTH - 1) && y == 0){
        count += individ[WIDTH*(HEIGHT-2)] + individ[WIDTH*(HEIGHT-2) + 1] + individ[WIDTH*(HEIGHT-1) + 1];
        return count;
    }

    if (x == 0){
        count += individ[WIDTH + y] + individ[WIDTH + y-1] + individ[WIDTH + y+1] +
                    individ[y-1] + individ[y+1];
        return count;
    }

    if (x == WIDTH-1){
        count += individ[WIDTH * (HEIGHT-2) + y] + individ[WIDTH*(HEIGHT-2) + y-1] + 
                    individ[WIDTH*(HEIGHT-2) + y+1] + individ[WIDTH*(HEIGHT-1) + y-1] +
                    individ[WIDTH*(HEIGHT-1) + y+1];
        return count; 
    }

    if (y == 0){
        count += individ[(x-1)*(WIDTH)] + individ[(x+1)*WIDTH] + individ[x*WIDTH + 1]+
                    individ[(x+1)*WIDTH + 1] + individ[(x-1)*WIDTH + 1];
    }

    if (y == HEIGHT-1){
        count += individ[(x-1)*(WIDTH) + y] + individ[(x+1)*WIDTH + y] + individ[x*WIDTH + y - 1]+
                 individ[(x+1)*WIDTH + y - 1] + individ[(x-1)*WIDTH + y - 1];
        return count;
    }

    return count;
}


void GoLEngine::print_field() const{
    GoLEngine::print_field(field);
}


void GoLEngine::print_field(const std::bitset<WIDTH * HEIGHT>& individ){
    for(unsigned i = 0; i < WIDTH; i++){
        for (unsigned j=0; j < HEIGHT; j++){
            printw("%c ", (individ[i * WIDTH + j]) ? 'X' : '-');
        }
        printw("\n");
    }
    refresh();
    getch();
    clear();
}


void GoLEngine::individ_to_file(const std::bitset<WIDTH * HEIGHT>& individ, const char* filename){
    std::ofstream file_data;
    file_data.open(filename);
    if (!file_data.is_open()){
        throw std::runtime_error("Fail opening file: ");
    }

    for(unsigned i = 0; i < WIDTH; i++){
        for (unsigned j=0; j < HEIGHT; j++){
            file_data << ((individ[i * WIDTH + j]) ? 'X' : '-' ) << " ";
        }
        file_data << "\n";
    }

    file_data.close();
}

#endif // __GoLEngine__