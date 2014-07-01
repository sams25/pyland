#include <boost/python.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "api.h"
#include "debug.h"
#include "../main.h"

namespace py = boost::python;

Vec2D::Vec2D(int x, int y): x(x), y(y) {};

Vec2D Vec2D::operator+(Vec2D other) {
    return Vec2D(x + other.x, y + other.y);
}

void Vec2D::operator+=(Vec2D other) {
    x += other.x;
    y += other.y;
}

std::ostream& operator<<(std::ostream& out, Vec2D in) {
    return out << in.to_string();
}

std::string Vec2D::to_string() {
    std::ostringstream stringStream;
    stringStream << "(" << x << " " << y << ")";
    return stringStream.str();
}



Player::Player(Vec2D start, std::string name, int id):
    position(start), script(""), id(id) {
        this->name = std::string(name);
}

uint64_t Player::call_number = 0;

void Player::move(Vec2D by) {
    position += by;

    for (int dx=0; dx < by.x; ++dx) {
        move_object(id, 1);
    }
    for (int dx=0; dx < -by.x; ++dx) {
        move_object(id, 3);
    }
    for (int dy=0; dy < by.y; ++dy) {
        move_object(id, 0);
    }
    for (int dy=0; dy < -by.y; ++dy) {
        move_object(id, 2);
    }
}

void Player::monologue() {
    std::cout << "I am " << name << " and I am standing at " << position << "!" << std::endl;
}

void Player::run_script() {
    ++call_number;
    script(py::ptr(this));
}


void Player::give_script(py::api::object main_namespace) {
    py::api::object tempoary_scope = main_namespace.attr("copy")();
    std::string from_file =
        // TODO: find a more scalable approach
        "import time\n"
        "def move(x):\n"
        "    time.sleep(0.01)\n"
        "    player.move(x)\n"
    
        "def monologue():\n"
        "    player.monologue()\n"
    
        "north, south, east, west = Vec2D(0, 1), Vec2D(0, -1), Vec2D(1, 0), Vec2D(-1, 0)\n"
    
        "def script(player):\n"
        "    " + read_file();

    print_debug << from_file << std::endl;
    script = py::exec(from_file.c_str(), tempoary_scope);
    script = tempoary_scope["script"];
   // py::import("dis").attr("dis")(script);
}

std::string Player::read_file() {
    std::string loc = "python_embed/" + name + ".py";
    std::ifstream inFile (loc); //open the input file
    if (inFile.is_open()) { 
        std::stringstream strStream;
        strStream << inFile.rdbuf(); //read the file
        std::string old_text = strStream.str();
        boost::regex replace("\\n");
        std::string new_text = boost::regex_replace(old_text, replace, "\n    ");
        return new_text;
    } else {
        print_debug << "file opening unsuccessful" << std::endl;
        return "";
    }
}
