#pragma once
#include <godot_cpp/godot.hpp>

using namespace godot;

void register_initializer(ModuleInitializationLevel p_level);
void register_terminator(ModuleInitializationLevel p_level);
